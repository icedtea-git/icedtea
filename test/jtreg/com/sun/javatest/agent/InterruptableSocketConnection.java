/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */
package com.sun.javatest.agent;

import java.io.IOException;
import java.io.InputStream;
import java.io.InterruptedIOException;
import java.net.Socket;

public class InterruptableSocketConnection extends SocketConnection {

    public InterruptableSocketConnection(Socket socket) throws IOException {
        super(socket);
    }
    public InterruptableSocketConnection(String host, int port) throws IOException {
        super(host, port);
    }

    public InputStream getInputStream() {
        return new InterruptableInputStream();
    }

    class InterruptableInputStream extends InputStream {

        public int read() throws IOException {
            byte[] b = new byte[1];
            int n = read(b);
            if (n == -1) {
                return -1;
            }
            else {
                n = 0xFF & b[0];
                return n;
            }
        }
        public int read(byte[] buffer, int offset, int count) throws IOException {
            if (count == 0)
                return 0;

            try {
                return new InterruptableReader().read(buffer, offset, count);
            }
            catch (InterruptedException ie) {
                InterruptedIOException iio =
                        new InterruptedIOException
                        ("Interrupted while waiting for agent response");
                iio.fillInStackTrace();
                throw iio;
            }
        }
        public void close() throws IOException {
            socketInput.close();
        }

        private synchronized void waitWhileReading() throws InterruptedException {
            while (reading)
                wait();
        }

        private boolean reading = false;
        private Object data;

        class InterruptableReader {
            private IOException ioe;
            private int n;

            public int read(byte[] buffer, int offset, int count)
                    throws IOException, InterruptedException {
                synchronized (InterruptableInputStream.this) {
                    ioe = null;
                    n = -1;

                    readInThread(buffer, offset, count);
                    waitWhileReading();

                    if (ioe != null) {
                        throw ioe;
                    }

                    return n;
                }
            }
            private void readInThread(byte[] buffer, int offset, int count) {
                final byte[] b = buffer;
                final int o = offset;
                final int c = count;

                Thread reader = new Thread() {
                    public void run() {
                        try {
                            n = socketInput.read(b, o, c);
                        }
                        catch (IOException io) {
                            ioe = io;
                        }
                        finally {
                            synchronized(InterruptableInputStream.this) {
                                reading = false;
                                InterruptableInputStream.this.notifyAll();
                            }
                        }
                    }
                };
                reading = true;
                reader.start();
            }
        };
    };

}
