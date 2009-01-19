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

import java.util.concurrent.*;
import java.security.AccessController;
import sun.security.action.GetPropertyAction;

import org.classpath.icedtea.java.nio.channels.spi.AsynchronousChannelProvider.ThreadPoolType;

/**
 * Defines static methods to create thread pools that are configured by
 * system properties or a Map of parameters (or a combination of both).
 */

public class ThreadPool {
    private static final String PROP_PREFIX = "java.nio.channels.DefaultThreadPool.";
    private static final String INITIAL_SIZE = "initialSize";
    private static final String THREAD_FACTORY = "threadFactory";

    private static final ThreadFactory defaultThreadFactory = new ThreadFactory() {

         public Thread newThread(Runnable r) {
             Thread t = new Thread(r);
             t.setDaemon(true);
             return t;
        }
     };

    private final ThreadPoolType poolType;
    private final ExecutorService executor;
    private final int poolSize;

    private ThreadPool(ThreadPoolType poolType,
                       ExecutorService executor,
                       int poolSize)
    {
        this.poolType = poolType;
        this.executor = executor;
        this.poolSize = poolSize;
    }

    ExecutorService executor() {
        return executor;
    }

    int poolSize() {
        return poolSize;
    }

    boolean isFixedThreadPool() {
        return poolType == ThreadPoolType.FIXED;
    }

    private static class LazyInitialization {
        final static ThreadPool defaultThreadPool = createDefault();
    }

    // return the default (system-wide) thread pool
    static ThreadPool getDefault() {
        return LazyInitialization.defaultThreadPool;
    }

    // create using default settings (configured by system properties)
    static ThreadPool createDefault() {
        int initialSize = getDefaultThreadPoolInitialSize();
        if (initialSize < 0)
            initialSize = Runtime.getRuntime().availableProcessors();
        ThreadFactory threadFactory = getDefaultThreadPoolThreadFactory();
        if (threadFactory == null)
            threadFactory = defaultThreadFactory;
        ExecutorService executor =
            new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                                   Long.MAX_VALUE, TimeUnit.MILLISECONDS,
                                   new SynchronousQueue<Runnable>(),
                                   threadFactory);
        return new ThreadPool(ThreadPoolType.CACHED, executor, initialSize);
    }

    public static ThreadPool create(ThreadPoolType poolType,
                                    ExecutorService executor,
                                    int nThreads)
    {
        if (poolType == null || executor == null)
            throw new NullPointerException();
        if (poolType == ThreadPoolType.FIXED) {
            if (nThreads <= 0)
                throw new IllegalArgumentException("'nThreads' must be > 0");
        } else if (poolType == ThreadPoolType.CACHED) {
            // attempt to check if cached thread pool
            if (executor instanceof ThreadPoolExecutor) {
                int max = ((ThreadPoolExecutor)executor).getMaximumPoolSize();
                if (max == Integer.MAX_VALUE) {
                    if (nThreads < 0)
                        nThreads = Runtime.getRuntime().availableProcessors();
                } else {
                    // not a cached thread pool so ignore initial size
                    nThreads = 0;
                }
            } else {
                // some other type of thread pool
                if (nThreads < 0) nThreads = 0;
            }
        } else {
            throw new AssertionError();
        }
        return new ThreadPool(poolType, executor, nThreads);
    }

    static ThreadFactory defaultThreadFactory() {
        return defaultThreadFactory;
    }

    private static int getDefaultThreadPoolInitialSize() {
        String propName = PROP_PREFIX + INITIAL_SIZE;
        String value = AccessController.doPrivileged(new GetPropertyAction(propName));
        if (value != null) {
            try {
                return Integer.parseInt(value);
            } catch (NumberFormatException x) {
                throw new Error("Value of property '" + propName + "' is invalid: " + x);
            }
        }
        return -1;
    }

    private static ThreadFactory getDefaultThreadPoolThreadFactory() {
        String prop = AccessController.doPrivileged(new
            GetPropertyAction(PROP_PREFIX + THREAD_FACTORY));
        if (prop != null) {
            try {
                Class<?> c = Class
                    .forName(prop, true, ClassLoader.getSystemClassLoader());
                return ((ThreadFactory)c.newInstance());
            } catch (ClassNotFoundException x) {
                throw new Error(x);
            } catch (InstantiationException x) {
                throw new Error(x);
            } catch (IllegalAccessException x) {
                throw new Error(x);
            }
        }
        return null;
    }
}
