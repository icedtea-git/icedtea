package sun.applet;

import java.io.*;

public class PluginDebug {

	static final boolean DEBUG = System.getenv().containsKey("ICEDTEAPLUGIN_DEBUG"); 

    public static void debug(String message) {
    	if (DEBUG)
    		System.err.println(message);
	}
}
