/* SharedSecrets.java
   Copyright (C) 2009 Red Hat, Inc.

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.

IcedTea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version.
 */

/** Based on sun.misc.SharedSecrets */
package org.classpath.icedtea.misc;

import sun.misc.Unsafe;

/** A repository of "shared secrets", which are a mechanism for
    calling implementation-private methods in another package without
    using reflection. A package-private class implements a public
    interface and provides the ability to call package-private methods
    within that package; the object implementing that interface is
    provided through a third package to which access is restricted.
    This framework avoids the primary disadvantage of using reflection
    for this purpose, namely the loss of compile-time checking. */

public class SharedSecrets 
{
  private static final Unsafe unsafe = Unsafe.getUnsafe();
  private static JavaIODeleteOnExitAccess javaIODeleteOnExitAccess;
  private static JavaUtilConcurrentThreadPoolExecutorAccess javaUtilConcurrentThreadPoolExecutorAccess;
  private static JavaNetGetIndexAccess javaNetGetIndexAccess;

  public static void setJavaIODeleteOnExitAccess(JavaIODeleteOnExitAccess jida) 
  {
    javaIODeleteOnExitAccess = jida;
  }
  
  public static JavaIODeleteOnExitAccess getJavaIODeleteOnExitAccess() 
  {
    if (javaIODeleteOnExitAccess == null) 
      {
	unsafe.ensureClassInitialized(java.io.File.class);
      }
    return javaIODeleteOnExitAccess;
  }

  public static void setJavaUtilConcurrentThreadPoolExecutorAccess(JavaUtilConcurrentThreadPoolExecutorAccess juctpea) 
  {
    javaUtilConcurrentThreadPoolExecutorAccess = juctpea;
  }
  
  public static JavaUtilConcurrentThreadPoolExecutorAccess getJavaUtilConcurrentThreadPoolExecutorAccess() 
  {
    if (javaUtilConcurrentThreadPoolExecutorAccess == null) 
      {
	unsafe.ensureClassInitialized(java.util.concurrent.ThreadPoolExecutor.class);
      }
    return javaUtilConcurrentThreadPoolExecutorAccess;
  }

  public static void setJavaNetGetIndexAccess(JavaNetGetIndexAccess jngia) 
  {
    javaNetGetIndexAccess = jngia;
  }
  
  public static JavaNetGetIndexAccess getJavaNetGetIndexAccess() 
  {
    if (javaNetGetIndexAccess == null) 
      {
	unsafe.ensureClassInitialized(java.net.NetworkInterface.class);
      }
    return javaNetGetIndexAccess;
  }


}
