package sun.applet;

public class PluginException extends Exception {

	public PluginException (int instance, int reference, Throwable t) {
		t.printStackTrace();
		this.setStackTrace(t.getStackTrace());
		
		PluginAppletSecurityContext.contexts.get(0).store.dump();

		String message = "instance " + instance + " reference " + reference + " Error " + t.getMessage();
		PluginMain.write(message);
	}
}
