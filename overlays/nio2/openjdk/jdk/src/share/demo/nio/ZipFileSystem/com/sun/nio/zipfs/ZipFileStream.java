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

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Set;
import java.util.Map;
import java.io.IOException;
import java.util.HashSet;

import org.classpath.icedtea.java.nio.file.DirectoryStream;
import org.classpath.icedtea.java.nio.file.NotDirectoryException;
import org.classpath.icedtea.java.nio.file.Path;
import org.classpath.icedtea.java.nio.file.ReadOnlyFileSystemException;

public class ZipFileStream implements DirectoryStream<Path> {

    private final ZipFilePath zipPath;
    private final DirectoryStream.Filter<? super Path> filter;
    private volatile boolean isOpen;
    private final Object closeLock;
    private Iterator<Path> iterator;

    private class ZipFilePathIterator implements
            Iterator<Path> {

        private boolean atEof;
        private Path nextEntry;
        private Path prevEntry;
        private Iterator<Path> entryIterator;

        ZipFilePathIterator() throws IOException {
            atEof = false;
            Map<ZipFilePath, ZipEntryInfo> entries = null;
            int nameCount = zipPath.getNameCount();
            entries = ZipUtils.getEntries(zipPath);
            Set<ZipFilePath> s = entries.keySet();
            Set<Path> s1 = new HashSet<Path>();
            for (ZipFilePath f : s) {

                boolean b = f.startsWith(zipPath);
                if ((nameCount + 1) > f.getNameCount() || !b) {
                    continue;
                }
                ZipFilePath entry = zipPath.resolve(f.getName(nameCount));
                if (filter == null || filter.accept(entry)) {
                    s1.add(entry);
                }
            }
            if (s1.isEmpty()) {
            // if there is no file keep quiet
            }
            entryIterator = s1.iterator();
        }

        @SuppressWarnings("unchecked")
        private boolean accept(Path entry) {
            return filter.accept(entry);
        }

        private Path readNextEntry() {
            Path entry = entryIterator.next();
            if ((filter == null) || accept(entry)) {
                return entry;
            }
            return null;
        }

        public synchronized boolean hasNext() {
            boolean isThereNext = entryIterator.hasNext();
            if (!isThereNext) {
                atEof = true;
            }
            return isThereNext;
        }

        public synchronized Path next() {
            if (nextEntry == null) {
                if (!atEof) {
                    nextEntry = readNextEntry();
                }
                if (nextEntry == null) {
                    atEof = true;
                    throw new NoSuchElementException();
                }
            }
            prevEntry = nextEntry;
            nextEntry = null;
            return prevEntry;
        }

        public void remove() {
            UnsupportedOperationException e = new UnsupportedOperationException();
            e.initCause(new ReadOnlyFileSystemException());
            throw e;
        }
    }


    public Iterator<Path> iterator() {
        synchronized (this) {
            if (iterator != null) {
                throw new IllegalStateException();
            }
            try {
                iterator = new ZipFilePathIterator();
            } catch (IOException e) {
                IllegalStateException ie = new IllegalStateException();
                ie.initCause(e);
                throw ie;
            }
            return iterator;
        }
    }

    public void close() throws IOException {
    // no impl
    }

    /** Creates a new instance of ZipFileStream */
    public ZipFileStream(ZipFilePath zipPath,
            DirectoryStream.Filter<? super Path> filter)
            throws IOException {

        if (zipPath.getNameCount() != 0) { // if path is '/' no need for check existence
            zipPath.checkAccess();
        }

        if (!zipPath.isArchiveFile() && !zipPath.isDirectory()) {
            throw new NotDirectoryException("Not a Directory " + zipPath.toString());
        }
        this.zipPath = zipPath;
        this.filter = filter;
        this.isOpen = true;
        this.closeLock = new Object();
    }
}
