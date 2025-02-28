/*
 * Copyright (C), 2000-2004 by the monit project group.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include <config.h>

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef TIME_WITH_SYS_TIME
#include <time.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#else
#include <time.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_KVM_H
#include <kvm.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_SYS_PROC_H
#include <sys/proc.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#ifdef HAVE_SYS_RESOURCEVAR_H
#include <sys/resourcevar.h>
#endif

#ifdef HAVE_SYS_LOCK_H
#include <sys/lock.h>
#endif

#ifdef HAVE_SYS_USER_H
#include <sys/user.h>
#endif

#ifdef HAVE_VM_VM_H
#include <vm/vm.h>
#endif

#ifdef HAVE_VM_VM_OBJECT_H
#include <vm/vm_object.h>
#endif

#ifdef HAVE_VM_PMAP_H
#include <vm/pmap.h>
#endif

#ifdef HAVE_MACHINE_PMAP_H
#include <machine/pmap.h>
#endif

#ifdef HAVE_MACHINE_VMPARAM_H
#include <machine/vmparam.h>
#endif

#ifdef HAVE_VM_VM_MAP_H
#include <vm/vm_map.h>
#endif

#ifdef HAVE_SYS_VMMETER_H
#include <sys/vmmeter.h>
#endif

#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif

#include "monitor.h"
#include "process.h"
#include "process_sysdep.h"

/**
 *  System dependent resource gathering code for FreeBSD.
 *
 *  @author Jan-Henrik Haukeland, <hauk@tildeslash.com>
 *  @author Christian Hopp <chopp@iei.tu-clausthal.de>
 *  @author Rory Toma <rory@digeo.com>
 *
 *  @version \$Id: sysdep_FREEBSD.c,v 1.13 2004/02/29 22:24:45 martinp Exp $
 *
 *  @file
 */

#define pagetok(size) ((size) << pageshift)
#define tv2sec(tv) (((u_int64_t) tv.tv_sec * 1000000) + (u_int64_t) tv.tv_usec)

static int pageshift;

#ifndef LOG1024
#define LOG1024         10
#endif

static kvm_t * kvm_handle;

static void calcru(struct proc *p, struct timeval *up, struct timeval *sp,
		   struct timeval *ip)
{
  quad_t totusec;
  u_quad_t u, st, ut, it, tot;
#if (__FreeBSD_version < 300003) || (__FreeBSD_version >= 500000) 
  long sec, usec;
#endif 
  
  st = p->p_sticks;
  ut = p->p_uticks;
  it = p->p_iticks;
  
  tot = st + ut + it;
  if (tot == 0)
    {
      st = 1;
      tot = 1;
    }

#if (__FreeBSD_version >= 500000)
  sec = p->p_runtime.sec;
  usec = p->p_runtime.frac;

  totusec = (quad_t)sec * 1000000 + usec;
#endif

#if (__FreeBSD_version >= 300003) && (__FreeBSD_version < 500000)
  totusec = (u_quad_t) p->p_runtime;
#endif

#if (__FreeBSD_version < 300003)
  sec = p->p_rtime.tv_usec;
  usec = p->p_rtime.tv_usec;
  
  totusec = (quad_t)sec * 1000000 + usec;
#endif
  
  if(totusec < 0)
    {
      fprintf (stderr, "calcru: negative time: %ld usec\n",
	       (long)totusec);
      totusec = 0;
    }
  
  u = totusec;
  st = (u * st) / tot;
  sp->tv_sec = st / 1000000;
  sp->tv_usec = st % 1000000;
  ut = (u * ut) / tot;
  up->tv_sec = ut / 1000000;
  up->tv_usec = ut % 1000000;
  
  if(ip != NULL)
    {
      it = (u * it) / tot;
      ip->tv_sec = it / 1000000;
      ip->tv_usec = it % 1000000;
    }
}

int init_process_info_sysdep(void) {

  register int pagesize;
  struct vmmeter vmm;
  int mib[2];
  size_t len;
  
  struct nlist nlst [] = {
      { "_bufspace"},
      { "_cnt" },
      { 0 }
  };

  if(getuid()!=0) {

    return FALSE;

  }

  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  len = sizeof(num_cpus);
  sysctl(mib, 2, &num_cpus, &len, NULL, 0);

  kvm_handle = kvm_open(NULL, NULL, NULL, O_RDONLY, "monit");
  
  if ( kvm_handle == NULL ) {
    
    return FALSE;
    
  }
  
  /* ----------------------------- INIT MEM -----------------------------*/
  /* Got it from libgtop/sysdep/freebsd/mem.c */

  /* Initialize nlist structure */
  if (kvm_nlist (kvm_handle, nlst) < 0)
  {
    return FALSE;
  }

  /* get the page size with "getpagesize" and calculate pageshift
   * from it */

  pagesize = getpagesize ();
  pageshift = 0;
  while (pagesize > 1) {

    pageshift++;
    pagesize >>= 1;

  }

  /* we only need the amount of log(2)1024 for our conversion */
  pageshift -= LOG1024;

  /* Get the data from kvm_* */
  if (sysctl(mib, 2, &vmm, &len, NULL, 0) < 0) {
    return FALSE;

  }

  mem_kbyte_max= (vmm.v_pageout_free_min +
		  vmm.v_free_count + vmm.v_wire_count +
		  vmm.v_active_count + vmm.v_inactive_count) *
    ( getpagesize() / 1024 ) ;

  return TRUE;

}

int get_process_info_sysdep(ProcInfo_T p) {

  struct kinfo_proc *pinfo;

  /* Only needed for older versions of BSD that use kvm_uread */
  /* struct user *u_addr = (struct user *)USRSTACK; */
  struct pstats pstats;
  struct plimit plimit;
  struct vmspace *vms;
  register struct rusage *rup;
  long stat_utime;
  long stat_stime;
  long stat_cutime;
  long stat_cstime;

  u_int64_t rss_lim;

  int count;

  /* Got it from libgtop */

  pinfo = kvm_getprocs(kvm_handle, KERN_PROC_PID, p->pid, &count);

  if ((pinfo == NULL) || (count < 1)) {

    return FALSE;

  }

  /* ----------------------------- CPU TIMING ----------------------------*/
  /* Got it from libgtop/sysdep/freebsd/proctime.c */
  

  if (kvm_read (kvm_handle, 
#if (__FreeBSD_version > 500000)
		(unsigned long) &pinfo->ki_addr->u_stats,
#else
		(unsigned long) pinfo [0].kp_proc.p_stats,
#endif
		&pstats, sizeof (pstats)) == sizeof (pstats)) {
    
    /* Need to fix for different versions of BSD - I think older ones
       use kvm_uread, and newer use kvm_read */

  /*  if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
      kvm_uread (kvm_handle, &(pinfo [0]).kp_proc,
		 (unsigned long) &u_addr->u_stats,
		 (char *) &pstats, sizeof (pstats)) == sizeof (pstats)) {
  */
    rup = &pstats.p_ru;
#if (__FreeBSD_version > 500000)
    calcru(&pinfo->ki_addr,
#else
    calcru(&(pinfo [0]).kp_proc,
#endif
	   &rup->ru_utime, &rup->ru_stime, NULL);

    stat_utime = tv2sec (pstats.p_ru.ru_utime);
    stat_stime = tv2sec (pstats.p_ru.ru_stime);

    stat_cutime = tv2sec (pstats.p_cru.ru_utime);
    stat_cstime = tv2sec (pstats.p_cru.ru_stime);

  } else {

    return FALSE;

  }

  p->cputime_prev= p->cputime;
  p->cputime= (int)(( stat_utime + stat_stime ) / 1000);

  if( include_children ) {

    p->cputime+= (int)(( stat_cutime + stat_cstime ) / 1000);

  }

  /* first run ? */

  if ( p->time_prev == 0.0 ) {

    p->cputime_prev= p->cputime;

  }

  /* ----------------------------- MEMORY --------------------------------*/
  /* Got it from libgtop/sysdep/freebsd/procmem.c */

  if (kvm_read (kvm_handle,
#if (__FreeBSD_version > 500000)
		(unsigned long) &pinfo->ki_addr->p_limit,
#else
		(unsigned long) pinfo [0].kp_proc.p_limit,
#endif
		(char *) &plimit, sizeof (plimit)) != sizeof (plimit)) {

    return FALSE;

  }

  rss_lim = (u_int64_t)
    (plimit.pl_rlimit [RLIMIT_RSS].rlim_cur);

  vms = &pinfo [0].kp_eproc.e_vm;

  p->mem_kbyte= (u_int64_t) pagetok (vms->vm_rssize);
             /* <<LOG1024 removed, we wanna have kb */

  /* ----------------------------- STATE ---------------------------------*/
  /* Got it from libgtop/sysdep/freebsd/procstate.c */

  if ( pinfo [0].kp_proc.p_stat == SZOMB ) {

    p->status_flag |= PROCESS_ZOMBIE;

  }

  p->mem_percent = (int) ((double) p->mem_kbyte * 1024.0 / mem_kbyte_max);

  return TRUE;

}

/**
 * Read all processes of the proc files system to initilize
 * the process tree (sysdep version... but should work for
 * all procfs based unices)
 * @param reference  reference of ProcessTree
 * @return treesize>0 if succeeded otherwise =0.
 */
int initprocesstree_sysdep(ProcessTree_T ** reference) {

  struct kinfo_proc *pinfo;

  /* Only needed for older versions of BSD that use kvm_uread */
  /* struct user *u_addr = (struct user *)USRSTACK; */
  struct plimit plimit;
  struct vmspace *vms;

  u_int64_t rss_lim;
  int i=0;
  int treesize;

  ProcessTree_T * pt;


  /* Got it from libgtop */

  pinfo = kvm_getprocs(kvm_handle, KERN_PROC_ALL, 0, &treesize);

  if ((pinfo == NULL) || (treesize < 1)) {

    return 0;

  }

  /* Allocate the tree */
  
  pt = xcalloc(sizeof(ProcessTree_T), treesize);

  /* Insert data from /proc directory */

  for(i=0;i<treesize;i++) {

    pt[i].pid   = pinfo[i].kp_proc.p_pid;
    pt[i].ppid  = pinfo[i].kp_eproc.e_ppid;

    if (kvm_read (kvm_handle,
		  (unsigned long) pinfo[i].kp_proc.p_limit,
		  (char *) &plimit, sizeof (plimit)) != sizeof (plimit)) {
      
      return FALSE;
      
    }

    rss_lim = (u_int64_t)
      (plimit.pl_rlimit [RLIMIT_RSS].rlim_cur);
    
    vms = &pinfo [i].kp_eproc.e_vm;
    
    pt[i].mem_kbyte= (u_int64_t) pagetok (vms->vm_rssize);
    /* <<LOG1024 removed, we wanna have kb */

  }

  * reference = pt;

  return treesize;

}

/**
 * This routine returns 'nelem' double precision floats containing
 * the load averages in 'loadv'; at most 3 values will be returned.
 * @param loadv destination of the load averages
 * @param nelem number of averages
 * @return: 0 if successful, -1 if failed (and all load averages are 0).
 */
int getloadavg_sysdep (double *loadv, int nelem) {

  return getloadavg(loadv, nelem);

}
