package org.classpath.icedtea.plugin;

import java.security.AccessController;
import java.security.PrivilegedAction;
import java.util.ArrayList;
import java.util.LinkedList;

import sun.applet.PluginDebug;
import sun.applet.PluginStreamHandler;



class PluginMessageConsumer {

	int MAX_WORKERS = 3;
	LinkedList<String> readQueue = new LinkedList<String>();
	ArrayList<PluginMessageHandlerWorker> workers = new ArrayList<PluginMessageHandlerWorker>();
	PluginStreamHandler streamHandler = null;

	public PluginMessageConsumer(PluginStreamHandler streamHandler) {
		
		this.streamHandler = streamHandler;

		for (int i=0; i < MAX_WORKERS; i++) {
			System.err.println("Creating worker " + i);
			PluginMessageHandlerWorker worker = new PluginMessageHandlerWorker(streamHandler, i);
			worker.start();
			workers.add(worker);
		}
	}

	public void consume(String message) {
		
		PluginDebug.debug("Consumer received message " + message);
		
		synchronized(readQueue) {
			readQueue.add(message);
		}

		PluginDebug.debug("Message " + message + " added to queue. Looking for free worker...");
		final PluginMessageHandlerWorker worker = getFreeWorker();

		synchronized(readQueue) {
			if (readQueue.size() > 0) {
				worker.setmessage(readQueue.poll());
			}
		}

		AccessController.doPrivileged(new PrivilegedAction() {
			public Object run () {
				worker.interrupt();
				return null;
			}
		});
	}

	private PluginMessageHandlerWorker getFreeWorker() {
		
		// FIXME: Can be made more efficient by having an idle worker pool
		
		while (true) {
			for (PluginMessageHandlerWorker worker: workers) {
				if (worker.isFree()) {
					PluginDebug.debug("Found free worker with id " + worker.getWorkerId());
					// mark it busy before returning
					worker.busy();
					return worker;
				}
			}
			Thread.yield();
		}

		//throw new RuntimeException("Out of message handler workers");
	}
	
}
