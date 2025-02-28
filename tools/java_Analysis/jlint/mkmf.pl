#!/usr/bin/perl
###############################################################################
# mkmf.pl
# Makefile generator v0.5 written by Cyrille Artho
# changes:
# 0.1: initial version.
# 0.2: better output (each file on one line with \ at the end) and update mode
# 0.3: removed stupid bug that produces broken Makefiles
# 0.4: removed another stupid bug that omitted the .c files in dependencies...
# 0.5: removed unused @hfiles list
###############################################################################
# generates a simple default Makefile
# a) takes Makefile.top and (optionally) Makefile.bottom and generates
#    dependencies for .o and .h files in between
# b) take existing Makefile (generated by mkmf) and updates automatically
#    generated part.
###############################################################################
# Usage: mkmf.pl target [regexps to ignore]
# assumptions: Target = built from all .o files in current directory.
# .o files are built with gcc -c. .c* files are scanned for .h[h] files.
# dependency for each x.o file: x.c[cp]* x.hh? [any other header files]
# ignore regexps are used for filenames, which are not included if they match.
###############################################################################
# TODO:
# a) better comment line (include date, cmd line args used for generating
#    Makefile etc.)
# b) this tool is not supposed to support bigger projects, so if you need
#    special features, use autoconf :-)
###############################################################################

use File::Find;

my $target = shift;
my @cfiles; # C files
#my @hfiles; # header files
my $ignore; # regexps to ignore
my $is_cpp; # C or C++
my @input;
my $found = 0;

die "Usage: mkmf <target> [ignore]\n" unless $target;

while (my $tmp = shift) { $ignore .= "$tmp|"; }
$ignore =~ s/\|$//;
if ($ignore eq "") { $ignore = "\$^"; } # pattern that never matches

if (@input = <>) {
  while (my $line = shift @input) {
    if ($line !~ /^\# --> automatically generated dependencies/) {
      print $line;
    } else {
      $found = 1;
      last;
    }
  }
  if (! $found) {
    die "This input file has not been preprocessed by mkmf before!\n";
  }
   while (my $line = shift @input) {
    if ($line =~ /^\# --> end of automatically generated dependencies/) {
      $found = 1;
      last;
    }
  }
  if (! $found) {
    die "This input file has not been preprocessed by mkmf before!\n";
  } 
} else {
  open (FILE, "Makefile.top") or die "Cannot open Makefile.top: $!\n";
  print <FILE>;
  close FILE;
}
print "# --> automatically generated dependencies follow; do not remove this line.\n";

getFiles();

# target

print "$target:";
foreach (@cfiles) { /\.c[cpx+]*$/; print " \\\n\t$`.o"; } # print name + .o
print "\n\t";

($is_cpp) = grep /$target\.c[cpx+]*$/, @cfiles;
$is_cpp =~ s/.*\.c//; # remove all but suffix (also removes first letter of it)
if (is_cpp) { print '$(CPP)'; } else { print '$(CC)'; }
print ' -o ', "$target";
foreach (@cfiles) { /\.c[cpx+]*$/; print " $`.o"; } # print name + .o
print ' $(LFLAGS)';
print "\n\n"; # target line finished

printDeps();

print "# --> end of automatically generated dependencies; do not remove this line.\n";

open (FILE, "Makefile.bottom") or exit; # no error here
print <FILE>;
close FILE;

### end of main ###

sub getFiles { # look for files with right suffixes
  find (\&checkFile, '.');
}

sub checkFile {
  return if (!(-f));
  return unless (/\.[ch][chpx+]*$/);
  return if /$ignore/io;
  if (/\.c[cpx+]*$/) {
    push @cfiles, $_;
  } else {
#    push @hfiles, $_;
  }
}

sub printDeps { # check dependencies of C/C++ and header files
  foreach my $file (@cfiles) {
    $file =~/\.c[cpx+]*$/; print "$`.o: $file"; # print name + .o
    checkHeaders($file, {}); # print dependencies
    print "\n\t";
    if (is_cpp) { print '$(CPP)'; } else { print '$(CC)'; }
    print " \$(CFLAGS) $file\n\n";
  }
}

sub checkHeaders { # parse file for #include "..."
  my $filename = shift;
  my $deps = shift;
  my @newfiles;
  open (FILE, $filename) or die "Could not open $filename: $!\n";
  while (<FILE>) {
    if (s/^\#include\s*\"(.*)\"$/$1/) {
      if (! defined $$deps{$1}) {
        print " \\\n\t$1";
        $$deps{$1} = 1;
        push @newfiles, $1;
      }
    }
  }
  close FILE;
  foreach $newfile (@newfiles) {
    checkHeaders($newfile, $deps);
  }
}
