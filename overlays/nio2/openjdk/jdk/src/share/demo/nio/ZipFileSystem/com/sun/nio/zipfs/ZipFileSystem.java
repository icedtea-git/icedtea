/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Sun Microsystems nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.sun.nio.zipfs;

import java.io.Closeable;
import java.io.IOException;
import java.net.URI;
import java.util.*;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.regex.Pattern;

import org.classpath.icedtea.java.nio.file.ClosedFileSystemException;
import org.classpath.icedtea.java.nio.file.DirectoryStream;
import org.classpath.icedtea.java.nio.file.FileRef;
import org.classpath.icedtea.java.nio.file.FileStore;
import org.classpath.icedtea.java.nio.file.FileSystem;
import org.classpath.icedtea.java.nio.file.InvalidPathException;
import org.classpath.icedtea.java.nio.file.Path;
import org.classpath.icedtea.java.nio.file.PathMatcher;
import org.classpath.icedtea.java.nio.file.WatchService;

import org.classpath.icedtea.java.nio.file.attribute.UserPrincipalLookupService;

import org.classpath.icedtea.java.nio.file.spi.FileSystemProvider;

public class ZipFileSystem extends FileSystem {

    private final ZipFileSystemProvider provider;
    //path upto first zip file
    // for example in Win c:/foo/bar.zip/a/b/c - zipFile represents c:/foo/bar.zip
    // this contains real path following the links (change it, if no need to follow links)
    private final String zipFile;
    private final String defaultdir;
    private final ReadWriteLock closeLock = new ReentrantReadWriteLock();
    private boolean open = true;
    private Set<Closeable> closeableObjects = new HashSet<Closeable>();

    ZipFileSystem(ZipFileSystemProvider provider,
            FileRef fref) {

        this(provider, fref.toString(), "/");

    }

    ZipFileSystem(ZipFileSystemProvider provider, String path, String defaultDir) {
        this.provider = provider;
        this.zipFile = path;
        this.defaultdir = defaultDir;
    }


    public FileSystemProvider provider() {
        return provider;
    }


    public String getSeparator() {
        return "/";
    }


    public boolean isOpen() {
        return open;
    }


    public boolean isReadOnly() {
        return true;
    }


    public void close() throws IOException {
        closeLock.writeLock().lock();
        URI root = null;
        try {
            if (!open) {
                return;
            }
            root = getPath("/").toUri();
            open = false;
        } finally {
            closeLock.writeLock().unlock();
        }
        implClose(root);
    }

    final void begin() {
        closeLock.readLock().lock();
        if (!isOpen()) {
            throw new ClosedFileSystemException();
        }
    }

    final void end() {
        closeLock.readLock().unlock();
    }
    // Free all cached Zip/Jar files
    private void implClose(URI root) throws IOException {
        ZipUtils.remove(root); // remove cached filesystem
        provider.removeFileSystem(root);
        Iterator<Closeable> itr = closeableObjects.iterator();
        while (itr.hasNext()) {
            try {
                itr.next().close();
                itr.remove();
            } catch (IOException e) {
                throw e;
            }
        }
    }

    boolean addCloseableObjects(Closeable obj) {
        return closeableObjects.add(obj);
    }


    public Iterable<Path> getRootDirectories() {
        try {
            begin();
            ArrayList<Path> pathArr = new ArrayList<Path>();
            ZipFilePath root = new ZipFilePath(this, new byte[]{'/'});
            pathArr.add(root);
            return pathArr;
        } finally {
            end();
        }

    }

    String getDefaultDir() {
        return defaultdir;
    }

    String getZipFileSystemFile() {
        return zipFile;
    }


    public ZipFilePath getPath(String path) {

        if (path == null) {
            throw new NullPointerException();
        }
        if (path.equals("")) {
            throw new InvalidPathException(path, "path should not be empty");
        }
        try {
            begin();
            byte[] parsedPath = ZipPathParser.normalize(path).getBytes();
            return new ZipFilePath(this, parsedPath);
        } finally {
            end();
        }
    }


    public UserPrincipalLookupService getUserPrincipalLookupService() {
        return null;
    }


    public WatchService newWatchService() {
        throw new UnsupportedOperationException();
    }


    public Iterable<FileStore> getFileStores() {
        try {
            begin();
            Iterator<Path> iterator = this.getRootDirectories().iterator();
            final ZipFileStoreIterator zipIterator = new ZipFileStoreIterator(iterator);
            return new Iterable<FileStore>() {

                public Iterator<FileStore> iterator() {
                    return zipIterator;
                }
            };
        } finally {
            end();
        }
    }

    private static class ZipFileStoreIterator implements Iterator<FileStore> {

        private final Iterator<Path> roots;
        private FileStore next;

        ZipFileStoreIterator(Iterator<Path> root) {
            roots = root;
        }

        private FileStore readNext() {
            for (;;) {
                if (!roots.hasNext()) {
                    return null;
                }
                try {
                    ZipFilePath root = (ZipFilePath) roots.next();
                    FileStore fs = ZipFileStore.create(root);
                    if (fs != null) {
                        return fs;
                    }
                } catch (IOException e) {

                }
            }
        }


        public synchronized boolean hasNext() {
            if (next != null) {
                return true;
            }
            next = readNext();
            return (next != null);

        }

        public synchronized FileStore next() {
            if (next == null) {
                next = readNext();
            }
            if (next == null) {
                throw new NoSuchElementException();
            } else {
                FileStore result = next;
                next = null;
                return result;
            }
        }

        public void remove() {
            throw new UnsupportedOperationException("");
        }
    }

    private static final Set<String> supportedFileAttributeViews =
        Collections.unmodifiableSet(new HashSet(Arrays.asList("basic", "zip", "jar")));


    public Set<String> supportedFileAttributeViews() {
        return supportedFileAttributeViews;
    }


    public String toString() {
        return getZipFileSystemFile();
    }


    public PathMatcher getNameMatcher(String syntax, String expr) {
        if (syntax.equalsIgnoreCase("glob")) {
             // Temporary
             String regex = sun.nio.fs.Globs.toRegexPattern(expr);
             final Pattern pattern = Pattern.compile(regex,
                 (Pattern.CASE_INSENSITIVE | Pattern.UNICODE_CASE));
             final DirectoryStream.Filter<Path> filter =
                new DirectoryStream.Filter<Path>() {

                    public boolean accept(Path entry)  {
                        return pattern.matcher(entry.getName().toString()).matches();
                    }
                };

            return new PathMatcher() {

                public boolean matches(Path path) {
                    // match on file name only
                    Path name = path.getName();
                    if (name == null)
                        return false;
                    return filter.accept(name);
                }};
        }
        if (syntax.equalsIgnoreCase("regex")) {
            final Pattern pattern = Pattern.compile(expr);
            return new PathMatcher() {

                public boolean matches(Path path) {
                    // match on file name only
                    Path name = path.getName();
                    if (name == null)
                        return false;
                    return pattern.matcher(name.toString()).matches();
                }};
        }
        throw new UnsupportedOperationException("Syntax '" + syntax +
            "' not recognized");
    }
}
