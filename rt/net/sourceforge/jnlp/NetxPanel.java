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

import net.sourceforge.jnlp.runtime.AppletInstance;
import net.sourceforge.jnlp.runtime.JNLPRuntime;

import java.net.URL;
import java.util.Hashtable;

import sun.applet.AppletViewerPanel;

/**
 * This panel calls into netx to run an applet, and pipes the display
 * into a panel from gcjwebplugin.
 *
 * @author      Francis Kung <fkung@redhat.com>
 */
public class NetxPanel extends AppletViewerPanel
{
    private PluginBridge bridge = null;

    public NetxPanel(URL documentURL, Hashtable atts)
    {
        super(documentURL, atts);
    }

    //Overriding to use Netx classloader. You might need to relax visibility
    //in sun.applet.AppletPanel for runLoader().
    protected void runLoader() {

    	try {
    		bridge = new PluginBridge(baseURL, 
    				getDocumentBase(),
    				getJarFiles(), 
    				getCode(),
    				getWidth(), 
    				getHeight(), 
    				atts);
    		
    		//The custom NetX Policy and SecurityManager are set here.
    		if (!JNLPRuntime.isInitialized()) {
    			System.out.println("initializing JNLPRuntime...");
    			JNLPRuntime.initialize();
    		} else {
    			System.out.println("JNLPRuntime already initialized");
    		}

    		doInit = true;
    		dispatchAppletEvent(APPLET_LOADING, null);
    		status = APPLET_LOAD;

    		Launcher l = new Launcher();
    		AppletInstance appInst = null;
                try {
                    appInst = (AppletInstance) l.launch(bridge, this);
                } catch (LaunchException e) {
                    // Assume user has indicated he does not trust the
                    // applet.
                    System.exit(0);
                }
    		applet = appInst.getApplet();
    		
    		//On the other hand, if you create an applet this way, it'll work
    		//fine. Note that you might to open visibility in sun.applet.AppletPanel
    		//for this to work (the loader field, and getClassLoader).
    		//loader = getClassLoader(getCodeBase(), getClassLoaderCacheKey());
    		//applet = createApplet(loader);
    		
    		// This shows that when using NetX's JNLPClassLoader, keyboard input
    		// won't make it to the applet, whereas using sun.applet.AppletClassLoader
    		// works just fine.
    		
    		dispatchAppletEvent(APPLET_LOADING_COMPLETED, null);

    		if (applet != null)
    		{
    			// Stick it in the frame
    			applet.setStub(this);
    			applet.setVisible(false);
    			add("Center", applet);
    			showAppletStatus("loaded");
    			validate();
    		}
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    }
    
    // Reminder: Relax visibility in sun.applet.AppletPanel
    protected synchronized void createAppletThread() {
    	handler = new Thread(this);
    	handler.start();
    }

}
