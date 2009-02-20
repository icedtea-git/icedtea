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

import java.io.IOException;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

import java.nio.file.FileStore;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.ProviderMismatchException;
import java.nio.file.attribute.FileAttributeView;
import java.nio.file.attribute.FileStoreAttributeView;
import java.nio.file.attribute.FileStoreAttributeView;
import java.nio.file.attribute.FileStoreSpaceAttributeView;
import java.nio.file.attribute.FileStoreSpaceAttributes;
import java.nio.file.attribute.Attributes;
import java.nio.file.attribute.BasicFileAttributeView;

public class ZipFileStore extends FileStore {

    private final ZipFilePath root;
    private final String zipFileName;
    private final String type = "zipfs";

    ZipFileStore(ZipFilePath path) {
        this.root = path;
        zipFileName = path.getFileSystem().getZipFileSystemFile();
    }

    static FileStore create(ZipFilePath root) throws IOException {
        return new ZipFileStore(root);
    }


    public String name() {
        return zipFileName;
    }


    public String type() {
        return type;
    }


    public boolean isReadOnly() {
        return root.getFileSystem().isReadOnly();
    }


    public boolean supportsFileAttributeView(Class<? extends FileAttributeView> type) {
        if (type == BasicFileAttributeView.class)
            return true;
        if (type == ZipFileAttributeView.class)
            return true;
        if (type == JarFileAttributeView.class)
            return true;
        return false;
    }


    public boolean supportsFileAttributeView(String name) {
        // FIXME
        if (name.equals("basic") || name.equals("zip") || name.equals("jar")) {
            return true;
        }
        return false;
    }


    @SuppressWarnings("unchecked")
    public <V extends FileStoreAttributeView> V getFileStoreAttributeView(Class<V> viewType) {
        if (viewType == FileStoreSpaceAttributeView.class) {
            return (V) new ZipFileStoreAttributeView(this);
        }
        return null;
    }


    public FileStoreAttributeView getFileStoreAttributeView(String name) {
        if (name.equals("space")) {
            return new ZipFileStoreAttributeView(this);
        }
        return null;
    }

    private static class ZipFileStoreAttributeView implements FileStoreSpaceAttributeView {

        private final ZipFileStore fileStore;

        public ZipFileStoreAttributeView(ZipFileStore fileStore) {
            this.fileStore = fileStore;
        }


        public String name() {
            return "space";
        }


        public Object getAttribute(String attribute) throws IOException {
            FileStoreSpaceAttributes attrs = readAttributes();
            if (attribute.equals("totalSpace")) {
                return attrs.totalSpace();
            }
            if (attribute.equals("unallocatedSpace")) {
                return attrs.unallocatedSpace();
            }
            if (attribute.equals("usableSpace")) {
                return attrs.usableSpace();
            }

            return null;
        }


        public void setAttribute(String attribute, Object value) {
            throw new UnsupportedOperationException();
        }

        private static final String TOTAL_SPACE_NAME = "totalSpace";
        private static final String USABLE_SPACE_NAME = "usableSpace";
        private static final String UNALLOCATED_SPACE_NAME = "unallocatedSpace";


        public Map<String, ?> readAttributes(String first, String... rest) throws IOException {
            boolean total = false;
            boolean usable = false;
            boolean unallocated = false;

            if (first.equals(TOTAL_SPACE_NAME)) total = true;
            else if (first.equals(USABLE_SPACE_NAME)) usable = true;
            else if (first.equals(UNALLOCATED_SPACE_NAME)) unallocated = true;
            else if (first.equals("*")) {
                total = true;
                usable = true;
                unallocated = true;
            }

            if (!total || !usable || !unallocated) {
              for (String attribute: rest) {
                if (attribute.equals("*")) {
                    total = true;
                    usable = true;
                    unallocated = true;
                    break;
                }
                if (attribute.equals(TOTAL_SPACE_NAME)) {
                    total = true;
                    continue;
                }
                if (attribute.equals(USABLE_SPACE_NAME)) {
                    usable = true;
                    continue;
                }
                if (attribute.equals(UNALLOCATED_SPACE_NAME)) {
                    unallocated = true;
                    continue;
                }
              }
            }

            FileStoreSpaceAttributes attrs = readAttributes();
            Map<String,Object> result = new HashMap<String,Object>(2);
            if (total)
                result.put(TOTAL_SPACE_NAME, attrs.totalSpace());
            if (usable)
                result.put(USABLE_SPACE_NAME, attrs.usableSpace());
            if (unallocated)
                result.put(UNALLOCATED_SPACE_NAME, attrs.unallocatedSpace());
            return result;
        }

        public FileStoreSpaceAttributes readAttributes() throws IOException {
            // get the size of the zip file
            String file = fileStore.name();
            Path path = FileSystems.getDefault().getPath(file);
            final long size = Attributes.readBasicFileAttributes(path).size();
            return new FileStoreSpaceAttributes() {

                public long totalSpace() {
                    return size; // size of the zip/jar file

                }

                public long usableSpace() {
                    return 0; // no usable space in zip/jar file

                }

                public long unallocatedSpace() {
                    return 0; // no unallocated space in zip/jar file.

                }
            };

        }
    }
}
