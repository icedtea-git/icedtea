package sun.applet;


public interface PluginStreamHandler {

	public void postMessage(String s);

	public void handleMessage(String message) throws PluginException;

	public void postCallRequest(PluginCallRequest request);

	public void write(String message);

	public boolean messageAvailable();

	public String getMessage();
	
	public void startProcessing();

}