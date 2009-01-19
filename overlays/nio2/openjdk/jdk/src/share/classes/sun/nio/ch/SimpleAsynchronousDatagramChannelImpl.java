/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
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

package sun.nio.ch;

import java.nio.ByteBuffer;

import java.nio.channels.AsynchronousCloseException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.NotYetConnectedException;

import java.net.*;
import java.io.IOException;
import java.util.*;
import java.util.concurrent.*;
import java.security.AccessController;
import java.security.AccessControlContext;
import java.security.PrivilegedExceptionAction;
import java.security.PrivilegedActionException;

import org.classpath.icedtea.java.net.ProtocolFamily;
import org.classpath.icedtea.java.net.SocketOption;

import org.classpath.icedtea.java.nio.channels.AsynchronousDatagramChannel;
import org.classpath.icedtea.java.nio.channels.CompletionHandler;
import org.classpath.icedtea.java.nio.channels.DatagramChannel;
import org.classpath.icedtea.java.nio.channels.InterruptedByTimeoutException;
import org.classpath.icedtea.java.nio.channels.MembershipKey;
import org.classpath.icedtea.java.nio.channels.MulticastChannel;
import org.classpath.icedtea.java.nio.channels.ShutdownChannelGroupException;

/**
 * A prototype implementation of AsynchronousDatagramChannel, used to aid
 * test and spec development.
 */

class SimpleAsynchronousDatagramChannelImpl
    extends AsynchronousDatagramChannel implements Groupable
{
    private final DatagramChannel dc;
    private final AsynchronousChannelGroupImpl group;
    private final Object attachKey;
    private boolean closed;

    SimpleAsynchronousDatagramChannelImpl(ProtocolFamily family,
                                          AsynchronousChannelGroupImpl group)
        throws IOException
    {
        super(group.provider());
        this.dc = (family == null) ?
            DatagramChannel.open() : DatagramChannel.open(family);
        this.group = group;

        boolean registered = false;
        try {
            if (!(dc instanceof DatagramChannelImpl))
                throw new UnsupportedOperationException();
            attachKey = group
                .attachForeignChannel(this, ((DatagramChannelImpl)dc).getFD());
            registered = true;
        } finally {
            if (!registered)
                dc.close();
        }
    }


    public AsynchronousChannelGroupImpl group() {
        return group;
    }


    public boolean isOpen() {
        return dc.isOpen();
    }


    public void close() throws IOException {
        synchronized (dc) {
            if (closed)
                return;
            closed = true;
        }
        group.detachForeignChannel(attachKey);
        dc.close();
    }


    public AsynchronousDatagramChannel connect(SocketAddress remote)
        throws IOException
    {
        dc.connect(remote);
        return this;
    }


    public AsynchronousDatagramChannel disconnect() throws IOException {
        dc.disconnect();
        return this;
    }

    private static class WrappedMembershipKey extends MembershipKey {
        private final MulticastChannel channel;
        private final MembershipKey key;

        WrappedMembershipKey(MulticastChannel channel, MembershipKey key) {
            this.channel = channel;
            this.key = key;
        }


        public boolean isValid() {
            return key.isValid();
        }


        public void drop() throws IOException {
            key.drop();
        }


        public MulticastChannel channel() {
            return channel;
        }


        public InetAddress group() {
            return key.group();
        }


        public NetworkInterface networkInterface() {
            return key.networkInterface();
        }


        public InetAddress sourceAddress() {
            return key.sourceAddress();
        }


        public MembershipKey block(InetAddress toBlock) throws IOException {
            key.block(toBlock);
            return this;
        }


        public MembershipKey unblock(InetAddress toUnblock) throws IOException {
            key.unblock(toUnblock);
            return this;
        }


        public String toString() {
            return key.toString();
        }
    }


    public MembershipKey join(InetAddress group,
                              NetworkInterface interf)
        throws IOException
    {
        MembershipKey key = ((MulticastChannel)dc).join(group, interf);
        return new WrappedMembershipKey(this, key);
    }


    public MembershipKey join(InetAddress group,
                              NetworkInterface interf,
                              InetAddress source)
        throws IOException
    {
        MembershipKey key = ((MulticastChannel)dc).join(group, interf, source);
        return new WrappedMembershipKey(this, key);
    }


    public <A> Future<Integer> send(ByteBuffer src,
                                    SocketAddress target,
                                    long timeout,
                                    TimeUnit unit,
                                    A attachment,
                                    CompletionHandler<Integer,? super A> handler)
    {
        if (timeout < 0L)
            throw new IllegalArgumentException("Negative timeout");
        if (unit == null)
            throw new NullPointerException();

        CompletedFuture<Integer,A> result;
        try {
            // assume it will not block
            int n = dc.send(src, target);
            result = CompletedFuture.withResult(this, n, attachment);
        } catch (IOException ioe) {
            result = CompletedFuture.withFailure(this, ioe, attachment);
        }
        Invoker.invoke(handler, result);
        return result;
    }


    public <A> Future<Integer> write(ByteBuffer src,
                                     long timeout,
                                     TimeUnit unit,
                                     A attachment,
                                     CompletionHandler<Integer,? super A> handler)
    {
        if (timeout < 0L)
            throw new IllegalArgumentException("Negative timeout");
        if (unit == null)
            throw new NullPointerException();

        CompletedFuture<Integer,A> result;
        try {
            // assume it will not block
            int n = dc.write(src);
            result = CompletedFuture.withResult(this, n, attachment);
        } catch (IOException ioe) {
            result = CompletedFuture.withFailure(this, ioe, attachment);
        }
        Invoker.invoke(handler, result);
        return result;
    }

    @SuppressWarnings("unchecked")
    private <V,A> Future<?> scheduleTimeout(final PendingFuture<V,? super A> result,
                                            long timeout, TimeUnit unit)
    {
        if (timeout > 0L) {
            Runnable readTimeoutTask = new Runnable() {
                public void run() {
                    if (result.setFailure(new InterruptedByTimeoutException()))
                        Invoker.invoke(result.handler(), (AbstractFuture<V,A>)result);
                }
            };
            Future<?> timeoutTask = group.schedule(readTimeoutTask, timeout, unit);
            result.setTimeoutTask(timeoutTask);
            return timeoutTask;
        } else {
            return null;
        }
    }


    public <A> Future<SocketAddress> receive(final ByteBuffer dst,
                                             long timeout,
                                             TimeUnit unit,
                                             A attachment,
                                             final CompletionHandler<SocketAddress,? super A> handler)
    {
        if (dst.isReadOnly())
            throw new IllegalArgumentException("Read-only buffer");
        if (timeout < 0L)
            throw new IllegalArgumentException("Negative timeout");
        if (unit == null)
            throw new NullPointerException();

        // complete immediately if channel closed
        if (!isOpen()) {
            CompletedFuture<SocketAddress,A> result = CompletedFuture.withFailure(this,
                new ClosedChannelException(), attachment);
            Invoker.invoke(handler, result);
            return result;
        }

        final AccessControlContext acc = (System.getSecurityManager() == null) ?
            null : AccessController.getContext();
        final PendingFuture<SocketAddress,A> result =
            new PendingFuture<SocketAddress,A>(this, handler, attachment);
        Runnable task = new Runnable() {
            public void run() {
                boolean completedByMe = false;
                try {
                    SocketAddress remote;
                    if (acc == null) {
                        remote = dc.receive(dst);
                    } else {
                        // receive in caller context
                        try {
                            remote = AccessController.doPrivileged(
                                new PrivilegedExceptionAction<SocketAddress>() {
                                    public SocketAddress run() throws IOException {
                                        return dc.receive(dst);
                                    }}, acc);
                        } catch (PrivilegedActionException pae) {
                            Exception cause = pae.getException();
                            if (cause instanceof SecurityException)
                                throw (SecurityException)cause;
                            throw (IOException)cause;
                        }
                    }
                    completedByMe = result.setResult(remote);
                } catch (Throwable x) {
                    if (x instanceof ClosedChannelException)
                        x = new AsynchronousCloseException();
                    completedByMe = result.setFailure(x);
                }
                if (completedByMe)
                    Invoker.invokeUnchecked(handler, result);
            }
        };
        Future<?> timeoutTask = scheduleTimeout(result, timeout, unit);
        try {
            group.executeOnPooledThread(task);
        } catch (RejectedExecutionException ree) {
            if (timeoutTask != null)
                timeoutTask.cancel(false);
            throw new ShutdownChannelGroupException();
        }
        return result;
    }


    public <A> Future<Integer> read(final ByteBuffer dst,
                                    long timeout,
                                    TimeUnit unit,
                                    A attachment,
                                    final CompletionHandler<Integer,? super A> handler)
    {
        if (dst.isReadOnly())
            throw new IllegalArgumentException("Read-only buffer");
        if (timeout < 0L)
            throw new IllegalArgumentException("Negative timeout");
        if (unit == null)
            throw new NullPointerException();
        // another thread may disconnect before read is initiated
        if (!dc.isConnected())
            throw new NotYetConnectedException();

        // complete immediately if channel closed
        if (!isOpen()) {
            CompletedFuture<Integer,A> result = CompletedFuture.withFailure(this,
                new ClosedChannelException(), attachment);
            Invoker.invoke(handler, result);
            return result;
        }

        final PendingFuture<Integer,A> result =
            new PendingFuture<Integer,A>(this, handler, attachment);
        Runnable task = new Runnable() {
            public void run() {
                boolean completedByMe = false;
                try {
                    int n = dc.read(dst);
                    completedByMe = result.setResult(n);
                } catch (Throwable x) {
                    if (x instanceof ClosedChannelException)
                        x = new AsynchronousCloseException();
                    completedByMe = result.setFailure(x);
                }
                if (completedByMe)
                    Invoker.invokeUnchecked(handler, result);
            }
        };
        Future<?> timeoutTask = scheduleTimeout(result, timeout, unit);
        try {
            group.executeOnPooledThread(task);
        } catch (RejectedExecutionException ree) {
            if (timeoutTask != null)
                timeoutTask.cancel(false);
            throw new ShutdownChannelGroupException();
        }
        return result;
    }


    public  AsynchronousDatagramChannel bind(SocketAddress local)
        throws IOException
    {
        dc.bind(local);
        return this;
    }


    public SocketAddress getLocalAddress() throws IOException {
        return dc.getLocalAddress();
    }


    public <T> AsynchronousDatagramChannel setOption(SocketOption<T> name, T value)
        throws IOException
    {
        dc.setOption(name, value);
        return this;
    }


    public  <T> T getOption(SocketOption<T> name) throws IOException {
        return dc.getOption(name);
    }


    public Set<SocketOption<?>> supportedOptions() {
        return dc.supportedOptions();
    }


    public SocketAddress getRemoteAddress() throws IOException {
        return dc.getRemoteAddress();
    }
}
