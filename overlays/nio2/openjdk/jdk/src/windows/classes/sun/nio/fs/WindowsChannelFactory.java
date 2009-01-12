/*
 * Copyright 2007 Sun Microsystems, Inc.  All Rights Reserved.
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

package sun.nio.fs;

import java.nio.file.*;
import java.nio.channels.*;
import java.io.FileDescriptor;
import java.io.IOException;
import java.util.*;

import com.sun.nio.file.ExtendedOpenOption;

import sun.nio.ch.FileChannelImpl;
import sun.nio.ch.ThreadPool;
import sun.nio.ch.WindowsAsynchronousFileChannelImpl;
import sun.misc.SharedSecrets;
import sun.misc.JavaIOFileDescriptorAccess;

import static sun.nio.fs.WindowsNativeDispatcher.*;
import static sun.nio.fs.WindowsConstants.*;

/**
 * Factory to create FileChannels and AsynchronousFileChannels.
 */

class WindowsChannelFactory {
    private static final JavaIOFileDescriptorAccess fdAccess =
        SharedSecrets.getJavaIOFileDescriptorAccess();

    private WindowsChannelFactory() { }

    /**
     * Do not follow reparse points when opening an existing file. Do not fail
     * if the file is a reparse point.
     */
    static final OpenOption NOFOLLOW_REPARSEPOINT = new OpenOption() { };

    /**
     * Open/creates file, returning FileChannel to access the file
     *
     * @param   pathForWindows
     *          The path of the file to open/create
     * @param   pathToCheck
     *          The path used for permission checks (if security manager)
     */
    static FileChannel newFileChannel(String pathForWindows,
                                      String pathToCheck,
                                      Set<? extends OpenOption> options,
                                      long pSecurityDescriptor)
        throws WindowsException
    {
        boolean reading = false;
        boolean writing = false;
        boolean append = false;
        boolean trunc = false;

        // check for invalid flags
        for (OpenOption flag: options) {
            if (flag == StandardOpenOption.READ) {
                reading = true; continue;
            }
            if (flag == StandardOpenOption.WRITE) {
                writing = true; continue;
            }
            if (flag == StandardOpenOption.APPEND) {
                append = true;
                writing = true;
                continue;
            }
            if (flag == StandardOpenOption.TRUNCATE_EXISTING) {
                trunc = true; continue;
            }
            if (flag == null)
                throw new NullPointerException();
            if (!(flag instanceof StandardOpenOption) &&
                !(flag instanceof ExtendedOpenOption))
            {
                throw new UnsupportedOperationException("Unsupported open option");
            }
        }

        // default is reading
        if (!reading && !writing) {
            reading = true;
        }

        // check for invalid combinations
        if (reading && append)
            throw new IllegalArgumentException("READ + APPEND not allowed");
        if (append && trunc)
            throw new IllegalArgumentException("APPEND + TRUNCATE_EXISTING not allowed");

        FileDescriptor fdObj = open(pathForWindows, pathToCheck, reading, writing,
            append, false, options, pSecurityDescriptor);
        return FileChannelImpl.open(fdObj, reading, writing, null);
    }

    /**
     * Open/creates file, returning AsynchronousFileChannel to access the file
     *
     * @param   pathForWindows
     *          The path of the file to open/create
     * @param   pathToCheck
     *          The path used for permission checks (if security manager)
     * @param   pool
     *          The thread pool that the channel is associated with
     */
    static AsynchronousFileChannel newAsynchronousFileChannel(String pathForWindows,
                                                              String pathToCheck,
                                                              Set<? extends OpenOption> options,
                                                              long pSecurityDescriptor,
                                                              ThreadPool pool)
        throws IOException
    {
        boolean reading = false;
        boolean writing = false;

        // check for invalid flags
        for (OpenOption flag: options) {
            if (flag == StandardOpenOption.READ) {
                reading = true; continue;
            }
            if (flag == StandardOpenOption.WRITE) {
                writing = true; continue;
            }
            if (flag == null)
                throw new NullPointerException();
            if (!(flag instanceof StandardOpenOption) &&
                !(flag instanceof ExtendedOpenOption))
            {
                throw new UnsupportedOperationException("Unsupported open option");
            }
            if (flag == StandardOpenOption.APPEND)
                throw new UnsupportedOperationException("'APPEND' not supported");
        }

        // default is reading
        if (!reading && !writing) {
            reading = true;
        }

        // open file for overlapped I/O
        FileDescriptor fdObj;
        try {
            fdObj = open(pathForWindows, pathToCheck, reading, writing, false,
                         true, options, pSecurityDescriptor);
        } catch (WindowsException x) {
            x.rethrowAsIOException(pathForWindows);
            return null;
        }

        // create the AsynchronousFileChannel
        try {
            return WindowsAsynchronousFileChannelImpl.open(fdObj, reading, writing, pool);
        } catch (IOException x) {
            // IOException is thrown if the file handle cannot be associated
            // with the completion port. All we can do is close the file.
            long handle = fdAccess.getHandle(fdObj);
            CloseHandle(handle);
            throw x;
        }
    }

    /**
     * Opens file based on parameters and options, returning a FileDescriptor
     * encapsulating the handle to the open file.
     */
    private static FileDescriptor open(String pathForWindows,
                                       String pathToCheck,
                                       boolean reading,
                                       boolean writing,
                                       boolean append,
                                       boolean overlapped,
                                       Set<? extends OpenOption> options,
                                       long pSecurityDescriptor)
        throws WindowsException
    {
        // set to true if file must be truncated after open
        boolean truncateAfterOpen = false;

        // map options
        int dwDesiredAccess = 0;
        if (reading)
            dwDesiredAccess |= GENERIC_READ;
        if (writing)
            dwDesiredAccess |= (append) ? FILE_APPEND_DATA : GENERIC_WRITE;

        int dwShareMode = 0;
        if (!options.contains(ExtendedOpenOption.NOSHARE_READ))
            dwShareMode |= FILE_SHARE_READ;
        if (!options.contains(ExtendedOpenOption.NOSHARE_WRITE))
            dwShareMode |= FILE_SHARE_WRITE;
        if (!options.contains(ExtendedOpenOption.NOSHARE_DELETE))
            dwShareMode |= FILE_SHARE_DELETE;

        int dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
        int dwCreationDisposition = OPEN_EXISTING;
        if (writing) {
            if (options.contains(StandardOpenOption.CREATE_NEW)) {
                dwCreationDisposition = CREATE_NEW;
                // force create to fail if file is orphaned reparse point
                dwFlagsAndAttributes |= FILE_FLAG_OPEN_REPARSE_POINT;
            } else {
                if (options.contains(StandardOpenOption.CREATE))
                    dwCreationDisposition = OPEN_ALWAYS;
                if (options.contains(StandardOpenOption.TRUNCATE_EXISTING)) {
                    // Windows doesn't have a creation disposition that exactly
                    // corresponds to CREATE + TRUNCATE_EXISTING so we use
                    // the OPEN_ALWAYS mode and then truncate the file.
                    if (dwCreationDisposition == OPEN_ALWAYS) {
                        truncateAfterOpen = true;
                    } else {
                        dwCreationDisposition = TRUNCATE_EXISTING;
                    }
                }
            }
        }

        if (options.contains(StandardOpenOption.DSYNC) || options.contains(StandardOpenOption.SYNC))
            dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
        if (overlapped)
            dwFlagsAndAttributes |= FILE_FLAG_OVERLAPPED;

        boolean deleteOnClose = options.contains(StandardOpenOption.DELETE_ON_CLOSE);
        if (deleteOnClose)
            dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;

        // NOFOLLOW_LINKS and NOFOLLOW_REPARSEPOINT mean open reparse point
        boolean okayToFollowLinks = true;
        if (dwCreationDisposition != CREATE_NEW &&
            (options.contains(LinkOption.NOFOLLOW_LINKS) ||
             options.contains(NOFOLLOW_REPARSEPOINT) ||
             deleteOnClose))
        {
            if (options.contains(LinkOption.NOFOLLOW_LINKS))
                okayToFollowLinks = false;
            dwFlagsAndAttributes |= FILE_FLAG_OPEN_REPARSE_POINT;
        }

        // permission check
        if (pathToCheck != null) {
            SecurityManager sm = System.getSecurityManager();
            if (sm != null) {
                if (reading)
                    sm.checkRead(pathToCheck);
                if (writing)
                    sm.checkWrite(pathToCheck);
                if (deleteOnClose)
                    sm.checkDelete(pathToCheck);
            }
        }

        // open file
        long handle = CreateFile(pathForWindows,
                                 dwDesiredAccess,
                                 dwShareMode,
                                 pSecurityDescriptor,
                                 dwCreationDisposition,
                                 dwFlagsAndAttributes);

        // make sure this isn't a symbolic link.
        if (!okayToFollowLinks) {
            try {
                // for security reasons we can only test here the file is a
                // reparse point. To read the reparse point tag would require
                // re-opening the file and this method is required to be atomic.
                if (WindowsFileAttributes.readAttributes(handle).isReparsePoint()) {
                    throw new WindowsException("File is reparse point");
                }

            } catch (WindowsException x) {
                CloseHandle(handle);
                throw x;
            }
        }

        // truncate file (for CREATE + TRUNCATE_EXISTING case)
        if (truncateAfterOpen) {
            try {
                SetEndOfFile(handle);
            } catch (WindowsException x) {
                CloseHandle(handle);
                throw x;
            }
        }

        // make the file sparse if needed
        if (dwCreationDisposition == CREATE_NEW &&
            options.contains(StandardOpenOption.SPARSE))
        {
            try {
                DeviceIoControlSetSparse(handle);
            } catch (WindowsException x) {
                // ignore as sparse option is hint
            }
        }

        // create FileDescriptor and return
        FileDescriptor fdObj = new FileDescriptor();
        fdAccess.setHandle(fdObj, handle);
        return fdObj;
    }
}
