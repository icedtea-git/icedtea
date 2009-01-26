/* VoidPluginCallRequest -- represent Java-to-JavaScript requests
   Copyright (C) 2008  Red Hat

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

IcedTea is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package sun.applet;

import java.util.ArrayList;
import java.util.LinkedList;

import sun.applet.AppletSecurity;

class PluginMessageConsumer {

	int MAX_WORKERS = 3;
	LinkedList<String> readQueue = new LinkedList<String>();
	ArrayList<PluginMessageHandlerWorker> workers = new ArrayList<PluginMessageHandlerWorker>();
	PluginStreamHandler streamHandler = null;

	public PluginMessageConsumer(PluginStreamHandler streamHandler) {
		
		AppletSecurity as = new AppletSecurity();
		this.streamHandler = streamHandler;

		for (int i=0; i < MAX_WORKERS; i++) {
			PluginDebug.debug("Creating worker " + i);
			PluginMessageHandlerWorker worker = new PluginMessageHandlerWorker(streamHandler, i, as);
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

		worker.interrupt();
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
