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

import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.util.*;
import java.io.IOException;
import java.security.AccessController;
import java.security.PrivilegedAction;

import org.classpath.icedtea.java.nio.file.FileRef;
import org.classpath.icedtea.java.nio.file.FileStore;
import org.classpath.icedtea.java.nio.file.FileSystem;
import org.classpath.icedtea.java.nio.file.Paths;

import org.classpath.icedtea.java.nio.file.attribute.BasicFileAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.FileAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.FileOwnerAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.FileStoreAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.FileStoreSpaceAttributes;
import org.classpath.icedtea.java.nio.file.attribute.FileStoreSpaceAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.PosixFileAttributeView;

/**
 * Base implementation of FileStore for Unix/like implementations.
 */

abstract class UnixFileStore
    extends FileStore
{
    // original path of file that identified file system
    private final UnixPath file;

    // device ID
    private final long dev;

    // entry in the mount tab
    private final UnixMountEntry entry;

    UnixFileStore(UnixPath file) throws IOException {
        // need device ID
        long devID = 0;
        try {
            devID = UnixFileAttributes.get(file, true).dev();
        } catch (UnixException x) {
            x.rethrowAsIOException(file);
        }
        this.file = file;
        this.dev = devID;
        this.entry = findMountEntry(file.getFileSystem());
    }

    UnixFileStore(UnixFileSystem fs, UnixMountEntry entry) {
        this.file = new UnixPath(fs, entry.dir());
        this.dev = entry.dev();
        this.entry = entry;
    }

    /**
     * Find the mount entry for this file system
     */
    abstract UnixMountEntry findMountEntry(UnixFileSystem fs) throws IOException;

    UnixPath file() {
        return file;
    }

    long dev() {
        return dev;
    }

    UnixMountEntry entry() {
        return entry;
    }


    public String name() {
        return entry.name();
    }


    public String type() {
        return entry.fstype();
    }


    public boolean isReadOnly() {
        return entry.isReadOnly();
    }


    @SuppressWarnings("unchecked")
    public <V extends FileStoreAttributeView> V getFileStoreAttributeView(Class<V> viewType)
    {
        if (viewType == FileStoreSpaceAttributeView.class)
            return (V) new UnixFileStoreSpaceAttributeView(this);
        return (V) null;
    }


    public FileStoreAttributeView getFileStoreAttributeView(String name) {
        if (name.equals("space"))
            return new UnixFileStoreSpaceAttributeView(this);
        return  null;
    }


    public boolean supportsFileAttributeView(Class<? extends FileAttributeView> type) {
        if (type == BasicFileAttributeView.class)
            return true;
        if (type == PosixFileAttributeView.class ||
            type == FileOwnerAttributeView.class)
        {
            // lookup fstypes.properties
            FeatureStatus status = checkIfFeaturePresent("posix");
            if (status == FeatureStatus.NOT_PRESENT)
                return false;
            return true;
        }
        return false;
    }


    public boolean supportsFileAttributeView(String name) {
        if (name.equals("basic") || name.equals("unix"))
            return true;
        if (name.equals("posix"))
            return supportsFileAttributeView(PosixFileAttributeView.class);
        if (name.equals("owner"))
            return supportsFileAttributeView(FileOwnerAttributeView.class);
        return false;
    }


    public boolean equals(Object ob) {
        if (ob == this)
            return true;
        if (!(ob instanceof UnixFileStore))
            return false;
        UnixFileStore other = (UnixFileStore)ob;
        return dev == other.dev;
    }


    public int hashCode() {
        return (int)(dev ^ (dev >>> 32));
    }


    public String toString() {
        StringBuilder sb = new StringBuilder(new String(entry.dir()));
        sb.append(" (");
        sb.append(entry.name());
        sb.append(")");
        return sb.toString();
    }

    private static class UnixFileStoreSpaceAttributeView
        extends AbstractFileStoreSpaceAttributeView
    {
        private final UnixFileStore fs;

        UnixFileStoreSpaceAttributeView(UnixFileStore fs) {
            this.fs = fs;
        }


        public FileStoreSpaceAttributes readAttributes()
            throws IOException
        {
            UnixPath file = fs.file();

            SecurityManager sm = System.getSecurityManager();
            if (sm != null) {
                file.checkRead();
                sm.checkPermission(new RuntimePermission("getFileStoreAttributes"));
            }
            final UnixFileStoreAttributes attrs;
            try {
                attrs = UnixFileStoreAttributes.get(file);
            } catch (UnixException x) {
                x.rethrowAsIOException(file);
                return null;    // keep compile happy
            }

            return new FileStoreSpaceAttributes() {

                public long totalSpace() {
                    return attrs.blockSize() * attrs.totalBlocks();
                }

                public long usableSpace() {
                    return attrs.blockSize() * attrs.availableBlocks();
                }

                public long unallocatedSpace() {
                    return attrs.blockSize() * attrs.freeBlocks();
                }
            };
        }
    }

    // -- fstypes.properties --

    private static final Object loadLock = new Object();
    private static volatile Properties props;

    enum FeatureStatus {
        PRESENT,
        NOT_PRESENT,
        UNKNOWN;
    }

    /**
     * Returns status to indicate if file system supports a given feature
     */
    FeatureStatus checkIfFeaturePresent(String feature) {
        if (props == null) {
            synchronized (loadLock) {
                if (props == null) {
                    props = AccessController.doPrivileged(
                        new PrivilegedAction<Properties>() {

                            public Properties run() {
                                return loadProperties();
                            }});
                }
            }
        }

        String value = props.getProperty(type());
        if (value != null) {
            String[] values = value.split("\\s");
            for (String s: values) {
                s = s.trim().toLowerCase();
                if (s.equals(feature)) {
                    return FeatureStatus.PRESENT;
                }
                if (s.startsWith("no")) {
                    s = s.substring(2);
                    if (s.equals(feature)) {
                        return FeatureStatus.NOT_PRESENT;
                    }
                }
            }
        }
        return FeatureStatus.UNKNOWN;
    }

    private static Properties loadProperties() {
        Properties result = new Properties();
        String fstypes = System.getProperty("java.home") + "/lib/fstypes.properties";
        FileRef file = Paths.get(fstypes);
        try {
            ReadableByteChannel rbc = file.newByteChannel();
            try {
                result.load(Channels.newReader(rbc, "UTF-8"));
            } finally {
                rbc.close();
            }
        } catch (IOException x) {
        }
        return result;
    }
}
