/*
 * Copyright (c) 2007-2008 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * -Redistribution of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN MIDROSYSTEMS, INC. ("SUN")
 * AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE
 * AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or intended
 * for use in the design, construction, operation or maintenance of any
 * nuclear facility.
 */

import java.nio.ByteBuffer;
import java.net.InetSocketAddress;
import java.io.IOException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;

public class EchoServer {

    // timeout for read and writes.
    static final long READ_TIMEOUT_IN_SECONDS  = 30;
    static final long WRITE_TIMEOUT_IN_SECONDS = 30;

    // the connection count
    static final AtomicInteger connectionCount = new AtomicInteger();

    /**
     * Encapsulates everything for a connection and is also the handler
     * that is invoked when read/write operations complete.
     */
    static class Connection
        implements CompletionHandler<Integer,Void>
    {
        private final AsynchronousSocketChannel channel;
        private final ByteBuffer buf;

        // true if reading; false if writing
        private volatile boolean isReading;

        private Connection(AsynchronousSocketChannel channel) {
            this.channel = channel;
            this.buf = ByteBuffer.allocateDirect(4096);
            connectionCount.incrementAndGet();
        }

        static void handle(AsynchronousSocketChannel channel) {
            new Connection(channel).startReading();
        }

        private void startReading() {
            buf.rewind();
            buf.limit(buf.capacity());
            isReading = true;
            channel.read(buf, READ_TIMEOUT_IN_SECONDS, TimeUnit.SECONDS, null, this);
        }

        private void closeChannel() {
            try {
                channel.close();
            } catch (IOException ignore) { }
            connectionCount.decrementAndGet();
        }

        // invoked when a read or write completes

        public void completed(Integer bytesTransferred, Void att) {
            int n = bytesTransferred;
            if (n < 0) {
                // EOF
                assert isReading;
                closeChannel();
                return;
            }

            // if read completed then flip buffer to write to client
            if (isReading) {
                buf.flip();
                isReading = false;
            }

            // write any remaining bytes
            if (!isReading && buf.hasRemaining()) {
                channel.write(buf, WRITE_TIMEOUT_IN_SECONDS, TimeUnit.SECONDS, null, this);
            } else {
                // nothing to write so switch back to reading
                startReading();
            }
        }

        // invoked if read or write fails

        public void failed(Throwable exc, Void att) {
            System.err.println(exc);
            closeChannel();
        }


        public void cancelled(Void att) {
            assert false;
        }
    }

    public static void main(String[] args) throws Exception {
        // single argument that is the port number
        int port = Integer.parseInt(args[0]);

        // create the listener
        final AsynchronousServerSocketChannel listener =
            AsynchronousServerSocketChannel.open().bind(new InetSocketAddress(port));

        // accept connections
        listener.accept(null, new CompletionHandler<AsynchronousSocketChannel,Void>() {

            public void completed(AsynchronousSocketChannel channel, Void att) {
                // start accepting the next connection
                listener.accept(null, this);

                // handle the new connection
                Connection.handle(channel);
            }


            public void failed(Throwable exc, Void att) {
                System.err.println(exc);
                System.exit(-1);
            }


            public void cancelled(Void att) {
            }
        });

        // keep the main thread busy reporting the connection count
        for (;;) {
            Thread.sleep(2000);
            System.out.println(connectionCount.get());
        }
    }
}
