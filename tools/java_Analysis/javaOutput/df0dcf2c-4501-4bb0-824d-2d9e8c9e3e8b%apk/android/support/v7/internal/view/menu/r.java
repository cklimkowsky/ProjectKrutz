package android.support.v7.internal.view.menu;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.support.v4.a.a.b;
import android.support.v4.view.ac;
import android.support.v4.view.l;
import android.util.Log;
import android.view.ActionProvider;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuItem;
import android.view.MenuItem.OnActionExpandListener;
import android.view.MenuItem.OnMenuItemClickListener;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewDebug.CapturedViewProperty;

public final class r
  implements b
{
  private static String w;
  private static String x;
  private static String y;
  private static String z;
  private final int a;
  private final int b;
  private final int c;
  private final int d;
  private CharSequence e;
  private CharSequence f;
  private Intent g;
  private char h;
  private char i;
  private Drawable j;
  private int k = 0;
  private n l;
  private ag m;
  private Runnable n;
  private MenuItem.OnMenuItemClickListener o;
  private int p = 16;
  private int q = 0;
  private View r;
  private l s;
  private ac t;
  private boolean u = false;
  private ContextMenu.ContextMenuInfo v;

  r(n paramn, int paramInt1, int paramInt2, int paramInt3, int paramInt4, CharSequence paramCharSequence, int paramInt5)
  {
    this.l = paramn;
    this.a = paramInt2;
    this.b = paramInt1;
    this.c = paramInt3;
    this.d = paramInt4;
    this.e = paramCharSequence;
    this.q = paramInt5;
  }

  private b a(View paramView)
  {
    this.r = paramView;
    this.s = null;
    if ((paramView != null) && (paramView.getId() == -1) && (this.a > 0))
      paramView.setId(this.a);
    this.l.i();
    return this;
  }

  public final b a(l paraml)
  {
    if (this.s == paraml);
    do
    {
      return this;
      this.r = null;
      if (this.s != null)
        this.s.a(null);
      this.s = paraml;
      this.l.b(true);
    }
    while (paraml == null);
    paraml.a(new r.1(this));
    return this;
  }

  final CharSequence a(ad paramad)
  {
    if ((paramad != null) && (paramad.b()))
      return getTitleCondensed();
    return getTitle();
  }

  final void a(ag paramag)
  {
    this.m = paramag;
    paramag.setHeaderTitle(getTitle());
  }

  final void a(ContextMenu.ContextMenuInfo paramContextMenuInfo)
  {
    this.v = paramContextMenuInfo;
  }

  public final void a(boolean paramBoolean)
  {
    int i1 = 0xFFFFFFFB & this.p;
    if (paramBoolean);
    for (int i2 = 4; ; i2 = 0)
    {
      this.p = (i2 | i1);
      return;
    }
  }

  public final boolean a()
  {
    if ((this.o != null) && (this.o.onMenuItemClick(this)));
    while (this.l.a(this.l.n(), this))
      return true;
    if (this.n != null)
    {
      this.n.run();
      return true;
    }
    if (this.g != null)
      try
      {
        this.l.d().startActivity(this.g);
        return true;
      }
      catch (ActivityNotFoundException localActivityNotFoundException)
      {
        Log.e("MenuItemImpl", "Can't find activity to handle intent; ignoring", localActivityNotFoundException);
      }
    if (this.s != null);
    return false;
  }

  public final int b()
  {
    return this.d;
  }

  final void b(boolean paramBoolean)
  {
    int i1 = this.p;
    int i2 = 0xFFFFFFFD & this.p;
    if (paramBoolean);
    for (int i3 = 2; ; i3 = 0)
    {
      this.p = (i3 | i2);
      if (i1 != this.p)
        this.l.b(false);
      return;
    }
  }

  final char c()
  {
    return this.i;
  }

  final boolean c(boolean paramBoolean)
  {
    int i1 = this.p;
    int i2 = 0xFFFFFFF7 & this.p;
    if (paramBoolean);
    for (int i3 = 0; ; i3 = 8)
    {
      this.p = (i3 | i2);
      int i4 = this.p;
      boolean bool = false;
      if (i1 != i4)
        bool = true;
      return bool;
    }
  }

  public final boolean collapseActionView()
  {
    if ((0x8 & this.q) == 0);
    do
    {
      return false;
      if (this.r == null)
        return true;
    }
    while ((this.t != null) && (!this.t.b()));
    return this.l.b(this);
  }

  final String d()
  {
    char c1 = this.i;
    if (c1 == 0)
      return "";
    StringBuilder localStringBuilder = new StringBuilder(w);
    switch (c1)
    {
    default:
      localStringBuilder.append(c1);
    case '\n':
    case '\b':
    case ' ':
    }
    while (true)
    {
      return localStringBuilder.toString();
      localStringBuilder.append(x);
      continue;
      localStringBuilder.append(y);
      continue;
      localStringBuilder.append(z);
    }
  }

  public final void d(boolean paramBoolean)
  {
    if (paramBoolean)
    {
      this.p = (0x20 | this.p);
      return;
    }
    this.p = (0xFFFFFFDF & this.p);
  }

  public final void e(boolean paramBoolean)
  {
    this.u = paramBoolean;
    this.l.b(false);
  }

  final boolean e()
  {
    return (this.l.b()) && (this.i != 0);
  }

  public final boolean expandActionView()
  {
    if (((0x8 & this.q) == 0) || (this.r == null));
    while ((this.t != null) && (!this.t.a()))
      return false;
    return this.l.a(this);
  }

  public final boolean f()
  {
    return (0x4 & this.p) != 0;
  }

  public final boolean g()
  {
    return this.l.o();
  }

  public final ActionProvider getActionProvider()
  {
    throw new UnsupportedOperationException("Implementation should use getSupportActionProvider!");
  }

  public final View getActionView()
  {
    if (this.r != null)
      return this.r;
    if (this.s != null)
    {
      this.r = this.s.b();
      return this.r;
    }
    return null;
  }

  public final char getAlphabeticShortcut()
  {
    return this.i;
  }

  public final int getGroupId()
  {
    return this.b;
  }

  public final Drawable getIcon()
  {
    if (this.j != null)
      return this.j;
    if (this.k != 0)
    {
      Drawable localDrawable = this.l.c().getDrawable(this.k);
      this.k = 0;
      this.j = localDrawable;
      return localDrawable;
    }
    return null;
  }

  public final Intent getIntent()
  {
    return this.g;
  }

  @ViewDebug.CapturedViewProperty
  public final int getItemId()
  {
    return this.a;
  }

  public final ContextMenu.ContextMenuInfo getMenuInfo()
  {
    return this.v;
  }

  public final char getNumericShortcut()
  {
    return this.h;
  }

  public final int getOrder()
  {
    return this.c;
  }

  public final SubMenu getSubMenu()
  {
    return this.m;
  }

  @ViewDebug.CapturedViewProperty
  public final CharSequence getTitle()
  {
    return this.e;
  }

  public final CharSequence getTitleCondensed()
  {
    if (this.f != null)
      return this.f;
    return this.e;
  }

  public final boolean h()
  {
    return (0x20 & this.p) == 32;
  }

  public final boolean hasSubMenu()
  {
    return this.m != null;
  }

  public final boolean i()
  {
    return (0x1 & this.q) == 1;
  }

  public final boolean isActionViewExpanded()
  {
    return this.u;
  }

  public final boolean isCheckable()
  {
    return (0x1 & this.p) == 1;
  }

  public final boolean isChecked()
  {
    return (0x2 & this.p) == 2;
  }

  public final boolean isEnabled()
  {
    return (0x10 & this.p) != 0;
  }

  public final boolean isVisible()
  {
    return (this.s == null) || ((0x8 & this.p) == 0);
  }

  public final boolean j()
  {
    return (0x2 & this.q) == 2;
  }

  public final boolean k()
  {
    return (0x4 & this.q) == 4;
  }

  public final l l()
  {
    return this.s;
  }

  public final boolean m()
  {
    return ((0x8 & this.q) != 0) && (this.r != null);
  }

  public final MenuItem setActionProvider(ActionProvider paramActionProvider)
  {
    throw new UnsupportedOperationException("Implementation should use setSupportActionProvider!");
  }

  public final MenuItem setAlphabeticShortcut(char paramChar)
  {
    if (this.i == paramChar)
      return this;
    this.i = Character.toLowerCase(paramChar);
    this.l.b(false);
    return this;
  }

  public final MenuItem setCheckable(boolean paramBoolean)
  {
    int i1 = this.p;
    int i2 = 0xFFFFFFFE & this.p;
    if (paramBoolean);
    for (int i3 = 1; ; i3 = 0)
    {
      this.p = (i3 | i2);
      if (i1 != this.p)
        this.l.b(false);
      return this;
    }
  }

  public final MenuItem setChecked(boolean paramBoolean)
  {
    if ((0x4 & this.p) != 0)
    {
      this.l.a(this);
      return this;
    }
    b(paramBoolean);
    return this;
  }

  public final MenuItem setEnabled(boolean paramBoolean)
  {
    if (paramBoolean);
    for (this.p = (0x10 | this.p); ; this.p = (0xFFFFFFEF & this.p))
    {
      this.l.b(false);
      return this;
    }
  }

  public final MenuItem setIcon(int paramInt)
  {
    this.j = null;
    this.k = paramInt;
    this.l.b(false);
    return this;
  }

  public final MenuItem setIcon(Drawable paramDrawable)
  {
    this.k = 0;
    this.j = paramDrawable;
    this.l.b(false);
    return this;
  }

  public final MenuItem setIntent(Intent paramIntent)
  {
    this.g = paramIntent;
    return this;
  }

  public final MenuItem setNumericShortcut(char paramChar)
  {
    if (this.h == paramChar)
      return this;
    this.h = paramChar;
    this.l.b(false);
    return this;
  }

  public final MenuItem setOnActionExpandListener(MenuItem.OnActionExpandListener paramOnActionExpandListener)
  {
    throw new UnsupportedOperationException("Implementation should use setSupportOnActionExpandListener!");
  }

  public final MenuItem setOnMenuItemClickListener(MenuItem.OnMenuItemClickListener paramOnMenuItemClickListener)
  {
    this.o = paramOnMenuItemClickListener;
    return this;
  }

  public final MenuItem setShortcut(char paramChar1, char paramChar2)
  {
    this.h = paramChar1;
    this.i = Character.toLowerCase(paramChar2);
    this.l.b(false);
    return this;
  }

  public final void setShowAsAction(int paramInt)
  {
    switch (paramInt & 0x3)
    {
    default:
      throw new IllegalArgumentException("SHOW_AS_ACTION_ALWAYS, SHOW_AS_ACTION_IF_ROOM, and SHOW_AS_ACTION_NEVER are mutually exclusive.");
    case 0:
    case 1:
    case 2:
    }
    this.q = paramInt;
    this.l.i();
  }

  public final MenuItem setTitle(int paramInt)
  {
    return setTitle(this.l.d().getString(paramInt));
  }

  public final MenuItem setTitle(CharSequence paramCharSequence)
  {
    this.e = paramCharSequence;
    this.l.b(false);
    if (this.m != null)
      this.m.setHeaderTitle(paramCharSequence);
    return this;
  }

  public final MenuItem setTitleCondensed(CharSequence paramCharSequence)
  {
    this.f = paramCharSequence;
    if (paramCharSequence == null);
    this.l.b(false);
    return this;
  }

  public final MenuItem setVisible(boolean paramBoolean)
  {
    if (c(paramBoolean))
      this.l.h();
    return this;
  }

  public final String toString()
  {
    return this.e.toString();
  }
}

/* Location:
 * Qualified Name:     android.support.v7.internal.view.menu.r
 * Java Class Version: 6 (50.0)
 * JD-Core Version:    0.6.1-SNAPSHOT
 */