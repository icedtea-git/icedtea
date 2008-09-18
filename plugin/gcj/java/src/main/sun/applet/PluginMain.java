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

import java.io.*;
import java.lang.reflect.Method;
import java.lang.reflect.InvocationTargetException;
import java.net.*;
import java.nio.charset.Charset;
import java.util.*;
import sun.net.www.ParseUtil;

class PluginParseRequest
{
  long handle;
  String tag;
  String documentbase;
  boolean alreadySent;
}

/**
 * The main entry point into PluginAppletViewer.
 */
public class PluginMain
{
    // A mapping of instance IDs to PluginAppletViewers.
    private static HashMap appletWindows = new HashMap();
    private static HashMap parseRequests = new HashMap();
    private static String currentKey;
    private static PluginAppletViewer currentWindow;
    private static PluginParseRequest currentRequest;
    private static BufferedReader pluginInputStream;
    private static BufferedWriter pluginOutputStream;
    // This is used in init().	Getting rid of this is desirable but depends
    // on whether the property that uses it is necessary/standard.
    public static final String theVersion = System.getProperty("java.version");

    /**
     * The main entry point into AppletViewer.
     */
    public static void main(String args[])
	throws IOException
    {
	if(args.length != 2) {
	    // Indicate to plugin that appletviewer is installed correctly.
	    System.exit(0);
	}
	// INSTALL THE SECURITY MANAGER
	init();
	start(new FileInputStream(args[0]), new FileOutputStream(args[1]));
	System.exit(0);
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

	// Create and install the security manager
	System.setSecurityManager(new AppletSecurity());

	// REMIND: Create and install a socket factory!
    }

    static void registerWindow(PluginAppletViewer pluginappletviewer)
    {
	appletWindows.put(currentKey, pluginappletviewer);
	currentWindow = (PluginAppletViewer)appletWindows.get(currentKey);
    }

    private static void deregisterWindow(PluginAppletViewer pluginappletviewer)
    {
	appletWindows.remove(currentWindow);
	currentWindow.dispose();
	currentWindow = null;
    }

    static void start(InputStream inputstream, OutputStream outputstream)
	throws MalformedURLException, IOException
    {
	// Set up input and output pipes.  Use UTF-8 encoding.
	pluginInputStream =
	    new BufferedReader(new InputStreamReader(inputstream,
						     Charset.forName("UTF-8")));
	pluginOutputStream =
	    new BufferedWriter(new OutputStreamWriter
			       (outputstream, Charset.forName("UTF-8")));

	write("running");

	// Read first message.
	String message = read();

	while(true) {
	    if (message.startsWith("instance")) {
		// Read applet instance identifier.
		currentKey = message.substring("instance".length() + 1);
		currentWindow =
		    (PluginAppletViewer)appletWindows.get(currentKey);
		currentRequest = null;
		if (currentWindow == null) {
		    if (!parseRequests.containsKey(currentKey))
			parseRequests.put(currentKey, new PluginParseRequest());
		    currentRequest =
			(PluginParseRequest)parseRequests.get(currentKey);
		}
	    } else if (message.startsWith("tag")) {
		if (currentRequest != null) {
		    int index = message.indexOf(' ', "tag".length() + 1);
		    currentRequest.documentbase =
			message.substring("tag".length() + 1, index);
		    currentRequest.tag = message.substring(index + 1);
		    if (currentRequest.handle != 0
			&& !currentRequest.alreadySent) {
			PluginAppletViewer.parse
			    (currentRequest.handle, 1, 1,
			     new StringReader(currentRequest.tag),
			     new URL(currentRequest.documentbase));
			parseRequests.remove(currentKey);
		    }
		}
	    } else if (message.startsWith("handle")) {
		if (currentRequest != null) {
		    currentRequest.handle = Long.parseLong
			(message.substring("handle".length() + 1, 
						message.indexOf("width") - 1));
		    int width = Integer.parseInt(message.substring
					(message.indexOf("width") + 
					"width".length() + 1, 
					message.indexOf("height") - 1));
		    int height = Integer.parseInt(message.substring(
					message.indexOf("height") + 
					"height".length() + 1));
		    if (currentRequest.tag != null
		       && !currentRequest.alreadySent) {
			PluginAppletViewer.parse
			    (currentRequest.handle, width, height,
			     new StringReader(currentRequest.tag),
			     new URL(currentRequest.documentbase));
			parseRequests.remove(currentKey);
		    }
		}
	    } else if (message.startsWith("width")) {
		int width =
		    Integer.parseInt(message.substring("width".length() + 1));
		int height = currentWindow.getHeight();
		currentWindow.updateAtts(width, height);
	 	currentWindow.setSize(width, height);
	    } else if (message.startsWith("height")) {
		int height =
		    Integer.parseInt(message.substring("height".length() + 1));
		int width = currentWindow.getWidth();
		currentWindow.updateAtts(width, height);
	        currentWindow.setSize(width, height);
	    } else if (message.startsWith("destroy")
		       && currentWindow != null) {
		deregisterWindow(currentWindow);
	    }

	    // Read next message.
	    message = read();
	}
    }

    /**
     * Write string to plugin.
     * 
     * @param message the message to write
     *
     * @exception IOException if an error occurs
     */
    static void write(String message)
	throws IOException
    {
	pluginOutputStream.write(message, 0, message.length());
	pluginOutputStream.newLine();
	pluginOutputStream.flush();

	System.err.println("  PIPE: appletviewer wrote: " + message);
    }

    /**
     * Read string from plugin.
     *
     * @return the read string
     *
     * @exception IOException if an error occurs
     */
    static String read()
	throws IOException
    {
	String message = pluginInputStream.readLine();
	System.err.println("  PIPE: appletviewer read: " + message);
	if (message == null || message.equals("shutdown")) {
	    try {
		// Close input/output channels to plugin.
		pluginInputStream.close();
		pluginOutputStream.close();
	    } catch (IOException exception) {
		// Deliberately ignore IOException caused by broken
		// pipe since plugin may have already detached.
	    }

	    System.err.println("APPLETVIEWER: exiting appletviewer");
	    System.exit(0);
	}
	return message;
    }
}
