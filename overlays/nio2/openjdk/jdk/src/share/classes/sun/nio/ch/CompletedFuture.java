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

import java.util.concurrent.TimeUnit;
import java.util.concurrent.ExecutionException;
import java.io.IOException;

import org.classpath.icedtea.java.nio.channels.AsynchronousChannel;

/**
 * A Future representing the result of an I/O operation that has already
 * completed.
 */

final class CompletedFuture<V,A>
    extends AbstractFuture<V,A>
{
    private final V result;
    private final Throwable exc;

    private CompletedFuture(AsynchronousChannel channel,
                            V result,
                            Throwable exc,
                            A attachment)
    {
        super(channel, attachment);
        this.result = result;
        this.exc = exc;
    }

    @SuppressWarnings("unchecked")
    static <V,A> CompletedFuture<V,A> withResult(AsynchronousChannel channel,
                                                 V result,
                                                 A attachment)
    {
        return new CompletedFuture<V,A>(channel, result, null, attachment);
    }

    @SuppressWarnings("unchecked")
    static <V,A> CompletedFuture<V,A> withFailure(AsynchronousChannel channel,
                                                  Throwable exc,
                                                  A attachment)
    {
        // exception must be IOException or SecurityException
        if (!(exc instanceof IOException) && !(exc instanceof SecurityException))
            exc = new IOException(exc);
        return new CompletedFuture(channel, null, exc, attachment);
    }


    public V get() throws ExecutionException {
        if (exc != null)
            throw new ExecutionException(exc);
        return result;
    }


    public V get(long timeout, TimeUnit unit) throws ExecutionException {
        if (unit == null)
            throw new NullPointerException();
        if (exc != null)
            throw new ExecutionException(exc);
        return result;
    }


    public boolean isCancelled() {
        return false;
    }


    public boolean isDone() {
        return true;
    }


    public boolean cancel(boolean mayInterruptIfRunning) {
        return false;
    }


    Throwable exception() {
        return exc;
    }


    V value() {
        return result;
    }
}
