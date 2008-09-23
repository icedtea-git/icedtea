package sun.applet;

import java.io.*;

public class PluginDebug {

	static final boolean DEBUG = true; 

    public static void debug(String message) {
    	if (DEBUG)
    		System.err.println(message);
	}
}
