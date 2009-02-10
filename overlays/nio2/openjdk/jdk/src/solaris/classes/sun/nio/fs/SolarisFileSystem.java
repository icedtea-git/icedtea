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

import java.nio.file.*;
import java.nio.file.attribute.*;
import java.io.IOException;
import java.util.*;
import java.security.AccessController;
import sun.security.action.GetPropertyAction;
import static sun.nio.fs.UnixNativeDispatcher.*;

/**
 * Solaris implementation of FileSystem
 */

class SolarisFileSystem extends UnixFileSystem {
    private final int majorVersion;
    private final int minorVersion;

    SolarisFileSystem(UnixFileSystemProvider provider, String dir) {
        super(provider, dir);

        String osversion = AccessController
            .doPrivileged(new GetPropertyAction("os.version"));
        String[] vers = osversion.split("\\.", 0);
        if (vers.length >= 2) {
            this.majorVersion = Integer.parseInt(vers[0]);
            this.minorVersion = Integer.parseInt(vers[1]);
        } else {
            // should not get here
            throw new AssertionError();
        }

        // O_NOFOLLOW not supported prior to Solaris 10
        if ((majorVersion < 5) || (majorVersion == 5 && minorVersion < 10))
            throw new RuntimeException("Need Solaris 10 or greater");
    }

    private boolean hasSolaris11Features() {
        if (majorVersion > 5 || (majorVersion == 5 && minorVersion >= 11)) {
            return true;
        } else {
            return false;
        }
    }


    boolean isSolaris() {
        return true;
    }


    public WatchService newWatchService()
        throws IOException
    {
        // FEN available since Solaris 11
        if (hasSolaris11Features()) {
            return new SolarisWatchService(this);
        } else {
            return new PollingWatchService();
        }
    }


    @SuppressWarnings("unchecked")
    public <V extends FileAttributeView> V newFileAttributeView(Class<V> view,
                                                                UnixPath file, LinkOption... options)
    {
        if (view == AclFileAttributeView.class)
            return (V) new SolarisAclFileAttributeView(file, followLinks(options));
        if (view == NamedAttributeView.class) {
            return(V) new SolarisNamedAttributeView(file, followLinks(options));
        }
        return super.newFileAttributeView(view, file, options);
    }


    protected FileAttributeView newFileAttributeView(String name,
                                                     UnixPath file,
                                                     LinkOption... options)
    {
        if (name.equals("acl"))
            return new SolarisAclFileAttributeView(file, followLinks(options));
        if (name.equals("xattr"))
            return new SolarisNamedAttributeView(file, followLinks(options));
        return super.newFileAttributeView(name, file, options);
    }

    // lazy initialization of the list of supported attribute views
    private static class LazyInitialization {
        static final Set<String> supportedFileAttributeViews =
            supportedFileAttributeViews();
        private static Set<String> supportedFileAttributeViews() {
            Set<String> result = new HashSet<String>();
            result.addAll(UnixFileSystem.standardFileAttributeViews());
            // additional Solaris-specific views
            result.add("acl");
            result.add("xattr");
            return Collections.unmodifiableSet(result);
        }
    }


    public Set<String> supportedFileAttributeViews() {
        return LazyInitialization.supportedFileAttributeViews;
    }


    void copyNonPosixAttributes(int ofd, int nfd) {
        SolarisNamedAttributeView.copyExtendedAttributes(ofd, nfd);
        // TDB: copy ACL from source to target
    }


    boolean supportsSecureDirectoryStreams() {
        return true;
    }

    /**
     * Returns object to iterate over entries in /etc/mnttab
     */

    Iterable<UnixMountEntry> getMountEntries() {
        ArrayList<UnixMountEntry> entries = new ArrayList<UnixMountEntry>();
        try {
            UnixPath mnttab = new UnixPath(this, "/etc/mnttab");
            long fp = fopen(mnttab, "r");
            try {
                for (;;) {
                    UnixMountEntry entry = new UnixMountEntry();
                    int res = getextmntent(fp, entry);
                    if (res < 0)
                        break;
                    entries.add(entry);
                }
            } finally {
                fclose(fp);
            }
        } catch (UnixException x) {
            // nothing we can do
        }
        return entries;
    }


    FileStore getFileStore(UnixPath path) throws IOException {
        return new SolarisFileStore(path);
    }


    FileStore getFileStore(UnixMountEntry entry) {
        return new SolarisFileStore(this, entry);
    }
}
