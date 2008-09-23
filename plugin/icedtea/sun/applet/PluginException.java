package sun.applet;


public class PluginException extends Exception {

	public PluginException (PluginStreamHandler sh, int instance, int reference, Throwable t) {
		t.printStackTrace();
		this.setStackTrace(t.getStackTrace());
		
		AppletSecurityContextManager.dumpStore(0);

		String message = "instance " + instance + " reference " + reference + " Error " + t.getMessage();
		sh.write(message);
	}
}
