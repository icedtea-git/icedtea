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

import java.nio.channels.AsynchronousCloseException;
import java.nio.channels.ClosedChannelException;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.locks.*;
import java.io.FileDescriptor;
import java.io.IOException;

import org.classpath.icedtea.java.nio.channels.AsynchronousFileChannel;
import org.classpath.icedtea.java.nio.channels.FileLock;

/**
 * Base implementation of AsynchronousFileChannel.
 */

abstract class AsynchronousFileChannelImpl
    extends AsynchronousFileChannel
{
    // close support
    protected final ReadWriteLock closeLock = new ReentrantReadWriteLock();
    protected volatile boolean closed;

    // file descriptor
    protected final FileDescriptor fdObj;

    // indicates if open for reading/writing
    protected final boolean reading;
    protected final boolean writing;

    // associated Executor
    protected final ExecutorService executor;

    protected AsynchronousFileChannelImpl(FileDescriptor fdObj,
                                          boolean reading,
                                          boolean writing,
                                          ExecutorService executor)
    {
        this.fdObj = fdObj;
        this.reading = reading;
        this.writing = writing;
        this.executor = executor;
    }

    final ExecutorService executor() {
        return executor;
    }


    public final boolean isOpen() {
        return !closed;
    }

    /**
     * Marks the beginning of an I/O operation.
     *
     * @throws  ClosedChannelException  If channel is closed
     */
    protected final void begin() throws IOException {
        closeLock.readLock().lock();
        if (closed)
            throw new ClosedChannelException();
    }

    /**
     * Marks the end of an I/O operation.
     */
    protected final void end() {
        closeLock.readLock().unlock();
    }

    /**
     * Marks end of I/O operation
     */
    protected final void end(boolean completed) throws IOException {
        end();
        if (!completed && !isOpen())
            throw new AsynchronousCloseException();
    }

    // -- file locking --

    private volatile FileLockTable fileLockTable;

    final void ensureFileLockTableInitialized() throws IOException {
        if (fileLockTable == null) {
            synchronized (this) {
                if (fileLockTable == null) {
                    fileLockTable = FileLockTable.newSharedFileLockTable(this, fdObj);
                }
            }
        }
    }

    final void invalidateAllLocks() {
        if (fileLockTable != null) {
            try {
                fileLockTable.removeAll( new FileLockTable.Releaser() {
                    public void release(FileLock fl) {
                        ((AsynchronousFileLockImpl)fl).invalidate();
                    }
                });
            } catch (IOException e) {
                throw new AssertionError(e);
            }
        }
    }

    /**
     * Adds region to lock table
     */
    protected final AsynchronousFileLockImpl addToFileLockTable(long position, long size, boolean shared) {
        final AsynchronousFileLockImpl fli;
        try {
            // like begin() but returns null instead of exception
            closeLock.readLock().lock();
            if (closed)
                return null;

            try {
                ensureFileLockTableInitialized();
            } catch (IOException x) {
                // should not happen
                throw new AssertionError(x);
            }
            fli = new AsynchronousFileLockImpl(this, position, size, shared);
            // may throw OverlappedFileLockException
            fileLockTable.add(fli);
        } finally {
            end();
        }
        return fli;
    }

    protected final void removeFromFileLockTable(AsynchronousFileLockImpl fli) {
        fileLockTable.remove(fli);
    }

    /**
     * Invoked by FileLockImpl to release lock acquired by this channel.
     */
    abstract void release(AsynchronousFileLockImpl fli) throws IOException;
}
