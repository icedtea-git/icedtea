package sun.applet;

import java.security.AccessControlContext;
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
	
	public static void handleMessage(int identifier, int reference,	String src, String[] privileges, String message) {
		PluginDebug.debug(identifier + " -- " + src + " -- " + reference + " -- " + message + " CONTEXT= " + contexts.get(identifier));
		AccessControlContext callContext = null;

		privileges = privileges != null ? privileges : new String[0];
		callContext = contexts.get(identifier).getAccessControlContext(privileges, src); 

		contexts.get(identifier).handleMessage(reference, src, callContext, message);
	}
}
