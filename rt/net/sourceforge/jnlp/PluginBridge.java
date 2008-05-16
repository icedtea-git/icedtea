/*
 * Copyright 2007 Red Hat, Inc.
 * This file is part of IcedTea, http://icedtea.classpath.org
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
 */

package net.sourceforge.jnlp;

import java.net.URL;
import java.net.MalformedURLException;
import java.util.Hashtable;
import java.util.Locale;
import java.util.List;
import java.util.ArrayList;


public class PluginBridge extends JNLPFile
{
    Version specVersion = new Version("1.0");
    Version fileVersion = new Version("1.1");

    String name;
    String[] jars;
    Hashtable atts;

    public PluginBridge(URL codebase, URL documentBase, String jar, String main,
                        int width, int height, Hashtable atts)
    throws Exception
    {
        this.codeBase = codebase;
        this.sourceLocation = documentBase;

        if (jar != null) {
            System.err.println("Jar string: " + jar);
            this.jars = jar.split(",");
            System.err.println("jars length: " + jars.length);
        }
        this.atts = atts;

        name = (String) atts.get("name");
        if (name == null)
            name = "Applet";
        else
            name = name + " applet";

        if (main.endsWith(".class"))
            main = main.substring(0, main.length() - 6);

        launchType = new AppletDesc(name, main, documentBase, width,
                                    height, atts);

        if (main.endsWith(".class")) //single class file only
            security = new SecurityDesc(this, SecurityDesc.SANDBOX_PERMISSIONS,
                                        codebase.getHost());
        else
            security = null;
    }

    public String getTitle()
    {
        return name;
    }

    public InformationDesc getInformation(final Locale locale)
    {
        return new InformationDesc(this, new Locale[] {locale}) {
            protected List getItems(Object key)
            {
                // Should we populate this list with applet attribute tags?
                List result = new ArrayList();
                return result;
            }
        };
    }

    public ResourcesDesc getResources(final Locale locale, final String os,
                                      final String arch)
    {
        return new ResourcesDesc(this, new Locale[] {locale}, new String[] {os},
        new String[] {arch}) {
            public List getResources(Class launchType)
            {
                List result = new ArrayList();
                result.addAll(sharedResources.getResources(launchType));

                // Need to add the JAR manually...
                //should this be done to sharedResources on init?
                try
                {
                    if (launchType.equals(JARDesc.class) && jars != null)
                    {
                        for (int i = 0; i < jars.length; i++)
                            result.add(new JARDesc(new URL(codeBase, jars[i]),
                                                   null, null, false, true, false));
                    }
                }
                catch (MalformedURLException ex)
                    { }
                return result;
            }

            public JARDesc[] getJARs() {
                List resources = getResources(JARDesc.class);
                ArrayList<JARDesc> jars = new ArrayList<JARDesc>();

                //Only get the JARDescs
                for (int i = 0; i < resources.size(); i++) {
                    Object resource = resources.get(i);
                    if (resource instanceof JARDesc)
                        jars.add((JARDesc) resource);
                }

                Object[] objectArray = jars.toArray();
                JARDesc[] jarArray = new JARDesc[objectArray.length];

                for (int i = 0; i < objectArray.length; i++)
                    jarArray[i] = (JARDesc) objectArray[i];

                return jarArray;
            }

            public void addResource(Object resource)
            {
                // todo: honor the current locale, os, arch values
                sharedResources.addResource(resource);
            }

        };
    }

    public boolean isApplet() {
        return true;
    }
    public boolean isApplication() {
        return false;
    }
    public boolean isComponent() {
        return false;
    }
    public boolean isInstaller() {
        return false;
    }
}
