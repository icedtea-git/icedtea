/* Context.java -- stub file.
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

import java.io.IOException;
import java.io.Reader;

import javax.script.Bindings;

public class Context {

	protected static final int FEATURE_E4X = 0;
	public static Object[] emptyArgs;

	public Scriptable newObject(Scriptable scope) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static boolean toBoolean(Object res) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Scriptable toObject(Object object, Scriptable topLevel) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Object toString(Object tmp) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Context getCurrentContext() {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static RhinoException reportRuntimeError(String string) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public void evaluateString(RhinoTopLevel level, String builtinVariables, String string, int i, Object object) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static Object javaToJS(Bindings bind, Scriptable topLevelScope) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public Object getUndefinedValue() {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public void setClassShutter(ClassShutter instance) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public void setWrapFactory(WrapFactory instance) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static void exit() {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public static Object jsToJava(Object res, Class desiredType) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public Object evaluateReader(Scriptable scope, Reader reader, String filename, int i, Object object) throws IOException {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public void evaluateString(Scriptable newScope, String printSource, String string, int i, Object object) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub
		
	}

	public Script compileReader(Scriptable scope, Reader script, String fileName, int i, Object object) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Context enter() {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Object javaToJS(Object object, RhinoTopLevel topLevel) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public Object toObject(Object thiz, RhinoTopLevel topLevel) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

	public static Object javaToJS(Object value, ExternalScriptable scriptable) {
		throw new RuntimeException("Not implemented.");
		// TODO Auto-generated method stub

	}

}
