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

import java.util.concurrent.*;
import java.io.IOException;
import java.util.Calendar;

import org.classpath.icedtea.java.nio.file.ClosedFileSystemException;
import org.classpath.icedtea.java.nio.file.FileRef;
import org.classpath.icedtea.java.nio.file.ReadOnlyFileSystemException;

import org.classpath.icedtea.java.nio.file.attribute.BasicFileAttributes;

public class ZipFileBasicAttributes implements
        BasicFileAttributes {

    FileRef file;
    ZipEntryInfo ze;

    /** Creates a new instance of ZipFileAttributes */
    public ZipFileBasicAttributes(FileRef file)
            throws IOException {
        this.file = file;
        ensureOpen();
        ze = ZipUtils.getEntry(file);
    }

    void ensureOpen() {
        if (file instanceof ZipFilePath && !((ZipFilePath) file).getFileSystem().isOpen()) {
            throw new ClosedFileSystemException();
        }
    }


    public long creationTime() {
        return ze.createTime;
    }

    public boolean isDirectory() {
        return ze.isDirectory;
    }

    public boolean isLink() {
        return false;
    }

    public boolean isOther() {
        return ze.isOtherFile;
    }

    public boolean isRegularFile() {
        return ze.isRegularFile;
    }

    public long lastAccessTime() {
        return ze.lastAccessTime;
    }

    public long lastModifiedTime() {
        long time = ze.lastModifiedTime;
        Calendar cal = dosTimeToJavaTime(time);
        return cal.getTimeInMillis();
    }

    private Calendar dosTimeToJavaTime(long time) {
        Calendar cal = Calendar.getInstance();
        cal.set((int) (((time >> 25) & 0x7f) + 1980),
                (int) (((time >> 21) & 0x0f) - 1),
                (int) ((time >> 16) & 0x1f),
                (int) ((time >> 11) & 0x1f),
                (int) ((time >> 5) & 0x3f),
                (int) ((time << 1) & 0x3e));
        return cal;
    }

    public int linkCount() {
        return 0;
    }

    public TimeUnit resolution() {
        return TimeUnit.MILLISECONDS;
    }

    public long size() {
        return ze.size;
    }

    public boolean isSymbolicLink() {
        return false;
    }

    public Object fileKey() {
        return null;
    }
}
