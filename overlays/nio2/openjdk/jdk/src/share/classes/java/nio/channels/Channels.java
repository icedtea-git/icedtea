/*
 * Copyright 2000-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2009 Red Hat, Inc.
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

package org.classpath.icedtea.java.nio.channels;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.nio.charset.UnsupportedCharsetException;
import java.nio.channels.spi.AbstractInterruptibleChannel;
import java.util.concurrent.ExecutionException;
import sun.nio.ch.ChannelInputStream;
import sun.nio.cs.StreamDecoder;
import sun.nio.cs.StreamEncoder;


/**
 * Utility methods for channels and streams.
 *
 * <p> This class defines static methods that support the interoperation of the
 * stream classes of the <tt>{@link java.io}</tt> package with the channel
 * classes of this package.  </p>
 *
 *
 * @author Mark Reinhold
 * @author Mike McCloskey
 * @author JSR-51 Expert Group
 * @since 1.4
 */

public final class Channels {

    private Channels() { }              // No instantiation

    /**
     * {@note new}
     * Constructs a stream that reads bytes from the given channel.
     *
     * <p> The stream will not be buffered, and it will not support the {@link
     * InputStream#mark mark} or {@link InputStream#reset reset} methods.  The
     * stream will be safe for access by multiple concurrent threads.  Closing
     * the stream will in turn cause the channel to be closed.  </p>
     *
     * @param  ch
     *         The channel from which bytes will be read
     *
     * @return  A new input stream
     *
     * @since 1.7
     */
    public static InputStream newInputStream(final AsynchronousByteChannel ch) {
        return new InputStream() {

            private ByteBuffer bb = null;
            private byte[] bs = null;           // Invoker's previous array
            private byte[] b1 = null;


            public synchronized int read() throws IOException {
                if (b1 == null)
                    b1 = new byte[1];
                int n = this.read(b1);
                if (n == 1)
                    return b1[0] & 0xff;
                return -1;
            }


            public synchronized int read(byte[] bs, int off, int len)
                throws IOException
            {
                if ((off < 0) || (off > bs.length) || (len < 0) ||
                    ((off + len) > bs.length) || ((off + len) < 0)) {
                    throw new IndexOutOfBoundsException();
                } else if (len == 0)
                    return 0;

                ByteBuffer bb = ((this.bs == bs)
                                 ? this.bb
                                 : ByteBuffer.wrap(bs));
                bb.position(off);
                bb.limit(Math.min(off + len, bb.capacity()));
                this.bb = bb;
                this.bs = bs;

                boolean interrupted = false;
                try {
                    for (;;) {
                        try {
                            return ch.read(bb).get();
                        } catch (ExecutionException ee) {
                            throw new IOException(ee.getCause());
                        } catch (InterruptedException ie) {
                            interrupted = true;
                        }
                    }
                } finally {
                    if (interrupted)
                        Thread.currentThread().interrupt();
                }
            }


            public void close() throws IOException {
                ch.close();
            }
        };
    }

    /**
     * {@note new}
     * Constructs a stream that writes bytes to the given channel.
     *
     * <p> The stream will not be buffered. The stream will be safe for access
     * by multiple concurrent threads.  Closing the stream will in turn cause
     * the channel to be closed.  </p>
     *
     * @param  ch
     *         The channel to which bytes will be written
     *
     * @return  A new output stream
     *
     * @since 1.7
     */
    public static OutputStream newOutputStream(final AsynchronousByteChannel ch) {
        return new OutputStream() {

            private ByteBuffer bb = null;
            private byte[] bs = null;   // Invoker's previous array
            private byte[] b1 = null;


            public synchronized void write(int b) throws IOException {
               if (b1 == null)
                    b1 = new byte[1];
                b1[0] = (byte)b;
                this.write(b1);
            }


            public synchronized void write(byte[] bs, int off, int len)
                throws IOException
            {
                if ((off < 0) || (off > bs.length) || (len < 0) ||
                    ((off + len) > bs.length) || ((off + len) < 0)) {
                    throw new IndexOutOfBoundsException();
                } else if (len == 0) {
                    return;
                }
                ByteBuffer bb = ((this.bs == bs)
                                 ? this.bb
                                 : ByteBuffer.wrap(bs));
                bb.limit(Math.min(off + len, bb.capacity()));
                bb.position(off);
                this.bb = bb;
                this.bs = bs;

                boolean interrupted = false;
                try {
                    while (bb.remaining() > 0) {
                        try {
                            ch.write(bb).get();
                        } catch (ExecutionException ee) {
                            throw new IOException(ee.getCause());
                        } catch (InterruptedException ie) {
                            interrupted = true;
                        }
                    }
                } finally {
                    if (interrupted)
                        Thread.currentThread().interrupt();
                }
            }


            public void close() throws IOException {
                ch.close();
            }
        };
    }


}
