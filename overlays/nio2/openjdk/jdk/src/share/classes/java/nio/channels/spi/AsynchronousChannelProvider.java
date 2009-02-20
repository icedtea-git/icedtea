/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * 
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

package java.nio.channels.spi;

import java.nio.channels.*;
import java.io.IOException;
import java.util.Iterator;
import java.util.ServiceLoader;
import java.util.ServiceConfigurationError;
import java.util.concurrent.*;
import java.security.AccessController;
import java.security.PrivilegedAction;

import java.net.ProtocolFamily;

import java.nio.channels.AsynchronousChannelGroup;
import java.nio.channels.AsynchronousDatagramChannel;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;

/**
 * Service-provider class for asynchronous channels.
 *
 * <p> An asynchronous channel provider is a concrete subclass of this class that
 * has a zero-argument constructor and implements the abstract methods specified
 * below.  A given invocation of the Java virtual machine maintains a single
 * system-wide default provider instance, which is returned by the {@link
 * #provider() provider} method.  The first invocation of that method will locate
 * the default provider as specified below.
 *
 * <p> All of the methods in this class are safe for use by multiple concurrent
 * threads.  </p>
 *
 * @since 1.7
 */

public abstract class AsynchronousChannelProvider {

    private static final Object lock = new Object();
    private static AsynchronousChannelProvider provider = null;

    private static Void checkPermission() {
        SecurityManager sm = System.getSecurityManager();
        if (sm != null)
            sm.checkPermission(new RuntimePermission("asynchronousChannelProvider"));
        return null;
    }
    private AsynchronousChannelProvider(Void ignore) { }

    /**
     * Initializes a new instance of this class.
     *
     * @throws  SecurityException
     *          If a security manager has been installed and it denies
     *          {@link RuntimePermission}<tt>("asynchronousChannelProvider")</tt>
     */
    protected AsynchronousChannelProvider() {
        this(checkPermission());
    }

    private static boolean loadProviderFromProperty() {
        String cn = System.getProperty("java.nio.channels.spi.AsynchronousChannelProvider");
        if (cn == null)
            return false;
        try {
            Class<?> c = Class.forName(cn, true,
                                       ClassLoader.getSystemClassLoader());
            provider = (AsynchronousChannelProvider)c.newInstance();
            return true;
        } catch (ClassNotFoundException x) {
            throw new ServiceConfigurationError(null, x);
        } catch (IllegalAccessException x) {
            throw new ServiceConfigurationError(null, x);
        } catch (InstantiationException x) {
            throw new ServiceConfigurationError(null, x);
        } catch (SecurityException x) {
            throw new ServiceConfigurationError(null, x);
        }
    }

    private static boolean loadProviderAsService() {

        ServiceLoader<AsynchronousChannelProvider> sl =
            ServiceLoader.load(AsynchronousChannelProvider.class,
                               ClassLoader.getSystemClassLoader());
        Iterator<AsynchronousChannelProvider> i = sl.iterator();
        for (;;) {
            try {
                if (!i.hasNext())
                    return false;
                provider = i.next();
                return true;
            } catch (ServiceConfigurationError sce) {
                if (sce.getCause() instanceof SecurityException) {
                    // Ignore the security exception, try the next provider
                    continue;
                }
                throw sce;
            }
        }
    }

    /**
     * Returns the system-wide default asynchronous channel provider for this
     * invocation of the Java virtual machine.
     *
     * <p> The first invocation of this method locates the default provider
     * object as follows: </p>
     *
     * <ol>
     *
     *   <li><p> If the system property
     *   <tt>java.nio.channels.spi.AsynchronousChannelProvider</tt> is defined
     *   then it is taken to be the fully-qualified name of a concrete provider class.
     *   The class is loaded and instantiated; if this process fails then an
     *   unspecified error is thrown.  </p></li>
     *
     *   <li><p> If a provider class has been installed in a jar file that is
     *   visible to the system class loader, and that jar file contains a
     *   provider-configuration file named
     *   <tt>java.nio.channels.spi.AsynchronousChannelProvider</tt> in the resource
     *   directory <tt>META-INF/services</tt>, then the first class name
     *   specified in that file is taken.  The class is loaded and
     *   instantiated; if this process fails then an unspecified error is
     *   thrown.  </p></li>
     *
     *   <li><p> Finally, if no provider has been specified by any of the above
     *   means then the system-default provider class is instantiated and the
     *   result is returned.  </p></li>
     *
     * </ol>
     *
     * <p> Subsequent invocations of this method return the provider that was
     * returned by the first invocation.  </p>
     *
     * @return  The system-wide default AsynchronousChannel provider
     */
    public static AsynchronousChannelProvider provider() {
        synchronized (lock) {
            if (provider != null) {
                return provider;
            }
            return AccessController
                .doPrivileged(new PrivilegedAction<AsynchronousChannelProvider>() {
                    public AsynchronousChannelProvider run() {
                            if (loadProviderFromProperty())
                                return provider;
                            if (loadProviderAsService())
                                return provider;
                            provider = sun.nio.ch.DefaultAsynchronousChannelProvider.create();
                            return provider;
                        }
                    });
        }
    }

    /**
     * The types of thread pool that an asynchronous channel group can be
     * constructed with.
     *
     * @since 1.7
     */
    public static enum ThreadPoolType {
        /**
         * Fixed thread pool.
         *
         * @see Executors#newFixedThreadPool
         */
        FIXED,
        /**
         * Cached thread pool.
         *
         * @see Executors#newCachedThreadPool
         */
        CACHED;
    }

    /**
     * Constructs a new asynchronous channel group.
     *
     * @param   poolType
     *          The type of thread pool
     * @param   executor
     *          The thread pool
     * @param   nThreads
     *          The number of threads or initial size, depending on the thread
     *          pool type.
     *
     * @throws  IllegalArgumentException
     *          If the {@code nThreads} parameter is invalid for the thread
     *          pool type
     * @throws  IOException
     *          If an I/O error occurs
     */
    public AsynchronousChannelGroup openAsynchronousChannelGroup
        (ThreadPoolType poolType, ExecutorService executor, int nThreads) throws IOException
    {
        return null;
    }

    /**
     * Opens an asynchronous server-socket channel.
     *
     * @param   group
     *          The group to which the channel is bound, or {@code null} to
     *          bind to the default group
     *
     * @return  The new channel
     *
     * @throws  IllegalChannelGroupException
     *          If the provider that created the group differs from this provider
     * @throws  ShutdownChannelGroupException
     *          The group is shutdown
     * @throws  IOException
     *          If an I/O error occurs
     */
    public abstract AsynchronousServerSocketChannel openAsynchronousServerSocketChannel
        (AsynchronousChannelGroup group) throws IOException;

    /**
     * Opens an asynchronous socket channel.
     *
     * @param   group
     *          The group to which the channel is bound, or {@code null} to
     *          bind to the default group
     *
     * @return  The new channel
     *
     * @throws  IllegalChannelGroupException
     *          If the provider that created the group differs from this provider
     * @throws  ShutdownChannelGroupException
     *          The group is shutdown
     * @throws  IOException
     *          If an I/O error occurs
     */
    public abstract AsynchronousSocketChannel openAsynchronousSocketChannel
        (AsynchronousChannelGroup group) throws IOException;

    /**
     * Opens an asynchronous datagram channel.
     *
     * @param   family
     *          The protocol family, or {@code null} for the default protocol
     *          family
     * @param   group
     *          The group to which the channel is bound, or {@code null} to
     *          bind to the default group
     *
     * @return  The new channel
     *
     * @throws  IllegalChannelGroupException
     *          If the provider that created the group differs from this provider
     * @throws  ShutdownChannelGroupException
     *          The group is shutdown
     * @throws  IOException
     *          If an I/O error occurs
     */
    public abstract AsynchronousDatagramChannel openAsynchronousDatagramChannel
        (ProtocolFamily family, AsynchronousChannelGroup group) throws IOException;
}
