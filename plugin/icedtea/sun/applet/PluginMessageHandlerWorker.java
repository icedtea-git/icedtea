package sun.applet;


class PluginMessageHandlerWorker extends Thread {

	private boolean free = true;
	private int id;
	private String message = null;
	private SecurityManager sm;
	PluginStreamHandler streamHandler = null;

	public PluginMessageHandlerWorker(PluginStreamHandler streamHandler, int id, SecurityManager sm) {
		this.id = id;
		this.streamHandler = streamHandler;
		this.sm = sm;
	}

	public void setmessage(String message) {
		this.message = message;
	}

	public void run() {
		while (true) {

			if (message != null) {
				
				// ideally, whoever returns things object should mark it 
				// busy first, but just in case..
				busy();

				try {
					streamHandler.handleMessage(message);
				} catch (PluginException pe) {
					/*
					   catch the exception and DO NOTHING. The plugin should take over after 
					   this error and let the user know. We don't quit because otherwise the 
					   exception will spread to the rest of the applets which is a no-no
					 */ 
				}

				this.message = null;
			} else {
				
				// Sleep when there is nothing to do
				try {
					Thread.sleep(Integer.MAX_VALUE);
					System.err.println("Consumer thread " + id + " sleeping...");
				} catch (InterruptedException ie) {
					System.err.println("Consumer thread " + id + " woken...");
					// nothing.. someone woke us up, see if there 
					// is work to do
				}
			}
			
			// mark ourselves free again
			free();
		}
	}
	
	
	
	public int getWorkerId() {
		return id;
	}

	public void busy() {
		this.free = false;
	}

	
	public void free() {
		this.free = true;
	}
	
	public boolean isFree() {
		return free;
	}
}
