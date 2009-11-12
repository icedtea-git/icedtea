/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */
package com.sun.javatest.tool;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.lang.NoSuchMethodException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.Set;

import com.sun.javatest.util.I18NResourceBundle;

class ManagerLoader
{
    ManagerLoader(Class managerClass, PrintStream log) {
        setManagerClass(managerClass);
        setLog(log);
    }

    void setManagerClass(Class managerClass) {
        this.managerClass = managerClass;
    }

    void setManagerConstructorArgs(Class[] argTypes, Object[] args) {
        constrArgTypes = argTypes;
        constrArgs = args;
    }

    void setLog(PrintStream log) {
        this.log = log;
    }

    Set loadManagers(String resourceName)
        throws IOException
    {
        ClassLoader loader = ManagerLoader.class.getClassLoader();
        Enumeration e = loader.getResources(resourceName);
        Set mgrs = new HashSet();
        while (e.hasMoreElements()) {
            URL entry = (URL)(e.nextElement());
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(entry.openStream()));
                String line;
                while ((line = in.readLine()) != null) {
                    line = line.trim();
                    if (line.length() == 0 || line.startsWith("#"))
                        continue;

                    try {
                        Class c = Class.forName(line);
                        Object mgr = newInstance(c);
                        if (managerClass.isInstance(mgr))
                            mgrs.add(mgr);
                        else {
                            if (log != null)
                                writeI18N("ml.notSubtype", new Object[] { line, managerClass.getName(), entry } );
                        }
                    }
                    catch (ClassNotFoundException ex) {
                        if (log != null)
                            writeI18N("ml.cantFindClass", new Object[] { line, entry } );
                    }
                    catch (IllegalAccessException ex) {
                        if (log != null)
                            writeI18N("ml.cantAccessClass", new Object[] { line, entry } );
                    }
                    catch (InstantiationException ex) {
                        if (log != null)
                            writeI18N("ml.cantCreateClass", new Object[] { line, entry } );
                    }
                    catch (NoSuchMethodException ex) {
                        if (log != null)
                            writeI18N("ml.cantFindConstr", new Object[] { line, entry } );
                    }
                }
                in.close();
            }
            catch (IOException ex) {
                if (log != null)
                    writeI18N("ml.cantRead", new Object[] { entry, ex });
            }
        }
        return mgrs;
    }

    private Object newInstance(Class c)
        throws IllegalAccessException, InstantiationException, NoSuchMethodException
    {
        if (constrArgTypes == null || constrArgTypes.length == 0)
            return c.newInstance();

        try {
            Constructor constr = c.getConstructor(constrArgTypes);
            return constr.newInstance(constrArgs);
        }
        catch (InvocationTargetException e) {
            Throwable t = e.getTargetException();
            if (t instanceof RuntimeException)
                throw ((RuntimeException) t);
            else if (t instanceof Error)
                throw ((Error) t);
            else
                throw new Error(e);
        }
    }

    private void writeI18N(String key, Object[] args) {
        log.println(i18n.getString(key, args));
    }

    private Class managerClass;
    private Constructor constr;
    private Class[] constrArgTypes;
    private Object[] constrArgs;
    private PrintStream log;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(ManagerLoader.class);
}
