package sun.applet;

import java.util.ArrayList;

class PluginMessageConsumer {

	int MAX_WORKERS = 3;
	ArrayList<PluginMessageHandlerWorker> workers = new ArrayList<PluginMessageHandlerWorker>();

	public PluginMessageConsumer() {
		for (int i=0; i < MAX_WORKERS; i++) {
			System.err.println("Creating worker " + i);
			PluginMessageHandlerWorker worker = new PluginMessageHandlerWorker(i);
			worker.start();
			workers.add(worker);
		}
	}

	public void consume(String message) {
		
		PluginDebug.debug("Consumer received message " + message);
		
		synchronized(PluginMain.readQueue) {
			PluginMain.readQueue.add(message);
		}

		PluginDebug.debug("Message " + message + " added to queue. Looking for free worker...");
		PluginMessageHandlerWorker worker = getFreeWorker();
		worker.interrupt();
	}

	private PluginMessageHandlerWorker getFreeWorker() {
		
		// FIXME: Can be made more efficient by having an idle worker pool
		
		while (true) {
			for (PluginMessageHandlerWorker worker: workers) {
				if (worker.isFree()) {
					PluginDebug.debug("Found free worker with id " + worker.getWorkerId());
					return worker;
				}
			}
			Thread.yield();
		}

		//throw new RuntimeException("Out of message handler workers");
	}
	
}
