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
import java.lang.reflect.*;
import java.net.*;
import java.nio.charset.Charset;
import java.util.*;

import sun.net.www.ParseUtil;

class RequestQueue {
    PluginCallRequest head = null;
    PluginCallRequest tail = null;
    private int size = 0;

    void post(PluginCallRequest request) {
        if (head == null) {
            head = tail = request;
            tail.next = null;
        } else {
            tail.next = request;
            tail = request;
            tail.next = null;
        }
        
        size++;
    }

    PluginCallRequest pop() {
        if (head == null)
            return null;

        PluginCallRequest ret = head;
        head = head.next;
        ret.next = null;
        
        size--;
        
        return ret;
    }
    
    int size() {
    	return size;
    }
}

/**
 * The main entry point into PluginAppletViewer.
 */
public class PluginMain
{
    private static BufferedReader pluginInputReader;
    private static StreamTokenizer pluginInputTokenizer;
    private static BufferedWriter pluginOutputWriter;
    // This is used in init().	Getting rid of this is desirable but depends
    // on whether the property that uses it is necessary/standard.
    public static final String theVersion = System.getProperty("java.version");

    private static RequestQueue queue = new RequestQueue();

	static LinkedList<String> readQueue = new LinkedList<String>();
	static LinkedList<String> writeQueue = new LinkedList<String>();

	static PluginMessageConsumer consumer;
	static Boolean shuttingDown = false;
	
	static final boolean redirectStreams = false;

    /**
     * The main entry point into AppletViewer.
     */
    public static void main(String args[])
	throws IOException
    {

    	if (args.length != 1) {
    		// Indicate to plugin that appletviewer is installed correctly.
    		System.exit(0);
    	}

    	int port = 0;
    	try {
    		port = Integer.parseInt(args[0]);
    	} catch (NumberFormatException e) {
    		System.err.println("Failed to parse port number: " + e);
    		System.exit(1);
    	}

    	if (redirectStreams) {
    		try {
    			File errFile = new File("/tmp/java.stderr");
    			File outFile = new File("/tmp/java.stdout");

    			System.setErr(new PrintStream(new FileOutputStream(errFile)));
    			System.setOut(new PrintStream(new FileOutputStream(outFile)));

    		} catch (Exception e) {
    			System.err.println("Unable to redirect streams");
    			e.printStackTrace();
    		}
    	}

    	// INSTALL THE SECURITY MANAGER
    	init();

    	System.err.println("Creating consumer...");
    	consumer = new PluginMessageConsumer();

    	beginListening(50007);
    }

    public PluginMain() {
    	
    	try {
    	    File errFile = new File("/tmp/java.stderr");
    	    File outFile = new File("/tmp/java.stdout");

    	    System.setErr(new PrintStream(new FileOutputStream(errFile)));
    	    System.setOut(new PrintStream(new FileOutputStream(outFile)));

    	} catch (Exception e) {
    	    System.err.println("Unable to redirect streams");
    	    e.printStackTrace();
    	}

    	// INSTALL THE SECURITY MANAGER
    	init();

    	System.err.println("Creating consumer...");
    	consumer = new PluginMessageConsumer();

		beginListening(50007);

    }

	private static void beginListening(int port) {
    	/*
    	 * Code for TCP/IP communication
    	 */ 
    	Socket socket = null;

    	try {
    		socket = new Socket("localhost", port);
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	
    	System.err.println("Socket initialized. Proceeding with start()");

		try {
	    	start(socket.getInputStream(), socket.getOutputStream());
	    	System.err.println("Streams initialized");
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
    
    public static void postMessage(String s) {

    	if (s == null || s.equals("shutdown")) {
    	    try {
    		// Close input/output channels to plugin.
    		pluginInputReader.close();
    		pluginOutputWriter.close();
    	    } catch (IOException exception) {
    		// Deliberately ignore IOException caused by broken
    		// pipe since plugin may have already detached.
    	    }
                PluginAppletSecurityContext.contexts.get(0).store.dump();
    	    System.err.println("APPLETVIEWER: exiting appletviewer");
    	    System.exit(0);
    	}

   		//PluginAppletSecurityContext.contexts.get(0).store.dump();
   		PluginDebug.debug("Plugin posted: " + s);

		System.err.println("Consuming " + s);
		consumer.consume(s);

   		PluginDebug.debug("Added to queue");
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

    public static void handleMessage(String message) throws PluginException{

    	StringTokenizer st = new StringTokenizer(message, " ");

    	String type = st.nextToken();
    	int identifier = Integer.parseInt(st.nextToken());

    	String rest = "";
    	int reference = -1;
    	String src = null;

    	String potentialReference = st.hasMoreTokens() ? st.nextToken() : "";

    	// if the next token is reference, read it, else reset "rest"
    	if (potentialReference.equals("reference")) {
    		reference = Integer.parseInt(st.nextToken());
    	} else {
    		rest += potentialReference + " ";
    	}

    	String potentialSrc = st.hasMoreTokens() ? st.nextToken() : "";

    	if (potentialSrc.equals("src")) {
    		src = st.nextToken();
    	} else {
    		rest += potentialSrc + " ";
    	}

    	while (st.hasMoreElements()) {
    		rest += st.nextToken();
    		rest += " ";
    	}

    	rest = rest.trim();

    	try {

    		System.err.println("Breakdown -- type: " + type + " identifier: " + identifier + " reference: " + reference + " src: " + src + " rest: " + rest);

    		if (rest.contains("JavaScriptGetWindow")
    				|| rest.contains("JavaScriptGetMember")
    				|| rest.contains("JavaScriptSetMember")
    				|| rest.contains("JavaScriptGetSlot")
    				|| rest.contains("JavaScriptSetSlot")
    				|| rest.contains("JavaScriptEval")
    				|| rest.contains("JavaScriptRemoveMember")
    				|| rest.contains("JavaScriptCall")
    				|| rest.contains("JavaScriptFinalize")
    				|| rest.contains("JavaScriptToString")) {
    			finishCallRequest(rest);
    			return;
    		}

    		if (type.equals("instance"))
    			PluginAppletViewer.handleMessage(identifier, reference, rest);
    		else if (type.equals("context")) {
    			PluginDebug.debug("Sending to PASC: " + identifier + "/" + reference + " and " + rest);
    			PluginAppletSecurityContext.handleMessage(identifier, src, reference, rest);
    		}
    	} catch (Exception e) {
    		throw new PluginException(identifier, reference, e);
    	}
    }

    static void start(InputStream inputstream, OutputStream outputstream)
	throws MalformedURLException, IOException
    {
	// Set up input and output pipes.  Use UTF-8 encoding.
	pluginInputReader =
	    new BufferedReader(new InputStreamReader(inputstream,
						     Charset.forName("UTF-8")));
        pluginInputTokenizer = new StreamTokenizer(pluginInputReader);
        pluginInputTokenizer.resetSyntax();
        pluginInputTokenizer.whitespaceChars('\u0000', '\u0000');
        pluginInputTokenizer.wordChars('\u0001', '\u00FF');
	pluginOutputWriter =
	    new BufferedWriter(new OutputStreamWriter
			       (outputstream, Charset.forName("UTF-8")));

	
	Thread listenerThread = new Thread() {

		public void run() {
			while (true) {

				System.err.println("Waiting for data...");

				int readChar = -1;
				// blocking read, discard first character
				try {
					readChar = pluginInputReader.read();
				} catch (IOException ioe) {
					// plugin may have detached
				}

				// if not disconnected
				if (readChar != -1) {
					String s = read();
					System.err.println("Got data, consuming " + s);
					consumer.consume(s);
				} else {
					try {
						// Close input/output channels to plugin.
						pluginInputReader.close();
						pluginOutputWriter.close();
					} catch (IOException exception) {
						// Deliberately ignore IOException caused by broken
						// pipe since plugin may have already detached.
					}
					PluginAppletSecurityContext.contexts.get(0).store.dump();
					System.err.println("APPLETVIEWER: exiting appletviewer");
					System.exit(0);
				}
			}
		}
	};
	
	listenerThread.start();
	
/*
	while(true) {
            String message = read();
            PluginDebug.debug(message);
            handleMessage(message);
            // TODO:
            // write(queue.peek());
	}
*/
    }

    public static void postCallRequest(PluginCallRequest request) {
        synchronized(queue) {
            queue.post(request);
        }
    }

    private static void finishCallRequest(String message) {
        PluginDebug.debug ("DISPATCHCALLREQUESTS 1");
        synchronized(queue) {
        PluginDebug.debug ("DISPATCHCALLREQUESTS 2");
            PluginCallRequest request = queue.pop();
            
            // make sure we give the message to the right request 
            // in the queue.. for the love of God, MAKE SURE!
            
            // first let's be efficient.. if there was only one 
            // request in queue, we're already set
            if (queue.size() != 0) {

            	int size = queue.size();
            	int count = 0;

            	while (!request.serviceable(message)) {
            		
            		// something is very wrong.. we have a message to 
            		// process, but no one to service it
            		if (count >= size) {
            			throw new RuntimeException("Unable to find processor for message " + message);
            		}
            		
        			// post request at the end of the queue
        			queue.post(request);

        			// Look at the next request
        			request = queue.pop();
        			
        			count++;
            	}
            	
            }
            
        PluginDebug.debug ("DISPATCHCALLREQUESTS 3");
            if (request != null) {
                PluginDebug.debug ("DISPATCHCALLREQUESTS 5");
                synchronized(request) {
                    request.parseReturn(message);
                    request.notifyAll();
                }
        PluginDebug.debug ("DISPATCHCALLREQUESTS 6");
        PluginDebug.debug ("DISPATCHCALLREQUESTS 7");
            }
        }
        PluginDebug.debug ("DISPATCHCALLREQUESTS 8");
    }

    /**
     * Write string to plugin.
     * 
     * @param message the message to write
     *
     * @exception IOException if an error occurs
     */
    static void write(String message)
    {

    	System.err.println("  PIPE: appletviewer wrote: " + message);
        synchronized(pluginOutputWriter) {
        	try {
        		pluginOutputWriter.write(message, 0, message.length());
        		pluginOutputWriter.write(0);
        		pluginOutputWriter.flush();
        	} catch (IOException e) {
        		// if we are shutting down, ignore write failures as 
        		// pipe may have closed
        		synchronized(shuttingDown) {
        			if (!shuttingDown) {
        				e.printStackTrace();
        			}
        		}

        		// either ways, if the pipe is broken, there is nothing 
        		// we can do anymore. Don't hang around.
        		System.err.println("Unable to write to PIPE. APPLETVIEWER exiting");        		
        		System.exit(1);
        	}
		}

		return;
    /*	
    	synchronized(writeQueue) {
            writeQueue.add(message);
            System.err.println("  PIPE: appletviewer wrote: " + message);
    	}
	*/

    }

    static boolean messageAvailable() {
    	return writeQueue.size() != 0;
    }

    static String getMessage() {
    	synchronized(writeQueue) {
			String ret = writeQueue.size() > 0 ? writeQueue.poll() : "";
    		return ret;
    	}
    }

    /**
     * Read string from plugin.
     *
     * @return the read string
     *
     * @exception IOException if an error occurs
     */
    static String read()
    {
    	try {
    		pluginInputTokenizer.nextToken();
    	} catch (IOException e) {
    		throw new RuntimeException(e);
    	}
    	String message = pluginInputTokenizer.sval;
    	PluginDebug.debug("  PIPE: appletviewer read: " + message);
    	if (message == null || message.equals("shutdown")) {
    		synchronized(shuttingDown) {
    			shuttingDown = true;
    		}
    		try {
    			// Close input/output channels to plugin.
    			pluginInputReader.close();
    			pluginOutputWriter.close();
    		} catch (IOException exception) {
    			// Deliberately ignore IOException caused by broken
    			// pipe since plugin may have already detached.
    		}
    		PluginAppletSecurityContext.contexts.get(0).store.dump();
    		System.err.println("APPLETVIEWER: exiting appletviewer");
    		System.exit(0);
    	}
    	return message;
    }
}
