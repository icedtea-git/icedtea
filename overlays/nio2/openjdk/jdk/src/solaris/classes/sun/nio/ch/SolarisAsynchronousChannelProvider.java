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

import java.util.concurrent.ExecutorService;
import java.io.IOException;

import org.classpath.icedtea.java.net.ProtocolFamily;

import org.classpath.icedtea.java.nio.channels.AsynchronousChannelGroup;
import org.classpath.icedtea.java.nio.channels.AsynchronousDatagramChannel;
import org.classpath.icedtea.java.nio.channels.AsynchronousSocketChannel;
import org.classpath.icedtea.java.nio.channels.AsynchronousServerSocketChannel;
import org.classpath.icedtea.java.nio.channels.IllegalChannelGroupException;

import org.classpath.icedtea.java.nio.channels.spi.AsynchronousChannelProvider;

public class SolarisAsynchronousChannelProvider
    extends AsynchronousChannelProvider
{
    private static volatile SolarisEventPort defaultEventPort;

    private SolarisEventPort defaultEventPort() throws IOException {
        if (defaultEventPort == null) {
            synchronized (SolarisAsynchronousChannelProvider.class) {
                if (defaultEventPort == null) {
                    defaultEventPort =
                        new SolarisEventPort(this, ThreadPool.getDefault()).start();
                }
            }
        }
        return defaultEventPort;
    }

    public SolarisAsynchronousChannelProvider() {
    }


    public AsynchronousChannelGroup openAsynchronousChannelGroup(ThreadPoolType poolType,
                                                                 ExecutorService executor,
                                                                 int nThreads)
        throws IOException
    {
        return new SolarisEventPort(this, ThreadPool.create(poolType, executor, nThreads)).start();
    }

    private SolarisEventPort toEventPort(AsynchronousChannelGroup group)
        throws IOException
    {
        if (group == null) {
            return defaultEventPort();
        } else {
            if (!(group instanceof SolarisEventPort))
                throw new IllegalChannelGroupException();
            return (SolarisEventPort)group;
        }
    }


    public AsynchronousServerSocketChannel openAsynchronousServerSocketChannel(AsynchronousChannelGroup group)
        throws IOException
    {
        return new UnixAsynchronousServerSocketChannelImpl(toEventPort(group));
    }


    public AsynchronousSocketChannel openAsynchronousSocketChannel(AsynchronousChannelGroup group)
        throws IOException
    {
        return new UnixAsynchronousSocketChannelImpl(toEventPort(group));
    }


    public AsynchronousDatagramChannel openAsynchronousDatagramChannel(ProtocolFamily family,
                                                                       AsynchronousChannelGroup group)
        throws IOException
    {
        return new SimpleAsynchronousDatagramChannelImpl(family, toEventPort(group));
    }
}
