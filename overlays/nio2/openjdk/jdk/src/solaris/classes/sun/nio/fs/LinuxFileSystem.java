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

import java.io.IOException;
import java.util.*;
import java.security.AccessController;
import sun.security.action.GetPropertyAction;
import static sun.nio.fs.LinuxNativeDispatcher.*;

import java.nio.file.FileStore;
import java.nio.file.LinkOption;
import java.nio.file.WatchService;

import java.nio.file.attribute.DosFileAttributeView;
import java.nio.file.attribute.FileAttributeView;
import java.nio.file.attribute.NamedAttributeView;

/**
 * Linux implementation of FileSystem
 */

class LinuxFileSystem extends UnixFileSystem {
    private final int majorVersion;
    private final int minorVersion;
    private final int microVersion;

    LinuxFileSystem(UnixFileSystemProvider provider, String dir) {
        super(provider, dir);

        // assume X.Y[-Z] format
        String osversion = AccessController
            .doPrivileged(new GetPropertyAction("os.version"));
        String[] vers = osversion.split("\\.", 0);
        if (vers.length >= 2) {
            this.majorVersion = Integer.parseInt(vers[0]);
            this.minorVersion = Integer.parseInt(vers[1]);
            if (vers.length > 2) {
                String[] microVers = vers[2].split("-", 0);
                this.microVersion = (microVers.length > 0) ?
                    Integer.parseInt(microVers[0]) : 0;
            } else {
                this.microVersion = 0;
            }
        } else {
            throw new AssertionError();
        }
    }


    public WatchService newWatchService()
        throws IOException
    {
        // inotify available since 2.6.13
        if ((majorVersion > 2) ||
            (majorVersion == 2 && minorVersion > 6) ||
            ((majorVersion == 2) && (minorVersion == 6) && (microVersion >= 13)))
        {
            return new LinuxWatchService(this);
        }

        // use polling implementation on older kernels
        return new PollingWatchService();
    }


    @SuppressWarnings("unchecked")
    public <V extends FileAttributeView> V newFileAttributeView(Class<V> view,
                                                                UnixPath file,
                                                                LinkOption... options)
    {
        if (view == DosFileAttributeView.class)
            return (V) new LinuxDosFileAttributeView(file, followLinks(options));
        if (view == NamedAttributeView.class)
            return (V) new LinuxNamedAttributeView(file, followLinks(options));
        return super.newFileAttributeView(view, file, options);
    }


    @SuppressWarnings("unchecked")
    public FileAttributeView newFileAttributeView(String name,
                                                  UnixPath file,
                                                  LinkOption... options)
    {
        if (name.equals("dos"))
            return new LinuxDosFileAttributeView(file, followLinks(options));
        if (name.equals("xattr"))
            return new LinuxNamedAttributeView(file, followLinks(options));
        return super.newFileAttributeView(name, file, options);
    }

    // lazy initialization of the list of supported attribute views
    private static class LazyInitialization {
        static final Set<String> supportedFileAttributeViews =
            supportedFileAttributeViews();
        private static Set<String> supportedFileAttributeViews() {
            Set<String> result = new HashSet<String>();
            result.addAll(UnixFileSystem.standardFileAttributeViews());
            // additional Linux-specific views
            result.add("dos");
            result.add("xattr");
            return Collections.unmodifiableSet(result);
        }
    }


    public Set<String> supportedFileAttributeViews() {
        return LazyInitialization.supportedFileAttributeViews;
    }


    void copyNonPosixAttributes(int ofd, int nfd) {
        LinuxNamedAttributeView.copyExtendedAttributes(ofd, nfd);
    }


    boolean supportsSecureDirectoryStreams() {
        // openat etc. available since 2.6.16
        if ((majorVersion > 2) ||
            (majorVersion == 2 && minorVersion > 6) ||
            ((majorVersion == 2) && (minorVersion == 6) && (microVersion >= 16)))
        {
            return true;
        }
        return false;
    }

    /**
     * Returns object to iterate over entries in /etc/mtab
     */

    Iterable<UnixMountEntry> getMountEntries() {
        ArrayList<UnixMountEntry> entries = new ArrayList<UnixMountEntry>();
        try {
            long fp = setmntent("/etc/mtab".getBytes(), "r".getBytes());
            try {
                for (;;) {
                    UnixMountEntry entry = new UnixMountEntry();
                    int res = getextmntent(fp, entry);
                    if (res < 0)
                        break;
                    entries.add(entry);
                }
            } finally {
                endmntent(fp);
            }

        } catch (UnixException x) {
            // nothing we can do
        }
        return entries;
    }


    FileStore getFileStore(UnixPath path) throws IOException {
        return new LinuxFileStore(path);
    }


    FileStore getFileStore(UnixMountEntry entry) {
        return new LinuxFileStore(this, entry);
    }
}
