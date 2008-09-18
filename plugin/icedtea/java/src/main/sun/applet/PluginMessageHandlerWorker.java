package sun.applet;

class PluginMessageHandlerWorker extends Thread {

	private boolean free = true;
	private int id;
	
	public PluginMessageHandlerWorker(int id) {
		this.id = id;
	}
	
	public void run() {
		while (true) {

			String msg = null;
			synchronized(PluginMain.readQueue) {
				if (PluginMain.readQueue.size() > 0) {
					msg = PluginMain.readQueue.poll();
				}
			}
			
			if (msg != null) {
				free = false;
				System.err.println("Thread " + id + " picking up " + msg + " from queue...");

				try {
					PluginMain.handleMessage(msg);
				} catch (PluginException pe) {
					/*
					   catch the exception and DO NOTHING. The plugin should take over after 
					   this error and let the user know. We don't quit because otherwise the 
					   exception will spread to the rest of the applets which is a no-no
					 */ 
				}

				free = true;
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
		}
	}
	
	public int getWorkerId() {
		return id;
	}
	
	public boolean isFree() {
		return free;
	}
}
