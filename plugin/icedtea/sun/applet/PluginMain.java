/*
 * Copyright 1999-2006 Sun Microsystems, Inc.  All Rights Reserved.
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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Enumeration;
import java.util.Properties;

/**
 * The main entry point into PluginAppletViewer.
 */
public class PluginMain
{
	final boolean redirectStreams = System.getenv().containsKey("ICEDTEAPLUGIN_DEBUG");
	static PluginStreamHandler streamHandler;
	
    // This is used in init().	Getting rid of this is desirable but depends
    // on whether the property that uses it is necessary/standard.
    public static final String theVersion = System.getProperty("java.version");
    
    private PluginAppletSecurityContext securityContext;

    /**
     * The main entry point into AppletViewer.
     */
    public static void main(String args[])
	throws IOException
    {

    	try {
    		PluginMain pm = new PluginMain(System.getProperty("user.home") + "/.icedteaplugin/icedtea-plugin-to-appletviewer", System.getProperty("user.home") + "/.icedteaplugin/icedtea-appletviewer-to-plugin");
    	} catch (Exception e) {
    		e.printStackTrace();
    		System.err.println("Something very bad happened. I don't know what to do, so I am going to exit :(");
    		System.exit(1);
    	}
    }

    public PluginMain(String inPipe, String outPipe) {
    	
    	if (redirectStreams) {
    		try {
    			File errFile = new File("/tmp/java.stderr");
    			File outFile = new File("/tmp/java.stdout");

    			System.setErr(new PrintStream(new FileOutputStream(errFile)));
    			System.setOut(new PrintStream(new FileOutputStream(outFile)));

    		} catch (Exception e) {
    			PluginDebug.debug("Unable to redirect streams");
    			e.printStackTrace();
    		}
    	}

    	connect(inPipe, outPipe);

    	securityContext = new PluginAppletSecurityContext(0);
    	securityContext.prePopulateLCClasses();
    	securityContext.setStreamhandler(streamHandler);
    	AppletSecurityContextManager.addContext(0, securityContext);

		PluginAppletViewer.setStreamhandler(streamHandler);
		PluginAppletViewer.setPluginCallRequestFactory(new PluginCallRequestFactory());

    	init();

		// Streams set. Start processing.
		streamHandler.startProcessing();
    }

	public void connect(String inPipe, String outPipe) {
		try {
			streamHandler = new PluginStreamHandler(new FileInputStream(inPipe), new FileOutputStream(outPipe));
	    	PluginDebug.debug("Streams initialized");
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	private static void init() {
		Properties avProps = new Properties();

		// ADD OTHER RANDOM PROPERTIES
		// XXX 5/18 need to revisit why these are here, is there some
		// standard for what is available?

		// Standard browser properties
		avProps.put("browser", "sun.applet.AppletViewer");
		avProps.put("browser.version", "1.06");
		avProps.put("browser.vendor", "Sun Microsystems Inc.");
		avProps.put("http.agent", "Java(tm) 2 SDK, Standard Edition v" + theVersion);

		// Define which packages can be extended by applets
		// XXX 5/19 probably not needed, not checked in AppletSecurity
		avProps.put("package.restrict.definition.java", "true");
		avProps.put("package.restrict.definition.sun", "true");

		// Define which properties can be read by applets.
		// A property named by "key" can be read only when its twin
		// property "key.applet" is true.  The following ten properties
		// are open by default.	 Any other property can be explicitly
		// opened up by the browser user by calling appletviewer with
		// -J-Dkey.applet=true
		avProps.put("java.version.applet", "true");
		avProps.put("java.vendor.applet", "true");
		avProps.put("java.vendor.url.applet", "true");
		avProps.put("java.class.version.applet", "true");
		avProps.put("os.name.applet", "true");
		avProps.put("os.version.applet", "true");
		avProps.put("os.arch.applet", "true");
		avProps.put("file.separator.applet", "true");
		avProps.put("path.separator.applet", "true");
		avProps.put("line.separator.applet", "true");

		// Read in the System properties.  If something is going to be
		// over-written, warn about it.
		Properties sysProps = System.getProperties();
		for (Enumeration e = sysProps.propertyNames(); e.hasMoreElements(); ) {
			String key = (String) e.nextElement();
			String val = (String) sysProps.getProperty(key);
			avProps.setProperty(key, val);
		}

		// INSTALL THE PROPERTY LIST
		System.setProperties(avProps);

		// REMIND: Create and install a socket factory!
	}

    static boolean messageAvailable() {
    	return streamHandler.messageAvailable();
    }

    static String getMessage() {
    	return streamHandler.getMessage();
    }
}
