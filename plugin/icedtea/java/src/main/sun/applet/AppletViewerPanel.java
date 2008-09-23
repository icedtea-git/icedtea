/*
 * Copyright 1995-2005 Sun Microsystems, Inc.  All Rights Reserved.
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

package sun.applet;

import java.applet.AppletContext;
import java.awt.Dimension;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Hashtable;


/**
 * Sample applet panel class. The panel manages and manipulates the
 * applet as it is being loaded. It forks a seperate thread in a new
 * thread group to call the applet's init(), start(), stop(), and
 * destroy() methods.
 *
 * @author      Arthur van Hoff
 */
public class AppletViewerPanel extends AppletPanel {

    /* Are we debugging? */
    protected static boolean debug = true;

    /**
     * The document url.
     */
    protected URL documentURL;

    /**
     * The base url.
     */
    protected URL baseURL;

    /**
     * The attributes of the applet.
     */
    protected Hashtable atts;

    /*
     * JDK 1.1 serialVersionUID
     */
    private static final long serialVersionUID = 8890989370785545619L;

    private Dimension windowSizeFactor = new Dimension(750, 350);
    
    /**
     * Construct an applet viewer and start the applet.
     */
    protected AppletViewerPanel(URL documentURL, Hashtable atts) {
        this.documentURL = documentURL;
        this.atts = atts;

        String att = getParameter("codebase");
        if (att != null) {
            if (!att.endsWith("/")) {
                att += "/";
            }
            try {
                baseURL = new URL(documentURL, att);
            } catch (MalformedURLException e) {
            }
        }
        if (baseURL == null) {
            String file = documentURL.getFile();
            int i = file.lastIndexOf('/');
            if (i >= 0 && i < file.length() - 1) {
                try {
                    baseURL = new URL(documentURL, file.substring(0, i + 1));
                } catch (MalformedURLException e) {
                }
            }
        }

        // when all is said & done, baseURL shouldn't be null
        if (baseURL == null)
                baseURL = documentURL;


    }


    /**
     * Get an applet parameter.
     */
    public String getParameter(String name) {
        return (String)atts.get(name.toLowerCase());
    }

    /**
     * Get the document url.
     */
    public URL getDocumentBase() {
        return documentURL;

    }

    /**
     * Get the base url.
     */
    public URL getCodeBase() {
        return baseURL;
    }

    /**
     * Set applet size (as proportion of window size) if needed
     */
    public synchronized void setAppletSizeIfNeeded(int width, int height) {

    	Dimension newD = new Dimension(getWidth(), getHeight());

    	String h = getParameter("height");
    	String w = getParameter("width");

    	if (width != -1 && w != null && w.endsWith("%")) {
    		newD.width = (Integer.valueOf(w.substring(0, w.length() - 1)).intValue()/100)*width;
    	}

    	if (height != -1 && h != null && h.endsWith("%")) {
    		newD.height = (Integer.valueOf(h.substring(0, h.length() - 1)).intValue()/100)*height;
    	}
    	
    	synchronized(windowSizeFactor) {
    		windowSizeFactor = newD;
    	}
    }

    /**
     * Get the width.
     */
    public int getWidth() {
        String w = getParameter("width");
        if (w != null) {
        	try {
        		return Integer.valueOf(w).intValue();
        	} catch (NumberFormatException nfe) {
        		synchronized(windowSizeFactor) {
        			System.err.println("getWidth() returning " + windowSizeFactor.width);
        			return windowSizeFactor.width;
        		}
        	}
        }
        return 0;
    }


    /**
     * Get the height.
     */
    public int getHeight() {
        String h = getParameter("height");
        if (h != null) {
        	try {
        		return Integer.valueOf(h).intValue();
        	} catch (NumberFormatException nfe) {
        		synchronized(windowSizeFactor) {
        			System.err.println("getHeight() returning " + windowSizeFactor.height);
        			return windowSizeFactor.height;
        		}
        	}
        }
        return 0;
    }

    /**
     * Get initial_focus
     */
    public boolean hasInitialFocus()
    {

        // 6234219: Do not set initial focus on an applet
        // during startup if applet is targeted for
        // JDK 1.1/1.2. [stanley.ho]
        if (isJDK11Applet() || isJDK12Applet())
            return false;

        String initialFocus = getParameter("initial_focus");

        if (initialFocus != null)
        {
            if (initialFocus.toLowerCase().equals("false"))
                return false;
        }

        return true;
    }

    /**
     * Get the code parameter
     */
    public String getCode() {
        return getParameter("code");
    }


    /**
     * Return the list of jar files if specified.
     * Otherwise return null.
     */
    public String getJarFiles() {
        return getParameter("archive");
    }

    /**
     * Return the value of the object param
     */
    public String getSerializedObject() {
        return getParameter("object");// another name?
    }


    /**
     * Get the applet context. For now this is
     * also implemented by the AppletPanel class.
     */
    public AppletContext getAppletContext() {
        return (AppletContext)getParent();
    }

    protected static void debug(String s) {
        if(debug)
            System.err.println("AppletViewerPanel:::" + s);
    }

    protected static void debug(String s, Throwable t) {
        if(debug) {
            t.printStackTrace();
            debug(s);
        }
    }
}
