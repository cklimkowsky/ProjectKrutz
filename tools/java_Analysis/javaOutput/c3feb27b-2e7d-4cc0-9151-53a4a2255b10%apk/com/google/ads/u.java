package com.google.ads;

import android.content.Context;
import android.text.TextUtils;
import android.webkit.WebView;
import com.google.ads.internal.d;
import com.google.ads.util.b;
import com.google.ads.util.i.b;
import java.util.HashMap;

public class u
  implements o
{
  public u()
  {
  }

  public void a(d paramd, HashMap<String, String> paramHashMap, WebView paramWebView)
  {
    String str = (String)paramHashMap.get("u");
    if (TextUtils.isEmpty(str))
    {
      b.e("Could not get URL from track gmsg.");
      return;
    }
    new Thread(new ae(str, (Context)paramd.i().f.a())).start();
  }
}

/* Location:
 * Qualified Name:     com.google.ads.u
 * Java Class Version: 6 (50.0)
 * JD-Core Version:    0.6.1-SNAPSHOT
 */