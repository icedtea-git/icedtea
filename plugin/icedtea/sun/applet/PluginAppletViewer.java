 /*
  * Copyright 1995-2004 Sun Microsystems, Inc.  All Rights Reserved.
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
 
 import java.applet.Applet;
import java.applet.AppletContext;
import java.applet.AudioClip;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Label;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.Reader;
import java.io.StringReader;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.SocketPermission;
import java.net.URL;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import javax.swing.SwingUtilities;

import net.sourceforge.jnlp.NetxPanel;
import sun.awt.AppContext;
import sun.awt.SunToolkit;
import sun.awt.X11.XEmbeddedFrame;
import sun.misc.Ref;
 
 /**
  * Lets us construct one using unix-style one shot behaviors
  */
 
 class PluginAppletViewerFactory
 {
     public PluginAppletViewer createAppletViewer(int identifier,
                                                  long handle, int x, int y,
 						 URL doc, Hashtable atts) {
         PluginAppletViewer pluginappletviewer = new PluginAppletViewer(identifier, handle, x, y, doc, atts, System.out, this);
         return pluginappletviewer;
     }
 
     public boolean isStandalone()
     {
         return false;
     }
 }
 
 class PluginParseRequest
 {
     long handle;
     String tag;
     String documentbase;
 }
 
 /*
  */
 // FIXME: declare JSProxy implementation
 public class PluginAppletViewer extends XEmbeddedFrame
     implements AppletContext, Printable {
     /**
      * Some constants...
      */
     private static String defaultSaveFile = "Applet.ser";
 
     /**
      * The panel in which the applet is being displayed.
      */
     AppletViewerPanel panel;
 
     /**
      * The status line.
      */
     Label label;
 
     /**
      * output status messages to this stream
      */
 
     PrintStream statusMsgStream;
 
     /**
      * For cloning
      */
     PluginAppletViewerFactory factory;
 
     int identifier;
 
     private static HashMap<Integer, PluginParseRequest> requests = new HashMap();
 
     // Instance identifier -> PluginAppletViewer object.
     private static HashMap<Integer, PluginAppletViewer> applets = new HashMap();
     
     private static PluginStreamHandler streamhandler;
     
     private static PluginCallRequestFactory requestFactory;
     
     private double proposedHeightFactor;
     private double proposedWidthFactor;

     /**
      * Null constructor to allow instantiation via newInstance()
      */
     public PluginAppletViewer() {
     }
     
     /**
      * Create the applet viewer
      */
     public PluginAppletViewer(int identifier, long handle, int x, int y, final URL doc,
                               final Hashtable atts, PrintStream statusMsgStream,
                               PluginAppletViewerFactory factory) {
         super(handle, true);
    	this.factory = factory;
 	this.statusMsgStream = statusMsgStream;
         this.identifier = identifier;
         // FIXME: when/where do we remove this?
         PluginDebug.debug ("PARSING: PUTTING " + identifier + " " + this);
         applets.put(identifier, this);
         
         
         // we intercept height and width specifications here because 
         proposedHeightFactor = 1.0;
         proposedWidthFactor = 1.0;

         if (atts.get("heightPercentage") != null) {
        	 proposedHeightFactor = (Integer) atts.get("heightPercentage")/100.0;
         }
         
         if (((String) atts.get("width")).endsWith("%")) {
        	 proposedWidthFactor = (Integer) atts.get("widthPercentage")/100.0;
         }
 
         AccessController.doPrivileged(new PrivilegedAction() {
             public Object run() {
            	 	try {
            	 		panel = new NetxPanel(doc, atts, true);
            	 		AppletViewerPanel.debug("Using NetX panel");
            	 	} catch (Exception ex) {
            	 		AppletViewerPanel.debug("Unable to start NetX applet - defaulting to Sun applet", ex);
            	 		panel = new AppletViewerPanel(doc, atts);
            	 	}
                 return null;
             }
         });  

 	add("Center", panel);
 	panel.init();
 	appletPanels.addElement(panel);
 
 	pack();
 	setVisible(true);
 
 	WindowListener windowEventListener = new WindowAdapter() {
 
 	    public void windowClosing(WindowEvent evt) {
 		appletClose();
 	    }
 
 	    public void windowIconified(WindowEvent evt) {
 		appletStop();
 	    }
 
 	    public void windowDeiconified(WindowEvent evt) {
 		appletStart();
 	    }
 	};
 
 	class AppletEventListener implements AppletListener  
 	{
 	    final Frame frame;
 
 	    public AppletEventListener(Frame frame)
 	    {
 		this.frame = frame;
 	    }
 
 	    public void appletStateChanged(AppletEvent evt) 
 	    {
 		AppletPanel src = (AppletPanel)evt.getSource();
 
 		switch (evt.getID()) {
                     case AppletPanel.APPLET_RESIZE: {
 			if(src != null) {
 			    resize(preferredSize());
 			    validate();
                         }
 			break;
 		    }
 		    case AppletPanel.APPLET_LOADING_COMPLETED: {
 			Applet a = src.getApplet(); // sun.applet.AppletPanel
 			
 			// Fixed #4754451: Applet can have methods running on main
 			// thread event queue. 
 			// 
 			// The cause of this bug is that the frame of the applet 
 			// is created in main thread group. Thus, when certain 
 			// AWT/Swing events are generated, the events will be
 			// dispatched through the wrong event dispatch thread.
 			//
 			// To fix this, we rearrange the AppContext with the frame,
 			// so the proper event queue will be looked up.
 			//
 			// Swing also maintains a Frame list for the AppContext,
 			// so we will have to rearrange it as well.
 			//
 			if (a != null)
 			    AppletPanel.changeFrameAppContext(frame, SunToolkit.targetToAppContext(a));
 			else
 			    AppletPanel.changeFrameAppContext(frame, AppContext.getAppContext());
 
 			break;
 		    }
 		}
 	    }
 	};
 
 	addWindowListener(windowEventListener);
 	panel.addAppletListener(new AppletEventListener(this));
 
 	// Start the applet
    showStatus(amh.getMessage("status.start"));
 	initEventQueue();
 	
    // Wait for a maximum of 10 seconds for the panel to initialize
    // (happens in a separate thread)
 	Applet a;
    int maxSleepTime = 10000;
    int sleepTime = 0;
    while ((a = panel.getApplet()) == null && sleepTime < maxSleepTime) {
   	 try {
   		 Thread.sleep(100);
   		 sleepTime += 100;
   		 PluginDebug.debug("Waiting for applet to initialize... ");
   	 } catch (InterruptedException ie) {
   		 ie.printStackTrace();
   	 }
    }

    // Still null?
    if (panel.getApplet() == null) {
    	this.streamhandler.write("instance " + identifier + " reference " + -1 + " fatalError " + "Initialization failed");
    	return;
    }

    PluginDebug.debug("Applet initialized");

    // Applet initialized. Find out it's classloader and add it to the list
    String codeBase = doc.getProtocol() + "://" + doc.getHost();

    if (atts.get("codebase") != null) {
    	try {
    		URL appletSrcURL = new URL((String) atts.get("codebase"));
    		codeBase = appletSrcURL.getProtocol() + "://" + appletSrcURL.getHost();
    	} catch (MalformedURLException mfue) {
    		// do nothing
    	}
    }

    AppletSecurityContextManager.getSecurityContext(0).associateSrc(a.getClass().getClassLoader(), codeBase);
    
 	try {
 	    write("initialized");
 	} catch (IOException ioe) {
 		ioe.printStackTrace();
 	}

     }

 	public static void setStreamhandler(PluginStreamHandler sh) {
		streamhandler = sh;
	}

 	public static void setPluginCallRequestFactory(PluginCallRequestFactory rf) {
		requestFactory = rf;
	}

     /**
      * Handle an incoming message from the plugin.
      */
     public static void handleMessage(int identifier, int reference, String message)
     {

		 PluginDebug.debug("PAV handling: " + message);
		 
         try {
        	 if (message.startsWith("tag")) {
        		 
        		 // tag and handle must both be set before parsing, so we need
        		 // synchronization here, as the setting of these variables
        		 // may happen in independent threads
        		 
        		 synchronized(requests) {
        			 PluginParseRequest request = requests.get(identifier);
        			 if (request == null) {
        				 request = new PluginParseRequest();
        				 requests.put(identifier, request);
        			 }
        			 int index = message.indexOf(' ', "tag".length() + 1);
        			 request.documentbase =
        				 message.substring("tag".length() + 1, index);
        			 request.tag = message.substring(index + 1);
        			 PluginDebug.debug ("REQUEST TAG: " + request.tag + " " +
        					 Thread.currentThread());

        			 if (request.handle != 0) {
        				 PluginDebug.debug ("REQUEST TAG, PARSING " +
        						 Thread.currentThread());
        				 PluginAppletViewer.parse
        				 (identifier, request.handle,
        						 new StringReader(request.tag),
        						 new URL(request.documentbase));
        				 requests.remove(identifier);
        			 } else {
        				 PluginDebug.debug ("REQUEST HANDLE NOT SET: " + request.handle + ". BYPASSING");
        			 }
        		 }
        		 
             } else if (message.startsWith("handle")) {
            	 synchronized(requests) {
            		 PluginParseRequest request = requests.get(identifier);
            		 if (request == null) {
            			 request = new PluginParseRequest();
            			 requests.put(identifier, request);
            		 }
            		 request.handle = Long.parseLong
            		 (message.substring("handle".length() + 1));
            		 PluginDebug.debug ("REQUEST HANDLE: " + request.handle);
            		 if (request.tag != null) {
            			 PluginDebug.debug ("REQUEST HANDLE, PARSING " +
            					 Thread.currentThread());
            			 PluginAppletViewer.parse
            			 (identifier, request.handle,
            					 new StringReader(request.tag),
            					 new URL(request.documentbase));
            			 requests.remove(identifier);
            			 PluginDebug.debug ("REQUEST HANDLE, DONE PARSING " +
            					 Thread.currentThread());
            		 } else {
            			 PluginDebug.debug ("REQUEST TAG NOT SET: " + request.tag + ". BYPASSING");
            		 }
            	 }
             } else {
                 PluginDebug.debug ("HANDLING MESSAGE " + message + " instance " + identifier + " " + Thread.currentThread());
                 applets.get(identifier).handleMessage(reference, message);
             }
         } catch (Exception e) {
             throw new RuntimeException("Failed to handle message: " + message + " " +
                                         Thread.currentThread(), e);
         }
     }
 
     public void handleMessage(int reference, String message)
     {
         if (message.startsWith("width")) {

        	 // 0 => width, 1=> width_value, 2 => height, 3=> height_value
        	 String[] dimMsg = message.split(" ");
        	 
        	 final int height = (int) (proposedHeightFactor*Integer.parseInt(dimMsg[3]));
        	 final int width = (int) (proposedWidthFactor*Integer.parseInt(dimMsg[1]));

        	 if (panel instanceof NetxPanel)
        		 ((NetxPanel) panel).updateSizeInAtts(height, width);

        	 try {
				SwingUtilities.invokeAndWait(new Runnable() {
					 public void run() {

			        	 setSize(width, height);
						 
						 // There is a rather odd drawing bug whereby resizing 
						 // the panel makes no difference on initial call 
						 // because the panel thinks that it is already the 
						 // right size. Validation has no effect there either. 
						 // So we work around by setting size to 1, validating, 
						 // and then setting to the right size and validating 
						 // again. This is not very efficient, and there is 
						 // probably a better way -- but resizing happens 
						 // quite infrequently, so for now this is how we do it

			        	 panel.setSize(1,1);
			        	 panel.validate();

			        	 panel.setSize(width, height);
			        	 panel.validate();
					 }
				 });
			} catch (InterruptedException e) {
				// do nothing
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// do nothing
				e.printStackTrace();
			}
         } else if (message.startsWith("destroy")) {
             dispose();
         } else if (message.startsWith("GetJavaObject")) {
             // FIXME: how do we determine what security context this
             // object should belong to?
             Object o;

             // Wait for a maximum of 10 seconds for the panel to initialize
             // (happens in a separate thread)
             int maxSleepTime = 10000;
             int sleepTime = 0;
             while ((o = panel.getApplet()) == null && sleepTime < maxSleepTime) {
            	 try {
            		 Thread.sleep(100);
            		 sleepTime += 100;
            		 PluginDebug.debug("Waiting for applet to initialize...");
            	 } catch (InterruptedException ie) {
            		 ie.printStackTrace();
            	 }
             }

             PluginDebug.debug ("Looking for object " + o + " panel is " + panel);
             AppletSecurityContextManager.getSecurityContext(0).store(o);
             PluginDebug.debug ("WRITING 1: " + "context 0 reference " + reference + " GetJavaObject "
                                 + AppletSecurityContextManager.getSecurityContext(0).getIdentifier(o));
             streamhandler.write("context 0 reference " + reference + " GetJavaObject "
                              + AppletSecurityContextManager.getSecurityContext(0).getIdentifier(o));
             PluginDebug.debug ("WRITING 1 DONE");
         }
     }

     // FIXME: Kind of hackish way to ensure synchronized re-drawing 
     private synchronized void forceredraw() {
    	 doLayout();
     }
     
     /**
      * Send the initial set of events to the appletviewer event queue.
      * On start-up the current behaviour is to load the applet and call
      * Applet.init() and Applet.start().
      */
     private void initEventQueue() {
 	// appletviewer.send.event is an undocumented and unsupported system
 	// property which is used exclusively for testing purposes.
    	 PrivilegedAction pa = new PrivilegedAction() {
    		 public Object run() {
    			 return System.getProperty("appletviewer.send.event");
    		 }
    	 };
 	String eventList = (String) AccessController.doPrivileged(pa); 
 
 	if (eventList == null) {
 	    // Add the standard events onto the event queue.
 	    panel.sendEvent(AppletPanel.APPLET_LOAD);
 	    panel.sendEvent(AppletPanel.APPLET_INIT);
 	    panel.sendEvent(AppletPanel.APPLET_START);
 	} else {
 	    // We're testing AppletViewer.  Force the specified set of events
 	    // onto the event queue, wait for the events to be processed, and
 	    // exit.
 
 	    // The list of events that will be executed is provided as a
 	    // ","-separated list.  No error-checking will be done on the list.
   	    String [] events = splitSeparator(",", eventList);
 
  	    for (int i = 0; i < events.length; i++) {
  	    PluginDebug.debug("Adding event to queue: " + events[i]);
  		if (events[i].equals("dispose"))
  		    panel.sendEvent(AppletPanel.APPLET_DISPOSE);
  		else if (events[i].equals("load"))
  		    panel.sendEvent(AppletPanel.APPLET_LOAD);
  		else if (events[i].equals("init"))
  		    panel.sendEvent(AppletPanel.APPLET_INIT);
  		else if (events[i].equals("start"))
  		    panel.sendEvent(AppletPanel.APPLET_START);
  		else if (events[i].equals("stop"))
  		    panel.sendEvent(AppletPanel.APPLET_STOP);
  		else if (events[i].equals("destroy"))
  		    panel.sendEvent(AppletPanel.APPLET_DESTROY);
  		else if (events[i].equals("quit"))
  		    panel.sendEvent(AppletPanel.APPLET_QUIT);
  		else if (events[i].equals("error"))
  		    panel.sendEvent(AppletPanel.APPLET_ERROR);
  		else
 		    // non-fatal error if we get an unrecognized event
  		    PluginDebug.debug("Unrecognized event name: " + events[i]);
  	    }
 
   	    while (!panel.emptyEventQueue()) ;
  	    appletSystemExit();
 	}
     }
 
     /**
      * Split a string based on the presence of a specified separator.  Returns
      * an array of arbitrary length.  The end of each element in the array is
      * indicated by the separator of the end of the string.  If there is a
      * separator immediately before the end of the string, the final element
      * will be empty.  None of the strings will contain the separator.  Useful
      * when separating strings such as "foo/bar/bas" using separator "/".
      *
      * @param sep  The separator.
      * @param s    The string to split.
      * @return     An array of strings.  Each string in the array is determined
      *             by the location of the provided sep in the original string,
      *             s.  Whitespace not stripped.
      */
     private String [] splitSeparator(String sep, String s) {
  	Vector v = new Vector();
 	int tokenStart = 0;
 	int tokenEnd   = 0;
 
 	while ((tokenEnd = s.indexOf(sep, tokenStart)) != -1) {
 	    v.addElement(s.substring(tokenStart, tokenEnd));
 	    tokenStart = tokenEnd+1;
 	}
 	// Add the final element.
 	v.addElement(s.substring(tokenStart));
 
 	String [] retVal = new String[v.size()];
 	v.copyInto(retVal);
  	return retVal;
     }
 
     /*
      * Methods for java.applet.AppletContext
      */
 
     private static Map audioClips = new HashMap();
 
     /**
      * Get an audio clip.
      */
     public AudioClip getAudioClip(URL url) {
 	checkConnect(url);
 	synchronized (audioClips) {
 	    AudioClip clip = (AudioClip)audioClips.get(url);
 	    if (clip == null) {
 		audioClips.put(url, clip = new AppletAudioClip(url));
 	    }
 	    return clip;
 	}
     }
 
     private static Map imageRefs = new HashMap();
 
     /**
      * Get an image.
      */
     public Image getImage(URL url) {
 	return getCachedImage(url);
     }
 
     static Image getCachedImage(URL url) {
 	// System.getSecurityManager().checkConnection(url.getHost(), url.getPort());
 	return (Image)getCachedImageRef(url).get();
     }
 
     /**
      * Get an image ref.
      */
     static Ref getCachedImageRef(URL url) {
 	synchronized (imageRefs) {
 	    AppletImageRef ref = (AppletImageRef)imageRefs.get(url);
 	    if (ref == null) {
 		ref = new AppletImageRef(url);
 		imageRefs.put(url, ref);
 	    }
 	    return ref;
 	}
     }
 
     /**
      * Flush the image cache.
      */
     static void flushImageCache() {
 	imageRefs.clear();
     }
 
     static Vector appletPanels = new Vector();
 
     /**
      * Get an applet by name.
      */
     public Applet getApplet(String name) {
 	AppletSecurity security = (AppletSecurity)System.getSecurityManager();
 	name = name.toLowerCase();
 	SocketPermission panelSp =
 	    new SocketPermission(panel.getCodeBase().getHost(), "connect");
 	for (Enumeration e = appletPanels.elements() ; e.hasMoreElements() ;) {
 	    AppletPanel p = (AppletPanel)e.nextElement();
 	    String param = p.getParameter("name");
 	    if (param != null) {
 		param = param.toLowerCase();
 	    }
 	    if (name.equals(param) &&
 		p.getDocumentBase().equals(panel.getDocumentBase())) {
 
 		SocketPermission sp =
 		    new SocketPermission(p.getCodeBase().getHost(), "connect");
 
 		if (panelSp.implies(sp)) {
 		    return p.applet;
 		}
 	    }
 	}
 	return null;
     }
 
     /**
      * Return an enumeration of all the accessible
      * applets on this page.
      */
     public Enumeration getApplets() {
 	AppletSecurity security = (AppletSecurity)System.getSecurityManager();
 	Vector v = new Vector();
 	SocketPermission panelSp =
 	    new SocketPermission(panel.getCodeBase().getHost(), "connect");
 
 	for (Enumeration e = appletPanels.elements() ; e.hasMoreElements() ;) {
 	    AppletPanel p = (AppletPanel)e.nextElement();
 	    if (p.getDocumentBase().equals(panel.getDocumentBase())) {
 
 		SocketPermission sp =
 		    new SocketPermission(p.getCodeBase().getHost(), "connect");
 		if (panelSp.implies(sp)) {
 		    v.addElement(p.applet);
 		}
 	    }
 	}
 	return v.elements();
     }
 
     /**
      * Ignore.
      */
     public void showDocument(URL url) {
    	 PluginDebug.debug("Showing document...");
 	showDocument(url, "_self");
     }
 
     /**
      * Ignore.
      */
     public void showDocument(URL url, String target) {
 	try {
             // FIXME: change to postCallRequest
 	    write("url " + url + " " + target);
 	} catch (IOException exception) {
 	    // Deliberately ignore IOException.  showDocument may be
 	    // called from threads other than the main thread after
 	    // streamhandler.pluginOutputStream has been closed.
 	}
     }
 
     /**
      * Show status.
      */
     public void showStatus(String status) {
 	try {
             // FIXME: change to postCallRequest
 	    write("status " + status);
 	} catch (IOException exception) {
 	    // Deliberately ignore IOException.  showStatus may be
 	    // called from threads other than the main thread after
 	    // streamhandler.pluginOutputStream has been closed.
 	}
     }
 
     public int getWindow() {
    	 PluginDebug.debug ("STARTING getWindow");
    	 PluginCallRequest request = requestFactory.getPluginCallRequest("window",
    			 							"instance " + identifier + " " + "GetWindow", 
    			 							"JavaScriptGetWindow");
    	 PluginDebug.debug ("STARTING postCallRequest");
		 streamhandler.postCallRequest(request);
    	 PluginDebug.debug ("STARTING postCallRequest done");
    	 streamhandler.write(request.getMessage());
    	 try {
    		 PluginDebug.debug ("wait request 1");
    		 synchronized(request) {
    			 PluginDebug.debug ("wait request 2");
    			 while ((Integer) request.getObject() == 0)
    				 request.wait();
    			 PluginDebug.debug ("wait request 3");
    		 }
    	 } catch (InterruptedException e) {
    		 throw new RuntimeException("Interrupted waiting for call request.",
    				 e);
    	 }

    	 PluginDebug.debug ("STARTING getWindow DONE");
    	 return (Integer) request.getObject();
     }
 
     // FIXME: make private, access via reflection.
     public static Object getMember(int internal, String name)
     {
    	 AppletSecurityContextManager.getSecurityContext(0).store(name);
         int nameID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(name);
 
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("member", 
        		 							"instance " + 0 + " GetMember " + internal + " " + nameID, 
        		 							"JavaScriptGetMember");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait getMEM request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait getMEM request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait getMEM request 3 GOT: " + request.getObject().getClass());
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" getMember DONE");
         return request.getObject();
     }
 
     public static void setMember(int internal, String name, Object value) {
    	 AppletSecurityContextManager.getSecurityContext(0).store(name);
         int nameID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(name);
         AppletSecurityContextManager.getSecurityContext(0).store(value);
         int valueID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(value);
 
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("void",
        		 							"instance " + 0 + " SetMember " + internal + " " + nameID + " " + valueID, 
        		 							"JavaScriptSetMember");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait setMem request: " + request.getMessage());
             PluginDebug.debug ("wait setMem request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait setMem request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait setMem request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" setMember DONE");
     }
 
     // FIXME: handle long index as well.
     public static void setSlot(int internal, int index, Object value) {
    	 AppletSecurityContextManager.getSecurityContext(0).store(value);
         int valueID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(value);
 
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("void",
        		 						"instance " + 0 + " SetSlot " + internal + " " + index + " " + valueID, 
        		 						"JavaScriptSetSlot");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait setSlot request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait setSlot request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait setSlot request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" setSlot DONE");
     }
 
     public static Object getSlot(int internal, int index)
     {
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("member", 
        		 								"instance " + 0 + " GetSlot " + internal + " " + index, 
        		 								"JavaScriptGetSlot");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait getSlot request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait getSlot request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait getSlot request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" getSlot DONE");
         return request.getObject();
     }
 
     public static Object eval(int internal, String s)
     {
    	 AppletSecurityContextManager.getSecurityContext(0).store(s);
         int stringID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(s);
         // Prefix with dummy instance for convenience.
         // FIXME: rename GetMemberPluginCallRequest ObjectPluginCallRequest.
         PluginCallRequest request = requestFactory.getPluginCallRequest("member",	
        		 								"instance " + 0 + " Eval " + internal + " " + stringID, 
        		 								"JavaScriptEval");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait eval request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait eval request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait eval request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" getSlot DONE");
         return request.getObject();
     }
 
     public static void removeMember (int internal, String name) {
    	 AppletSecurityContextManager.getSecurityContext(0).store(name);
         int nameID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(name);
 
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("void",
        		 						"instance " + 0 + " RemoveMember " + internal + " " + nameID, 
        		 						"JavaScriptRemoveMember");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait removeMember request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait removeMember request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait removeMember request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" RemoveMember DONE");
     }
 
     public static Object call(int internal, String name, Object args[])
     {
         // FIXME: when is this removed from the object store?
         // FIXME: reference should return the ID.
         // FIXME: convenience method for this long line.
    	 AppletSecurityContextManager.getSecurityContext(0).store(name);
         int nameID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(name);
         AppletSecurityContextManager.getSecurityContext(0).store(args);
         int argsID = AppletSecurityContextManager.getSecurityContext(0).getIdentifier(args);
 
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("member",
        		 							"instance " + 0 + " Call " + internal + " " + nameID + " " + argsID, 
        		 							"JavaScriptCall");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait call request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait call request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait call request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" Call DONE");
         return request.getObject();
     }
 
     public static void JavaScriptFinalize(int internal)
     {
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("void",
        		 						"instance " + 0 + " Finalize " + internal, 
        		 						"JavaScriptFinalize");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait finalize request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait finalize request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait finalize request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" finalize DONE");
     }
 
     public static String javascriptToString(int internal)
     {
         // Prefix with dummy instance for convenience.
         PluginCallRequest request = requestFactory.getPluginCallRequest("member",
        		 								"instance " + 0 + " ToString " + internal, 
        		 								"JavaScriptToString");
         streamhandler.postCallRequest(request);
         streamhandler.write(request.getMessage());
         try {
             PluginDebug.debug ("wait ToString request 1");
             synchronized(request) {
                 PluginDebug.debug ("wait ToString request 2");
                 while (request.isDone() == false)
                     request.wait();
                 PluginDebug.debug ("wait ToString request 3");
             }
         } catch (InterruptedException e) {
             throw new RuntimeException("Interrupted waiting for call request.",
                                        e);
         }
         PluginDebug.debug (" ToString DONE");
         return (String) request.getObject();
     }
 
     // FIXME: make this private and access it from JSObject using
     // reflection.
     private void write(String message) throws IOException {
         PluginDebug.debug ("WRITING 2: " + "instance " + identifier + " " + message);
         streamhandler.write("instance " + identifier + " " + message);
         PluginDebug.debug ("WRITING 2 DONE");
     }

     public void setStream(String key, InputStream stream)throws IOException{
 	// We do nothing.
     }
 
     public InputStream getStream(String key){
 	// We do nothing.
 	return null;
     }
 
     public Iterator getStreamKeys(){
 	// We do nothing.
 	return null;
     }
 
     /**
      * System parameters.
      */
     static Hashtable systemParam = new Hashtable();
 
     static {
 	systemParam.put("codebase", "codebase");
 	systemParam.put("code", "code");
 	systemParam.put("alt", "alt");
 	systemParam.put("width", "width");
 	systemParam.put("height", "height");
 	systemParam.put("align", "align");
 	systemParam.put("vspace", "vspace");
 	systemParam.put("hspace", "hspace");
     }
 
     /**
      * Print the HTML tag.
      */
     public static void printTag(PrintStream out, Hashtable atts) {
 	out.print("<applet");
 
 	String v = (String)atts.get("codebase");
 	if (v != null) {
 	    out.print(" codebase=\"" + v + "\"");
 	}
 
 	v = (String)atts.get("code");
 	if (v == null) {
 	    v = "applet.class";
 	}
 	out.print(" code=\"" + v + "\"");
 	v = (String)atts.get("width");
 	if (v == null) {
 	    v = "150";
 	}
 	out.print(" width=" + v);
 
 	v = (String)atts.get("height");
 	if (v == null) {
 	    v = "100";
 	}
 	out.print(" height=" + v);
 
 	v = (String)atts.get("name");
 	if (v != null) {
 	    out.print(" name=\"" + v + "\"");
 	}
 	out.println(">");
 
 	// A very slow sorting algorithm
 	int len = atts.size();
 	String params[] = new String[len];
 	len = 0;
 	for (Enumeration e = atts.keys() ; e.hasMoreElements() ;) {
 	    String param = (String)e.nextElement();
 	    int i = 0;
 	    for (; i < len ; i++) {
 		if (params[i].compareTo(param) >= 0) {
 		    break;
 		}
 	    }
 	    System.arraycopy(params, i, params, i + 1, len - i);
 	    params[i] = param;
 	    len++;
 	}
 
 	for (int i = 0 ; i < len ; i++) {
 	    String param = params[i];
 	    if (systemParam.get(param) == null) {
 		out.println("<param name=" + param +
 			    " value=\"" + atts.get(param) + "\">");
 	    }
 	}
 	out.println("</applet>");
     }
 
     /**
      * Make sure the atrributes are uptodate.
      */
     public void updateAtts() {
 	Dimension d = panel.size();
 	Insets in = panel.insets();
 	panel.atts.put("width",
 		       new Integer(d.width - (in.left + in.right)).toString());
 	panel.atts.put("height",
 		       new Integer(d.height - (in.top + in.bottom)).toString());
     }
 
     /**
      * Restart the applet.
      */
     void appletRestart() {
 	panel.sendEvent(AppletPanel.APPLET_STOP);
 	panel.sendEvent(AppletPanel.APPLET_DESTROY);
 	panel.sendEvent(AppletPanel.APPLET_INIT);
 	panel.sendEvent(AppletPanel.APPLET_START);
     }
 
     /**
      * Reload the applet.
      */
     void appletReload() {
 	panel.sendEvent(AppletPanel.APPLET_STOP);
 	panel.sendEvent(AppletPanel.APPLET_DESTROY);
 	panel.sendEvent(AppletPanel.APPLET_DISPOSE);
 
 	/**
 	 * Fixed #4501142: Classlaoder sharing policy doesn't 
 	 * take "archive" into account. This will be overridden
 	 * by Java Plug-in.			[stanleyh]
 	 */
 	AppletPanel.flushClassLoader(panel.getClassLoaderCacheKey());
 
         /*
          * Make sure we don't have two threads running through the event queue
          * at the same time.
          */
         try {
             panel.joinAppletThread();
 	    panel.release();
         } catch (InterruptedException e) {
             return;   // abort the reload
         }
 
         AccessController.doPrivileged(new PrivilegedAction() {
             public Object run() {
            	 panel.createAppletThread();
                 return null;
             }
         });     
    
 	panel.sendEvent(AppletPanel.APPLET_LOAD);
 	panel.sendEvent(AppletPanel.APPLET_INIT);
 	panel.sendEvent(AppletPanel.APPLET_START);
     }
 
     public int print(Graphics graphics, PageFormat pf, int pageIndex) {
         return Printable.NO_SUCH_PAGE;
     }
 
     /**
      * Start the applet.
      */
     void appletStart() {
 	panel.sendEvent(AppletPanel.APPLET_START);
     }
 
     /**
      * Stop the applet.
      */
     void appletStop() {
 	panel.sendEvent(AppletPanel.APPLET_STOP);
     }
 
     /**
      * Shutdown a viewer.
      * Stop, Destroy, Dispose and Quit a viewer
      */
     private void appletShutdown(AppletPanel p) {
 	p.sendEvent(AppletPanel.APPLET_STOP);
 	p.sendEvent(AppletPanel.APPLET_DESTROY);
 	p.sendEvent(AppletPanel.APPLET_DISPOSE);
 	p.sendEvent(AppletPanel.APPLET_QUIT);
     }
 
     /**
      * Close this viewer.
      * Stop, Destroy, Dispose and Quit an AppletView, then
      * reclaim resources and exit the program if this is
      * the last applet.
      */
     void appletClose() {
 
 	// The caller thread is event dispatch thread, so
 	// spawn a new thread to avoid blocking the event queue
 	// when calling appletShutdown.
 	//
 	final AppletPanel p = panel;
 
 	new Thread(new Runnable()
 	{
 	    public void run()
 	    {
     		appletShutdown(p);
 		appletPanels.removeElement(p);
 		dispose();
 
 		if (countApplets() == 0) {
 		    appletSystemExit();
 		}
 	    }
 	}).start();
     }
 
     /**
      * Exit the program.
      * Exit from the program (if not stand alone) - do no clean-up
      */
     private void appletSystemExit() {
 	if (factory.isStandalone())
 	    System.exit(0);
     }
 
     /**
      * How many applets are running?
      */
 
     public static int countApplets() {
 	return appletPanels.size();
     }
 
 
     /**
      * The current character.
      */
     static int c;
 
     /**
      * Scan spaces.
      */
     public static void skipSpace(Reader in) throws IOException {
         while ((c >= 0) &&
 	       ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))) {
 	    c = in.read();
 	}
     }
 
     /**
      * Scan identifier
      */
     public static String scanIdentifier(Reader in) throws IOException {
 	StringBuffer buf = new StringBuffer();
 	while (true) {
 	    if (((c >= 'a') && (c <= 'z')) ||
 		((c >= 'A') && (c <= 'Z')) ||
 		((c >= '0') && (c <= '9')) || (c == '_')) {
 		buf.append((char)c);
 		c = in.read();
 	    } else {
 		return buf.toString();
 	    }
 	}
     }
 
     /**
      * Scan tag
      */
     public static Hashtable scanTag(Reader in) throws IOException {
 	Hashtable atts = new Hashtable();
 	skipSpace(in);
         while (c >= 0 && c != '>') {
 	    String att = scanIdentifier(in);
 	    String val = "";
 	    skipSpace(in);
 	    if (c == '=') {
 		int quote = -1;
 		c = in.read();
 		skipSpace(in);
 		if ((c == '\'') || (c == '\"')) {
 		    quote = c;
 		    c = in.read();
 		}
 		StringBuffer buf = new StringBuffer();
                 while ((c > 0) &&
 		       (((quote < 0) && (c != ' ') && (c != '\t') &&
                          (c != '\n') && (c != '\r') && (c != '>'))
 			|| ((quote >= 0) && (c != quote)))) {
 		    buf.append((char)c);
 		    c = in.read();
 		}
 		if (c == quote) {
 		    c = in.read();
 		}
 		skipSpace(in);
 		val = buf.toString();
 	    }
 	    PluginDebug.debug("PUT " + att + " = '" + val + "'");
 	    atts.put(att.toLowerCase(java.util.Locale.ENGLISH), val);
             while (true) {
                 if ((c == '>') || (c < 0) ||
                     ((c >= 'a') && (c <= 'z')) ||
                     ((c >= 'A') && (c <= 'Z')) ||
                     ((c >= '0') && (c <= '9')) || (c == '_'))
                     break;
                 c = in.read();
             }
             //skipSpace(in);
 	}
 	return atts;
     }
 
     /* values used for placement of AppletViewer's frames */
     private static int x = 0;
     private static int y = 0;
     private static final int XDELTA = 30;
     private static final int YDELTA = XDELTA;
 
     static String encoding = null;
 
     static private Reader makeReader(InputStream is) {
 	if (encoding != null) {
 	    try {
 		return new BufferedReader(new InputStreamReader(is, encoding));
 	    } catch (IOException x) { }
 	}
 	InputStreamReader r = new InputStreamReader(is);
 	encoding = r.getEncoding();
 	return new BufferedReader(r);
     }
 
     /**
      * Scan an html file for <applet> tags
      */
     public static void parse(int identifier, long handle, Reader in, URL url, String enc)
         throws IOException {
         encoding = enc;
         parse(identifier, handle, in, url, System.out, new PluginAppletViewerFactory());
     }
 
     public static void parse(int identifier, long handle, Reader in, URL url)
         throws IOException {
    	 final int fIdentifier = identifier;
    	 final long fHandle = handle;
    	 final Reader fIn = in;
    	 final URL fUrl = url;
    	 PrivilegedAction pa = new PrivilegedAction() {
    		 public Object run() {
    			 try {
    				 parse(fIdentifier, fHandle, fIn, fUrl, System.out, new PluginAppletViewerFactory());
    			 } catch (IOException ioe) {
    				 return ioe;
    			 }
    	         
    			 return null;
    		 }
    	 };

    	 Object ret = AccessController.doPrivileged(pa);
    	 if (ret instanceof IOException) {
    		 throw (IOException) ret;
    	 }
     }
 
     public static void parse(int identifier, long handle, Reader in, URL url,
                              PrintStream statusMsgStream,
                              PluginAppletViewerFactory factory)
         throws IOException
     {
    	 // <OBJECT> <EMBED> tag flags
    	 boolean isAppletTag = false;
    	 boolean isObjectTag = false;
    	 boolean isEmbedTag = false;

    	 // warning messages
    	 String requiresNameWarning = amh.getMessage("parse.warning.requiresname");
    	 String paramOutsideWarning = amh.getMessage("parse.warning.paramoutside");
    	 String appletRequiresCodeWarning = amh.getMessage("parse.warning.applet.requirescode");
    	 String appletRequiresHeightWarning = amh.getMessage("parse.warning.applet.requiresheight");
    	 String appletRequiresWidthWarning = amh.getMessage("parse.warning.applet.requireswidth");
    	 String objectRequiresCodeWarning = amh.getMessage("parse.warning.object.requirescode");
    	 String objectRequiresHeightWarning = amh.getMessage("parse.warning.object.requiresheight");
    	 String objectRequiresWidthWarning = amh.getMessage("parse.warning.object.requireswidth");
    	 String embedRequiresCodeWarning = amh.getMessage("parse.warning.embed.requirescode");
    	 String embedRequiresHeightWarning = amh.getMessage("parse.warning.embed.requiresheight");
    	 String embedRequiresWidthWarning = amh.getMessage("parse.warning.embed.requireswidth");
    	 String appNotLongerSupportedWarning = amh.getMessage("parse.warning.appnotLongersupported");

    	 java.net.URLConnection conn = url.openConnection();
    	 /* The original URL may have been redirected - this
    	  * sets it to whatever URL/codebase we ended up getting
    	  */
    	 url = conn.getURL();

    	 int ydisp = 1;
    	 Hashtable atts = null;

    	 while(true) {
    		 c = in.read();
    		 if (c == -1)
    			 break;

    		 if (c == '<') {
    			 c = in.read();
    			 if (c == '/') {
    				 c = in.read();
    				 String nm = scanIdentifier(in);
    				 if (nm.equalsIgnoreCase("applet") ||
    						 nm.equalsIgnoreCase("object") ||
    						 nm.equalsIgnoreCase("embed")) {

    					 // We can't test for a code tag until </OBJECT>
    					 // because it is a parameter, not an attribute.
    					 if(isObjectTag) {
    						 if (atts.get("code") == null && atts.get("object") == null) {
    							 statusMsgStream.println(objectRequiresCodeWarning);
    							 atts = null;
    						 }
    					 }

    					 if (atts != null) {
    						 // XXX 5/18 In general this code just simply
    						 // shouldn't be part of parsing.  It's presence
    						 // causes things to be a little too much of a
    						 // hack.
    						 factory.createAppletViewer(identifier, handle, x, y, url, atts);
    						 x += XDELTA;
    						 y += YDELTA;
    						 // make sure we don't go too far!
    						 Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
    						 if ((x > d.width - 300) || (y > d.height - 300)) {
    							 x = 0;
    							 y = 2 * ydisp * YDELTA;
    							 ydisp++;
    						 }
    					 }
    					 atts = null;
    					 isAppletTag = false;
    					 isObjectTag = false;
    					 isEmbedTag = false;
    				 }
    			 }
    			 else {
    				 String nm = scanIdentifier(in);
    				 if (nm.equalsIgnoreCase("param")) {
    					 Hashtable t = scanTag(in);
    					 String att = (String)t.get("name");
    					 if (att == null) {
    						 statusMsgStream.println(requiresNameWarning);
    					 } else {
    						 String val = (String)t.get("value");
    						 if (val == null) {
    							 statusMsgStream.println(requiresNameWarning);
    						 } else if (atts != null) {
    							 // to prevent headaches, c++ side encodes &, < and >
    							 // decode them back
    							 val = val.replace("&gt;", ">");
    							 val = val.replace("&lt;", "<");
    							 val = val.replace("&amp;", "&");
    							 atts.put(att.toLowerCase(), val);
    						 } else {
    							 statusMsgStream.println(paramOutsideWarning);
    						 }
    					 }
    				 }
    				 else if (nm.equalsIgnoreCase("applet")) {
    					 isAppletTag = true;
    					 atts = scanTag(in);

    					 // If there is a classid present, transform it to code tag
    					 if (atts.get("code") == null && atts.get("classid") != null && 
    							 ((String) atts.get("classid")).startsWith("java:")) {
    						 //skip "java:"
    						 atts.put("code", ((String) atts.get("classid")).substring(5));
    					 }

    					 if (atts.get("code") == null && atts.get("object") == null) {
    						 statusMsgStream.println(appletRequiresCodeWarning);
    						 atts = null;
    					 }

    					 if (atts.get("width") == null) {
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", 100);
    					 } else if (((String) atts.get("width")).endsWith("%")) {
    						 String w = (String) atts.get("width");
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", Integer.parseInt((w.substring(0,  w.length() -1))));
    					  }

    					 if (atts.get("height") == null) {
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", 100);
    					 } else if (((String) atts.get("height")).endsWith("%")) {
    						 String h = (String) atts.get("height");
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", Integer.parseInt(h.substring(0,  h.length() -1)));
    					 }
    				 }
    				 else if (nm.equalsIgnoreCase("object")) {
    					 isObjectTag = true;
    					 atts = scanTag(in);

    					 // If there is a classid present, transform it to code tag
    					 if (atts.get("code") == null && atts.get("classid") != null && 
    							 ((String) atts.get("classid")).startsWith("java:")) {
    						 //skip "java:"
    						 atts.put("code", ((String) atts.get("classid")).substring(5));
    					 }

    					 // The <OBJECT> attribute codebase isn't what
    					 // we want when not dealing with jars. If its 
    					 // defined, remove it in that case.
    					 if(atts.get("archive") == null && atts.get("codebase") != null) {
    						 atts.remove("codebase");
    					 }

    					 if (atts.get("width") == null) {
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", 100);
    					 } else if (((String) atts.get("width")).endsWith("%")) {
    						 String w = (String) atts.get("width");
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", Integer.parseInt(w.substring(0,  w.length() -1)));
    					 }

    					 if (atts.get("height") == null) {
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", 100);
    					 } else if (((String) atts.get("height")).endsWith("%")) {
    						 String h = (String) atts.get("height");
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", Integer.parseInt(h.substring(0,  h.length() -1)));
    					 }
    				 }
    				 else if (nm.equalsIgnoreCase("embed")) {
    					 isEmbedTag = true;
    					 atts = scanTag(in);

    					 // If there is a classid present, transform it to code tag
    					 if (atts.get("code") == null && atts.get("classid") != null && 
    							 ((String) atts.get("classid")).startsWith("java:")) {
    						 //skip "java:"
    						 atts.put("code", ((String) atts.get("classid")).substring(5));
    					 }

    					 if (atts.get("code") == null && atts.get("object") == null) {
    						 statusMsgStream.println(embedRequiresCodeWarning);
    						 atts = null;
    					 }
    					 
    					 if (atts.get("width") == null) {
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", 100);
    					 } else if (((String) atts.get("width")).endsWith("%")) {
    						 String w = (String) atts.get("width");
    						 atts.put("width", "100");
    						 atts.put("widthPercentage", Integer.parseInt(w.substring(0,  w.length() -1)));
    					 }

    					 if (atts.get("height") == null) {
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", 100);
    					 } else if (((String) atts.get("height")).endsWith("%")) {
    						 String h = (String) atts.get("height");
    						 atts.put("height", "100");
    						 atts.put("heightPercentage", Integer.parseInt(h.substring(0,  h.length() -1)));
    					 }
    				 }
    				 else if (nm.equalsIgnoreCase("app")) {
    					 statusMsgStream.println(appNotLongerSupportedWarning);
    					 Hashtable atts2 = scanTag(in);
    					 nm = (String)atts2.get("class");
    					 if (nm != null) {
    						 atts2.remove("class");
    						 atts2.put("code", nm + ".class");
    					 }
    					 nm = (String)atts2.get("src");
    					 if (nm != null) {
    						 atts2.remove("src");
    						 atts2.put("codebase", nm);
    					 }
    					 if (atts2.get("width") == null) {
    						 atts2.put("width", "100");
    						 atts2.put("widthPercentage", 100);
    					 } else if (((String) atts.get("width")).endsWith("%")) {
    						 String w = (String) atts.get("width");
    						 atts2.put("width", "100");
    						 atts2.put("widthPercentage", Integer.parseInt(w.substring(0,  w.length() -1)));
    					 }
    					 
    					 if (atts2.get("height") == null) {
    						 atts2.put("height", "100");
    						 atts2.put("heightPercentage", 100);
    					 } else if (((String) atts.get("height")).endsWith("%")) {
    						 String h = (String) atts.get("height");
    						 atts2.put("height", "100");
    						 atts2.put("heightPercentage", Integer.parseInt(h.substring(0,  h.length() -1)));
    					 }

    					 printTag(statusMsgStream, atts2);
    					 statusMsgStream.println();
    				 }
    			 }
    		 }
    	 }
    	 in.close();
     }
 

     private static AppletMessageHandler amh = new AppletMessageHandler("appletviewer");
 
     private static void checkConnect(URL url)
     {
 	SecurityManager security = System.getSecurityManager();
 	if (security != null) {
 	    try {
 		java.security.Permission perm =
 		    url.openConnection().getPermission();
 		if (perm != null)
 		    security.checkPermission(perm);
 		else
 		    security.checkConnect(url.getHost(), url.getPort());
 	    } catch (java.io.IOException ioe) {
 		    security.checkConnect(url.getHost(), url.getPort());
 	    }
 	}
     }
 }
