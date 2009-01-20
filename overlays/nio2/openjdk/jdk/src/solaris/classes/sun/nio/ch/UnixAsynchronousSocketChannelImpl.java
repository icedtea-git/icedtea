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
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA conne02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

package sun.nio.ch;

import java.nio.ByteBuffer;
import java.nio.channels.AlreadyConnectedException;
import java.nio.channels.AsynchronousCloseException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.ConnectionPendingException;
import java.net.*;
import java.util.concurrent.*;
import java.io.IOException;
import java.io.FileDescriptor;
import java.security.AccessController;
import java.security.PrivilegedAction;
import sun.security.action.GetBooleanAction;

import org.classpath.icedtea.java.nio.channels.CompletionHandler;
import org.classpath.icedtea.java.nio.channels.InterruptedByTimeoutException;
import org.classpath.icedtea.java.nio.channels.ShutdownChannelGroupException;

/**
 * Unix implementation of AsynchronousSocketChannel
 */

class UnixAsynchronousSocketChannelImpl
    extends AsynchronousSocketChannelImpl implements Port.PollableChannel
{
    private final static NativeDispatcher nd = new SocketDispatcher();
    private static enum OpType { CONNECT, READ, WRITE };

    private static final boolean DISABLE_SYNCHRONOUS_READ;
    static {
        PrivilegedAction<Boolean> pa =
            new GetBooleanAction("sun.nio.ch.disableSynchronousRead");
        DISABLE_SYNCHRONOUS_READ = AccessController.doPrivileged(pa).booleanValue();
    }

    private final Port port;
    private final int fdVal;

    UnixAsynchronousSocketChannelImpl(Port port)
        throws IOException
    {
        super(port);

        // set non-blocking
        try {
            IOUtil.configureBlocking(fd, false);
        } catch (IOException x) {
            nd.close(fd);
            throw x;
        }

        this.port = port;
        this.fdVal = IOUtil.fdVal(fd);

        // add mapping from file descriptor to this channel
        port.register(fdVal, this);
    }

    // Constructor for sockets created by UnixAsynchronousServerSocketChannelImpl
    UnixAsynchronousSocketChannelImpl(Port port,
                                      FileDescriptor fd,
                                      InetSocketAddress remote)
        throws IOException
    {
        super(port, fd, remote);

        this.fdVal = IOUtil.fdVal(fd);
        IOUtil.configureBlocking(fd, false);

        try {
            port.register(fdVal, this);
        } catch (ShutdownChannelGroupException x) {
            // ShutdownChannelGroupException thrown if we attempt to register a
            // new channel after the group is shutdown
            throw new IOException(x);
        }

        this.port = port;
    }


    public AsynchronousChannelGroupImpl group() {
        return port;
    }

    // lock used when updating interest events for this channel
    private final Object updateLock = new Object();

    // pending results for each operation type.
    private PendingFuture<Void,Object> pendingConnect;
    private PendingFuture<Number,Object> pendingRead;
    private PendingFuture<Number,Object> pendingWrite;

    private void updateEvents() {
        synchronized (updateLock) {
            int events = 0;
            if (pendingRead != null)
                events |= Port.POLLIN;
            if (pendingConnect != null || pendingWrite != null)
                events |= Port.POLLOUT;
            if (events != 0)
                port.startPoll(fdVal, events);
        }
    }

    /**
     * Invoked by event handler thread when file descriptor is polled
     */

    public void onEvent(int events) {
        boolean readable = (events & Port.POLLIN) > 0;
        boolean writable = (events & Port.POLLOUT) > 0;
        if ((events & (Port.POLLERR | Port.POLLHUP)) > 0) {
            readable = true;
            writable = true;
        }

        PendingFuture<Void,Object> connectResult = null;
        PendingFuture<Number,Object> readResult = null;
        PendingFuture<Number,Object> writeResult = null;

        // map event to pending result
        synchronized (updateLock) {
            if (readable && (pendingRead != null)) {
                readResult = pendingRead;
                pendingRead = null;
            }
            if (writable) {
                if (pendingWrite != null) {
                    writeResult = pendingWrite;
                    pendingWrite = null;
                } else if (pendingConnect != null) {
                    connectResult = pendingConnect;
                    pendingConnect = null;
                }
            }
        }

        // complete the I/O operation. Special case for when channel is
        // ready for both reading and writing. In that case, submit task to
        // complete write if write operation has a completion handler.
        if (readResult != null) {
            if (writeResult != null)
                finishWrite(writeResult, false);
            finishRead(readResult, true);
            return;
        }
        if (writeResult != null) {
            finishWrite(writeResult, true);
        }
        if (connectResult != null) {
            finishConnect(connectResult, true);
        }
    }

    // invoked by connect when connection could not be established immediately
    void setPendingConnect(PendingFuture<Void,Object> result)
        throws IOException
    {
        synchronized (updateLock) {
            pendingConnect = result;
        }
        updateEvents();
    }

    // invoked by read when there is no data to read
    void setPendingRead(PendingFuture<Number,Object> result)
        throws IOException
    {
        synchronized (updateLock) {
            pendingRead = result;
        }
        updateEvents();
    }

    // invoked by write when socket buffer is full and write cannot complete
    // immediately
    void setPendingWrite(PendingFuture<Number,Object> result)
        throws IOException
    {
        synchronized (updateLock) {
            pendingWrite = result;
        }
        updateEvents();
    }

    // returns and clears the result of a pending read
    PendingFuture<Number,Object> grabPendingRead() {
        synchronized (updateLock) {
            PendingFuture<Number,Object> result = pendingRead;
            pendingRead = null;
            return result;
        }
    }

    // returns and clears the result of a pending write
    PendingFuture<Number,Object> grabPendingWrite() {
        synchronized (updateLock) {
            PendingFuture<Number,Object> result = pendingWrite;
            pendingWrite = null;
            return result;
        }
    }


    void implClose() throws IOException {
        // remove the mapping
        port.unregister(fdVal);

        // close file descriptor
        nd.close(fd);

        // All outstanding I/O operations are required to fail
        final PendingFuture<Void,Object> readyToConnect;
        final PendingFuture<Number,Object> readyToRead;
        final PendingFuture<Number,Object> readyToWrite;
        synchronized (updateLock) {
            readyToConnect = pendingConnect;
            pendingConnect = null;
            readyToRead = pendingRead;
            pendingRead = null;
            readyToWrite = pendingWrite;
            pendingWrite = null;
        }
        if (readyToConnect != null) {
            finishConnect(readyToConnect, false);
        }
        if (readyToRead != null) {
            finishRead(readyToRead, false);
        }
        if (readyToWrite != null) {
            finishWrite(readyToWrite, false);
        }
    }


    public void onCancel(PendingFuture<?,?> task) {
        if (task.getContext() == OpType.CONNECT)
            killConnect();
        if (task.getContext() == OpType.READ)
            killConnect();
        if (task.getContext() == OpType.WRITE)
            killConnect();
    }

    // -- connect --

    // need stateLock to access
    private SocketAddress pendingRemote;

    private void setConnected() throws IOException {
        synchronized (stateLock) {
            state = ST_CONNECTED;
            localAddress = Net.localAddress(fd);
            remoteAddress = pendingRemote;
        }
    }

    private void finishConnect(PendingFuture<Void,Object> result,
                               boolean invokeDirect)
    {
        Throwable e = null;
        try {
            begin();
            checkConnect(fdVal);
            setConnected();
            result.setResult(null);
        } catch (Throwable x) {
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            e = x;
        } finally {
            end();
        }
        if (e != null) {
            // close channel if connection cannot be established
            try {
                close();
            } catch (IOException ignore) { }
            result.setFailure(e);
        }
        if (invokeDirect) {
            Invoker.invoke(result.handler(), result);
        } else {
            Invoker.invokeIndirectly(result.handler(), result);
        }
    }


    @SuppressWarnings("unchecked")
    public <A> Future<Void> connect(SocketAddress remote,
                                    A attachment,
                                    CompletionHandler<Void,? super A> handler)
    {
        if (!isOpen()) {
            CompletedFuture<Void,A> result = CompletedFuture
                .withFailure(this, new ClosedChannelException(), attachment);
            Invoker.invoke(handler, result);
            return result;
        }

        InetSocketAddress isa = Net.checkAddress(remote);

        // permission check
        SecurityManager sm = System.getSecurityManager();
        if (sm != null)
            sm.checkConnect(isa.getAddress().getHostAddress(), isa.getPort());

        // check and set state
        synchronized (stateLock) {
            if (state == ST_CONNECTED)
                throw new AlreadyConnectedException();
            if (state == ST_PENDING)
                throw new ConnectionPendingException();
            state = ST_PENDING;
            pendingRemote = remote;
        }

        AbstractFuture<Void,A> result = null;
        Throwable e = null;
        try {
            begin();
            int n = Net.connect(fd, isa.getAddress(), isa.getPort());
            if (n == IOStatus.UNAVAILABLE) {
                // connection could not be established immediately
                result = new PendingFuture<Void,A>(this, handler, attachment, OpType.CONNECT);
                setPendingConnect((PendingFuture<Void,Object>)result);
                return result;
            }
            setConnected();
            result = CompletedFuture.withResult(this, null, attachment);
        } catch (Throwable x) {
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            e = x;
        } finally {
            end();
        }

        // close channel if connect fails
        if (e != null) {
            try {
                close();
            } catch (IOException ignore) { }
            result = CompletedFuture.withFailure(this, e, attachment);
        }

        Invoker.invoke(handler, result);
        return result;
    }

    // -- read --

    // buffers used for the read
    private volatile ByteBuffer[] readBuffers;
    // true if the read is a scattering read (multiple buffers)
    private volatile boolean scatteringRead;

    @SuppressWarnings("unchecked")
    private void finishRead(PendingFuture<Number,Object> result,
                            boolean invokeDirect)
    {
        int n = -1;
        try {
            begin();

            ByteBuffer[] dsts = readBuffers;
            if (dsts.length == 1) {
                n = IOUtil.read(fd, dsts[0], -1, nd, null);
            } else {
                n = (int)IOUtil.read(fd, dsts, nd);
            }
            if (n == IOStatus.UNAVAILABLE) {
                // spurious wakeup, is this possible?
                setPendingRead(result);
                return;
            }

            // allow buffer(s) to be GC'ed.
            readBuffers = null;

            // allow another read to be initiated
            boolean wasScatteringRead = scatteringRead;
            enableReading();

            // result is Integer or Long
            if (wasScatteringRead) {
                result.setResult(Long.valueOf(n));
            } else {
                result.setResult(Integer.valueOf(n));
            }

        } catch (Throwable x) {
            enableReading();
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            result.setFailure(x);
        } finally {
            updateEvents();
            end();
        }

        if (invokeDirect) {
            Invoker.invoke(result.handler(), result);
        } else {
            Invoker.invokeIndirectly(result.handler(), result);
        }
    }

    private Runnable readTimeoutTask = new Runnable() {
        public void run() {
            PendingFuture<Number,Object> result = grabPendingRead();
            if (result == null)
                return;     // already completed

            // kill further reading before releasing waiters
            enableReading(true);

            // set completed and invoke handler
            result.setFailure(new InterruptedByTimeoutException());
            Invoker.invokeIndirectly(result.handler(), result);
        }
    };

    /**
     * Initiates a read or scattering read operation
     */

    @SuppressWarnings("unchecked")
    <V extends Number,A> Future<V> readImpl(ByteBuffer[] dsts,
                                            boolean isScatteringRead,
                                            long timeout,
                                            TimeUnit unit,
                                            A attachment,
                                            CompletionHandler<V,? super A> handler)
    {
        // A synchronous read is not attempted if disallowed by system property
        // or, we are using a fixed thread pool and the completion handler may
        // not be invoked directly (because the thread is not a pooled thread or
        // there are too many handlers on the stack).
        Invoker.GroupAndInvokeCount myGroupAndInvokeCount = null;
        boolean invokeDirect = false;
        boolean attemptRead = false;
        if (!DISABLE_SYNCHRONOUS_READ) {
            myGroupAndInvokeCount = Invoker.getGroupAndInvokeCount();
            invokeDirect = Invoker.mayInvokeDirect(myGroupAndInvokeCount, port);
            attemptRead = (handler == null) || invokeDirect ||
                !port.isFixedThreadPool();  // okay to attempt read when cached thread pool
        }

        AbstractFuture<V,A> result;
        try {
            begin();

            int n;
            if (attemptRead) {
                if (isScatteringRead) {
                    n = (int)IOUtil.read(fd, dsts, nd);
                } else {
                    n = IOUtil.read(fd, dsts[0], -1, nd, null);
                }
            } else {
                n = IOStatus.UNAVAILABLE;
            }

            if (n == IOStatus.UNAVAILABLE) {
                this.readBuffers = dsts;
                this.scatteringRead = isScatteringRead;
                result = new PendingFuture<V,A>(this, handler, attachment, OpType.READ);
                setPendingRead((PendingFuture<Number,Object>)result);

                // schedule timeout
                if (timeout > 0L) {
                    Future<?> timeoutTask =
                        port.schedule(readTimeoutTask, timeout, unit);
                    ((PendingFuture<V,A>)result).setTimeoutTask(timeoutTask);
                }
                return result;
            }

            // data available
            enableReading();

            // result type is Long or Integer
            if (isScatteringRead) {
                result = (CompletedFuture<V,A>)CompletedFuture
                    .withResult(this, Long.valueOf(n), attachment);
            } else {
                result = (CompletedFuture<V,A>)CompletedFuture
                    .withResult(this, Integer.valueOf(n), attachment);
            }
        } catch (Throwable x) {
            enableReading();
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            result = CompletedFuture.withFailure(this, x, attachment);
        } finally {
            end();
        }

        if (invokeDirect) {
            Invoker.invokeDirect(myGroupAndInvokeCount, handler, result);
        } else {
            Invoker.invokeIndirectly(handler, result);
        }
        return result;
    }

    // -- write --

    private volatile ByteBuffer[] writeBuffers;
    private volatile boolean gatheringWrite;

    private void finishWrite(PendingFuture<Number,Object> result,
                             boolean invokeDirect)
    {
        try {
            begin();

            ByteBuffer[] srcs = writeBuffers;
            int n;
            if (srcs.length == 1) {
                n = IOUtil.write(fd, srcs[0], -1, nd, null);
            } else {
                n = (int)IOUtil.write(fd, srcs, nd);
            }
            if (n == IOStatus.UNAVAILABLE) {
                // spurious wakeup, is this possible?
                setPendingWrite(result);
                return;
            }

            // allow buffer(s) to be GC'ed.
            writeBuffers = null;

            // allow another write to be initiated
            boolean wasGatheringWrite = gatheringWrite;
            enableWriting();

            // result is a Long or Integer
            if (wasGatheringWrite) {
                result.setResult(Long.valueOf(n));
            } else {
                result.setResult(Integer.valueOf(n));
            }

        } catch (Throwable x) {
            enableWriting();
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            result.setFailure(x);
        } finally {
            updateEvents();
            end();
        }
        if (invokeDirect) {
            Invoker.invoke(result.handler(), result);
        } else {
            Invoker.invokeIndirectly(result.handler(), result);
        }
    }

    private Runnable writeTimeoutTask = new Runnable() {
        public void run() {
            PendingFuture<Number,Object> result = grabPendingWrite();
            if (result == null)
                return;     // already completed

            // kill further writing before releasing waiters
            enableWriting(true);

            // set completed and invoke handler
            result.setFailure(new InterruptedByTimeoutException());
            Invoker.invokeIndirectly(result.handler(), result);
        }
    };


    /**
     * Initiates a read or scattering read operation
     */

    @SuppressWarnings("unchecked")
    <V extends Number,A> Future<V> writeImpl(ByteBuffer[] srcs,
                                             boolean isGatheringWrite,
                                             long timeout,
                                             TimeUnit unit,
                                             A attachment,
                                             CompletionHandler<V,? super A> handler)
    {
        Invoker.GroupAndInvokeCount myGroupAndInvokeCount =
            Invoker.getGroupAndInvokeCount();
        boolean invokeDirect = Invoker.mayInvokeDirect(myGroupAndInvokeCount, port);
        boolean attemptWrite = (handler == null) || invokeDirect ||
            !port.isFixedThreadPool();  // okay to attempt read when cached thread pool

        AbstractFuture<V,A> result;
        try {
            begin();

            int n;
            if (attemptWrite) {
                if (isGatheringWrite) {
                    n = (int)IOUtil.write(fd, srcs, nd);
                } else {
                    n = IOUtil.write(fd, srcs[0], -1, nd, null);
                }
            } else {
                n = IOStatus.UNAVAILABLE;
            }

            if (n == IOStatus.UNAVAILABLE) {
                this.writeBuffers = srcs;
                this.gatheringWrite = isGatheringWrite;
                result = new PendingFuture<V,A>(this, handler, attachment, OpType.WRITE);
                setPendingWrite((PendingFuture<Number,Object>)result);

                // schedule timeout
                if (timeout > 0L) {
                    Future<?> timeoutTask =
                        port.schedule(writeTimeoutTask, timeout, unit);
                    ((PendingFuture<V,A>)result).setTimeoutTask(timeoutTask);
                }
                return result;
            }

            // data available
            enableWriting();
            if (isGatheringWrite) {
                result = (CompletedFuture<V,A>)CompletedFuture
                    .withResult(this, Long.valueOf(n), attachment);
            } else {
                result = (CompletedFuture<V,A>)CompletedFuture
                    .withResult(this, Integer.valueOf(n), attachment);
            }
        } catch (Throwable x) {
            enableWriting();
            if (x instanceof ClosedChannelException)
                x = new AsynchronousCloseException();
            result = CompletedFuture.withFailure(this, x, attachment);
        } finally {
            end();
        }
        if (invokeDirect) {
            Invoker.invokeDirect(myGroupAndInvokeCount, handler, result);
        } else {
            Invoker.invokeIndirectly(handler, result);
        }
        return result;
    }

    // -- Native methods --

    private static native void checkConnect(int fdVal) throws IOException;

    static {
        Util.load();
    }
}
