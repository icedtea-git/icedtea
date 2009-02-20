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

import java.util.*;
import java.util.concurrent.*;
import java.io.IOException;

import java.nio.file.FileRef;
import java.nio.file.ReadOnlyFileSystemException;

import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.BasicFileAttributeView;

public class ZipFileBasicAttributeView implements
        BasicFileAttributeView {
    // encapsulates the object that we are bound too

    private final FileRef file;

    /** Creates a new instance of ZipFileAttributeView */
    public ZipFileBasicAttributeView(FileRef file) {
        this.file = file;
    }


    public String name() {
        return "basic";
    }


    public Object getAttribute(String attribute) throws IOException {

        BasicFileAttributes bfa = readAttributes();
        if (attribute.equals("lastModifiedTime")) {
            return bfa.lastModifiedTime();
        }
        if (attribute.equals("lastAccessTime")) {
            return bfa.lastAccessTime();
        }
        if (attribute.equals("creationTime")) {
            return bfa.creationTime();
        }
        if (attribute.equals("size")) {
            return bfa.size();
        }
        if (attribute.equals("isRegularFile")) {
            return bfa.isRegularFile();
        }
        if (attribute.equals("isDirectory")) {
            return bfa.isDirectory();
        }
        if (attribute.equals("isSymbolicLink")) {
            return bfa.isSymbolicLink();
        }
        if (attribute.equals("isOther")) {
            return bfa.isOther();
        }
        if (attribute.equals("linkCount")) {
            return bfa.linkCount();
        }
        if (attribute.equals("fileKey")) {
            return bfa.fileKey();
        }
        return null;
    }


    public void setAttribute(String attribute, Object value) {
        throw new ReadOnlyFileSystemException();
    }


    public Map<String, ?> readAttributes(String first, String... rest) throws IOException {
        int rem = rest.length;
        String[] attrs = new String[1 + rem];
        attrs[0] = first;
        if (rem > 0)
            System.arraycopy(rest, 0, attrs, 1, rem);
        Map<String, Object> result = new HashMap<String, Object>();
        BasicFileAttributes ba = readAttributes();
        boolean added = false;
        for (String attr : attrs) {
            added = addAttribute(result, attr, "lastModifiedTime", ba.lastModifiedTime());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "lastAccessTime", ba.lastAccessTime());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "creationTime", ba.creationTime());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "size", ba.size());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "isRegularFile", ba.isRegularFile());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "isDirectory", ba.isDirectory());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "isSymbolicLink", ba.isSymbolicLink());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "isOther", ba.isOther());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "linkCount", ba.linkCount());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "fileKey", ba.fileKey());
            if (added) {
                continue;
            }
            if (attr.equals("*")) {
                break;
            }
        }
        return result;
    }

    FileRef getBinding() {
        FileRef b = file;
        return b;
    }

    public BasicFileAttributes readAttributes()
            throws IOException {
        return new ZipFileBasicAttributes(getBinding());
    }

    public void setTimes(Long lastModifiedTime, Long lastAccessTime, Long createTime, TimeUnit unit) throws IOException {
        throw new ReadOnlyFileSystemException();
    }

    private boolean addAttribute(Map<String, Object> result, String attr, String checkAttr, Object value) {
        if ((result.containsKey(checkAttr))) {
            return true;
        }
        if (attr.equals("*") || attr.equals(checkAttr)) {
            result.put(checkAttr, value);
            if (attr.equals("*")) {
                return false; // not added completely.
            } else {
                return true;
            }
        }
        return false;
    }
}
