/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
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
package com.sun.javatest.util;

import java.io.File;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

/**
 * An object to cache details about a file. Entries in the cache
 * automatically expire if the file on disk is newer than the entry
 * in the cache, and may optionally expire after a nominated timeout.
 */
public class FileInfoCache
{
    /**
     * Create a FileInfoCache object.
     */
    public FileInfoCache() {
        map = new HashMap();
        // This next line is a workaround for a bug in URLClassLoader
        // that is provoked when FileInfoCache is used in conjunction
        // with JFileChooser (and more accurately, BasicDirectoryModel.)
        // The bug occurs when BasicDirectoryModel interrupts its loader
        // thread; the bug manifests itself as IndexOutOfBoundsException
        // inside URLClassLoader when trying to do "new Entry" inside
        // put. The workaround is to proactively load the class here.
        // The bug is in JDK 1.3.0_O (at least).
        Class c = Entry.class;
    }

    /**
     * Create a FileInfoCache object, specifying a timeout period for
     * entries in the cache.
     * @param timeout an interval, specified in milliseconds, after which, entries in the cache
     * will expire
     */
    public FileInfoCache(int timeout) {
        this();
        this.timeout = timeout;
    }

    /**
     * Put a new entry for a file in the cache. Any previous entry is overwritten.
     * @param f the file for which the entry is to be recorded in the cache
     * @param o the object to be saved in the cache for the given file
     */
    public synchronized void put(File f, Object o) {
        map.put(f, new Entry(f, o));

        if (timeout > 0) {
            long now = System.currentTimeMillis();
            long old = now - timeout;
            for (Iterator i = map.values().iterator(); i.hasNext(); ) {
                Entry e = (Entry) (i.next());
                if (e.lastUsed < old)
                    i.remove();
            }
        }
    }

    /**
     * Get an entry for a file from the cache, checking that the entry has not
     * timed out, or expired because the file on disk is newer than an entry in
     * the cache.
     * @param f the file for which to retrieve an entry from the cache
     * @return the value previously put in the cache for the given file, or null
     * if there is no current entry
     */
    public synchronized Object get(File f) {
        Entry e = (Entry) (map.get(f));
        if (e == null) {
            // not in cache
            return null;
        }
        else if (e.fileLastModified != f.lastModified()) {
            // cache out of date; file has been modified
            map.remove(f);
            return null;
        }
        else {
            // got valid entry
            e.lastUsed = System.currentTimeMillis();
            return e.value;
        }
    }

    /**
     * Clear all entries from the cache.
     */
    public synchronized void clear() {
        map.clear();
    }

    private Map map;
    private int timeout;

    private static class Entry {
        Entry(File f, Object value) {
            fileLastModified = f.lastModified();
            lastUsed = System.currentTimeMillis();
            this.value = value;
        }

        long fileLastModified;
        long lastUsed;
        Object value;
    }
}
