/* ScriptableObject.java -- stub file.
   Copyright (C) 2007 Red Hat, Inc.

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

package com.sun.script.javascript;

import javax.script.Invocable;

public class ScriptableObject {

	public static final String DONTENUM = null;
    
    protected Invocable engine;

	public Scriptable getParentScope() {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}
	public static void defineProperty(Scriptable scope, String string, JSAdapter obj, String dontenum2) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static Scriptable getTopLevelScope(Scriptable scope) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Scriptable getFunctionPrototype(Scriptable scope) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Object getProperty(Scriptable adaptee, String name) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static void putProperty(RhinoTopLevel topLevel, String string, JavaAdapter obj) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static Scriptable getObjectPrototype(Scriptable thisObj) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}
    
    protected void setParentScope(Scriptable scope) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	protected void setPrototype(Scriptable functionPrototype) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static void init(Context cx, RhinoTopLevel level, boolean b) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

}
