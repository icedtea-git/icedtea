package sun.applet;

import java.util.HashMap;

public abstract class AppletSecurityContext {

	public static PluginStreamHandler streamhandler;

	public static void setStreamhandler(PluginStreamHandler sh) {
		streamhandler = sh;
	}
	
	public abstract void handleMessage(String src, int reference, String message);
	
	public abstract void addClassLoader(String id, ClassLoader cl);
	
	public abstract void dumpStore();
	
	public abstract Object getObject(int identifier);
	
	public abstract int getIdentifier(Object o);
	
	public abstract void store(Object o);

}
