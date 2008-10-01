package sun.applet;

import java.util.HashMap;

public class AppletSecurityContextManager {

	// Context identifier -> PluginAppletSecurityContext object.
	// FIXME: make private
	private static HashMap<Integer, PluginAppletSecurityContext> contexts = new HashMap();
	
	public static void addContext(int identifier, PluginAppletSecurityContext context) {
		contexts.put(identifier, context);
	}
	
	public static PluginAppletSecurityContext getSecurityContext(int identifier) {
		return contexts.get(identifier);
	}

	public static void dumpStore(int identifier) {
		contexts.get(identifier).dumpStore();
	}
	
	public static void handleMessage(int identifier, String src, int reference,	String message) {
		System.err.println(identifier + " -- " + src + " -- " + reference + " -- " + message + " CONTEXT= " + contexts.get(identifier));
		contexts.get(identifier).handleMessage(src, reference, message);
	}
}
