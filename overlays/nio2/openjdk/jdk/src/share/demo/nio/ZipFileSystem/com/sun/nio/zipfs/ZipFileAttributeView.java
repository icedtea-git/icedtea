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
import java.util.*;

import java.nio.file.FileRef;

public class ZipFileAttributeView extends ZipFileBasicAttributeView {

    /** Creates a new instance of ZipFileAttributeView */
    public ZipFileAttributeView(FileRef file) {
        super(file);
    }


    public String name() {
        return "zip";
    }

    public Object getAttribute(String attribute) throws IOException {
        ZipFileAttributes zfa = readAttributes();
        if (attribute.equals("comment")) {
            return zfa.comment();
        }
        if (attribute.equals("compressedSize")) {
            return zfa.compressedSize();
        }
        if (attribute.equals("crc")) {
            return zfa.crc();
        }
        if (attribute.equals("extraField")) {
            return zfa.extra();
        }
        if (attribute.equals("method")) {
            return zfa.method();
        }
        if (attribute.equals("fileName")) {
            return zfa.name();
        }
        if (attribute.equals("isArchiveFile")) {
            return zfa.isArchiveFile();
        }
        if (attribute.equals("versionMadeBy")) {
            return zfa.versionMadeBy();
        }
        if (attribute.equals("externalAttrs")) {
            return zfa.getExternalAttrs();
        }
        return super.getAttribute(attribute);
    }


    public Map<String, ?> readAttributes(String first, String... rest) throws IOException {
        int rem = rest.length;
        String[] attrs = new String[1 + rem];
        attrs[0] = first;
        if (rem > 0)
            System.arraycopy(rest, 0, attrs, 1, rem);
        Map<String, Object> result = new HashMap<String, Object>();
        result.putAll(super.readAttributes(first, rest));
        ZipFileAttributes zfa = readAttributes();
        boolean added = false;
        for (String attr : attrs) {
            added = addAttribute(result, attr, "comment",zfa.comment());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "compressedSize",zfa.compressedSize());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "crc",zfa.crc());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "extraField",zfa.extra());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "method",zfa.method());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "fileName",zfa.name());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "isArchiveFile",zfa.isArchiveFile());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "versionMadeBy",zfa.versionMadeBy());
            if (added) {
                continue;
            }
            added = addAttribute(result, attr, "externalAttrs",zfa.getExternalAttrs());
            if (attr.equals("*")){
                break;
            }
        }
        return Collections.unmodifiableMap(result);
    }

    private boolean addAttribute(Map<String, Object> result, String attr, String checkAttr,Object value) {
        if ((result.containsKey(checkAttr))) {
            return true;
        }
        if (attr.equals("*") || attr.equals(checkAttr)) {
            result.put(checkAttr, value);
            if (attr.equals("*")) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }


    public ZipFileAttributes readAttributes()
            throws IOException {
        return new ZipFileAttributes(super.getBinding());
    }
}
