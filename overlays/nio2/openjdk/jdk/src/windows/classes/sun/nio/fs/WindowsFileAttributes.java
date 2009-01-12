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

package sun.nio.fs;

import java.nio.file.attribute.*;
import java.util.concurrent.TimeUnit;
import sun.misc.Unsafe;

import static sun.nio.fs.WindowsNativeDispatcher.*;
import static sun.nio.fs.WindowsConstants.*;

class WindowsFileAttributes
    implements DosFileAttributes
{
    private static final Unsafe unsafe = Unsafe.getUnsafe();

    /*
     * typedef struct _BY_HANDLE_FILE_INFORMATION {
     *     DWORD    dwFileAttributes;
     *     FILETIME ftCreationTime;
     *     FILETIME ftLastAccessTime;
     *     FILETIME ftLastWriteTime;
     *     DWORD    dwVolumeSerialNumber;
     *     DWORD    nFileSizeHigh;
     *     DWORD    nFileSizeLow;
     *     DWORD    nNumberOfLinks;
     *     DWORD    nFileIndexHigh;
     *     DWORD    nFileIndexLow;
     * } BY_HANDLE_FILE_INFORMATION;
     */
    private static final short SIZEOF_FILE_INFORMATION  = 52;
    private static final short OFFSETOF_ATTRIBUTES      = 0;
    private static final short OFFSETOF_CREATETIME      = 4;
    private static final short OFFSETOF_LASTACCESSTIME  = 12;
    private static final short OFFSETOF_LASTWRITETIME   = 20;
    private static final short OFFSETOF_VOLSERIALNUM    = 28;
    private static final short OFFSETOF_SIZEHIGH        = 32;
    private static final short OFFSETOF_SIZELOW         = 36;
    private static final short OFFSETOF_NUMLINKS        = 40;
    private static final short OFFSETOF_INDEXHIGH       = 44;
    private static final short OFFSETOF_INDEXLOW        = 48;

    // attributes read from file system
    private final int attrs;
    private final long creationTime;
    private final long lastAccessTime;
    private final long lastWriteTime;
    private final long size;
    private final int linkCount;
    private final int volSerialNumber;
    private final int fileIndexHigh;
    private final int fileIndexLow;

    // used when the file is reparse point
    private volatile boolean haveReparseTag;        // true if reparse tag read
    private volatile int reparseTag;                // the tag value

    /**
     * Convert 64-bit value representing the number of 100-nanosecond intervals
     * since January 1, 1601 to java time.
     */
    private static long toJavaTime(long time) {
        time /= 10000L;
        time -= 11644473600000L;
        return time;
    }

    /**
     * Convert java time to 64-bit value representing the number of 100-nanosecond
     * intervals since January 1, 1601.
     */
    static long toWindowsTime(long time) {
        time += 11644473600000L;
        time *= 10000L;
        return time;
    }

    /**
     * Initialize a few instance of this class
     */
    private WindowsFileAttributes(long address) {
        attrs = unsafe.getInt(address + OFFSETOF_ATTRIBUTES);
        creationTime = toJavaTime(unsafe.getLong(address + OFFSETOF_CREATETIME));
        lastAccessTime = toJavaTime(unsafe.getLong(address + OFFSETOF_LASTACCESSTIME));
        lastWriteTime = toJavaTime(unsafe.getLong(address + OFFSETOF_LASTWRITETIME));
        volSerialNumber = unsafe.getInt(address + OFFSETOF_VOLSERIALNUM);
        size =  ((long)(unsafe.getInt(address + OFFSETOF_SIZEHIGH)) << 32)
            + (unsafe.getInt(address + OFFSETOF_SIZELOW) & 0xFFFFFFFFL);
        linkCount = unsafe.getInt(address + OFFSETOF_NUMLINKS);
        fileIndexHigh = unsafe.getInt(address + OFFSETOF_INDEXHIGH);
        fileIndexLow = unsafe.getInt(address + OFFSETOF_INDEXLOW);
    }

    /**
     * Reads the attributes but does not process reparse points
     */
    static WindowsFileAttributes readAttributes(long handle)
        throws WindowsException
    {
        NativeBuffer buffer = NativeBuffers.getNativeBuffer(SIZEOF_FILE_INFORMATION);
        try {
            GetFileInformationByHandle(handle, buffer.address());
            return new WindowsFileAttributes(buffer.address());
        } finally {
            buffer.release();
        }
    }

    /**
     * If this object represents the attributes of a reparse point then read
     * the reparse point tag. This method has effect if the attributes were
     * read from a file that is not a reparse point.
     */
    WindowsFileAttributes finishRead(WindowsPath path)
        throws WindowsException
    {
        if (isReparsePoint()) {
            WindowsNativeDispatcher.FirstFile data =
                FindFirstFile(path.getPathForWin32Calls());
            reparseTag = data.reserved0();
            haveReparseTag = true;
            FindClose(data.handle());
        }
        return this;
    }

    /**
     * Returns attributes of given file. If the file is a reparse point then its
     * reparse tag is also read.
     */
    static WindowsFileAttributes get(WindowsPath path, boolean followLinks)
        throws WindowsException
    {
        long handle = path.openForReadAttributeAccess(followLinks);
        try {
            return readAttributes(handle).finishRead(path);
        } finally {
            CloseHandle(handle);
        }
    }

    /**
     * Returns true if the attribtues are of the same file - both files must
     * be open.
     */
    static boolean isSameFile(WindowsFileAttributes attrs1,
                              WindowsFileAttributes attrs2)
    {
        // volume serial number and file index must be the same
        if (attrs1.volSerialNumber != attrs2.volSerialNumber)
            return false;
        if (attrs1.fileIndexHigh != attrs2.fileIndexHigh)
            return false;
        if (attrs1.fileIndexLow != attrs2.fileIndexLow)
            return false;
        return true;
    }

    // package-private
    int attributes() {
        return attrs;
    }

    int volSerialNumber() {
        return volSerialNumber;
    }

    int fileIndexHigh() {
        return fileIndexHigh;
    }

    int fileIndexLow() {
        return fileIndexLow;
    }


    public long size() {
        return size;
    }


    public long lastModifiedTime() {
        return (lastWriteTime >= 0L) ? lastWriteTime : 0L;
    }


    public long lastAccessTime() {
        return (lastAccessTime >= 0L) ? lastAccessTime : 0L;
    }


    public long creationTime() {
        return (creationTime >= 0L) ? creationTime : 0L;
    }


    public TimeUnit resolution() {
        return TimeUnit.MILLISECONDS;
    }


    public int linkCount() {
        return linkCount;
    }


    public Object fileKey() {
        return null;
    }

    // package private
    boolean isReparsePoint() {
        return (attrs & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    }

    boolean isDirectoryLink() {
        return isSymbolicLink() && ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0);
    }


    public boolean isSymbolicLink() {
        if (isReparsePoint()) {
            if (!haveReparseTag)
                throw new AssertionError("reparse tag not read");
            return reparseTag == IO_REPARSE_TAG_SYMLINK;
        } else {
            return false;
        }
    }


    public boolean isDirectory() {
        // ignore FILE_ATTRIBUTE_DIRECTORY attribute if file is a sym link
        if (isSymbolicLink())
            return false;
        return ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0);
    }


    public boolean isOther() {
        if (isSymbolicLink())
            return false;
        // return true if device or reparse point
        if ((attrs & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_REPARSE_POINT)) != 0)
            return true;
        return false;
    }


    public boolean isRegularFile() {
        return !isSymbolicLink() && !isDirectory() && !isOther();
    }


    public boolean isReadOnly() {
        return (attrs & FILE_ATTRIBUTE_READONLY) != 0;
    }


    public boolean isHidden() {
        return (attrs & FILE_ATTRIBUTE_HIDDEN) != 0;
    }


    public boolean isArchive() {
        return (attrs & FILE_ATTRIBUTE_ARCHIVE) != 0;
    }


    public boolean isSystem() {
        return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
    }
}
