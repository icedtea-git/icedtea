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

import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedExceptionAction;
import java.security.PrivilegedActionException;
import java.io.IOException;
import java.util.*;

import java.nio.file.ClosedWatchServiceException;
import java.nio.file.DirectoryStream;
import java.nio.file.FileRef;
import java.nio.file.LinkOption;
import java.nio.file.NotDirectoryException;
import java.nio.file.Path;
import java.nio.file.StandardWatchEventKind;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;

import java.nio.file.attribute.Attributes;
import java.nio.file.attribute.BasicFileAttributes;

/**
 * Simple WatchService implementation that uses background thread to poll
 * registered directories for changes.  This implementation is for use
 * on operating systems that do not have file change notification support.
 */

class PollingWatchService
    extends AbstractWatchService
{
    // keys registered with the watch service
    private final List<PollingWatchKey> keys;

    private String sleepTimePropValue;
    private final int sleepTime;

    PollingWatchService() {
        this.keys = new ArrayList<PollingWatchKey>();

        // start background daemon thread to poll registered files
        AccessController.doPrivileged(new PrivilegedAction<Void>() {

            public Void run() {
                sleepTimePropValue = System.
                    getProperty("sun.nio.fs.PollingWatchService.sleepTime");

                Thread thr = new Thread(new Poller());
                thr.setDaemon(true);
                thr.start();
                return null;
            }
         });

         // use property value to configure sleep time if set
         int t = 0;
         if (sleepTimePropValue != null) {
             try {
                t = Integer.parseInt(sleepTimePropValue);
             } catch (NumberFormatException x) {
             }
         }
         if (t <= 0)
             t = 2000;  // default to 2 seconds.
         this.sleepTime = t;
    }

    /**
     * Register the given file with this watch service
     */

    WatchKey register(final Path path,
                      WatchEvent.Kind<?>[] events,
                      WatchEvent.Modifier... modifiers)
         throws IOException
    {
        // check events - CCE will be thrown if there are invalid elements
        if (events.length == 0)
            throw new IllegalArgumentException("No events to register");
        final Set<WatchEvent.Kind<?>> eventSet =
            new HashSet<WatchEvent.Kind<?>>(events.length);
        for (WatchEvent.Kind<?> event: events) {
            // standard events
            if (event == StandardWatchEventKind.ENTRY_CREATE ||
                event == StandardWatchEventKind.ENTRY_MODIFY ||
                event == StandardWatchEventKind.ENTRY_DELETE)
            {
                eventSet.add(event);
                continue;
            }

            // OVERFLOW is ignored
            if (event == StandardWatchEventKind.OVERFLOW) {
                if (events.length == 1)
                    throw new IllegalArgumentException("No events to register");
                continue;
            }

            // null/unsupported
            if (event == null)
                throw new NullPointerException("An element in event set is 'null'");
            throw new UnsupportedOperationException(event.name());
        }

        // no modifiers supported at this time
        if (modifiers.length > 0) {
            if (modifiers[0] == null)
                throw new NullPointerException();
            throw new UnsupportedOperationException("Modifier not supported");
        }

        // check if watch service is closed
        if (!isOpen())
            throw new ClosedWatchServiceException();

        // registration is done in privileged block as it requires the
        // attributes of the entries in the directory.
        try {
            return AccessController.doPrivileged(
                new PrivilegedExceptionAction<PollingWatchKey>() {

                    public PollingWatchKey run() throws IOException {
                        return doPrivilegedRegister(path, eventSet);
                    }
                });
        } catch (PrivilegedActionException pae) {
            Throwable cause = pae.getCause();
            if (cause != null && cause instanceof IOException)
                throw (IOException)cause;
            throw new AssertionError(pae);
        }
    }

    // registers directory returning a new key if not already registered or
    // existing key if already registered
    private PollingWatchKey doPrivilegedRegister(Path path,
                                                 Set<? extends WatchEvent.Kind<?>> events)
        throws IOException
    {
        // check file is a directory and get its file key if possible
        BasicFileAttributes attrs = Attributes.readBasicFileAttributes(path);
        if (!attrs.isDirectory()) {
            throw new NotDirectoryException(path.toString());
        }
        Object fileKey = attrs.fileKey();

        // Iterate over existing keys to check if file is already registered.
        // Return existing key or create new key.
        synchronized (keys) {
            for (PollingWatchKey key: keys) {
                Object otherFileKey = key.fileKey();

                boolean isSame = false;
                if (fileKey != null && otherFileKey != null) {
                    isSame = fileKey.equals(otherFileKey);
                } else {
                    try {
                        isSame = path.isSameFile(key.directory());
                    } catch (IOException x) {
                        // one of the files is not accessible
                    }
                }

                // if key exists then update events and return it
                if (isSame) {
                    key.setEvents(events);
                    return key;
                }
            }

            // create new key
            synchronized (closeLock()) {
                if (!isOpen())
                    throw new ClosedWatchServiceException();
                PollingWatchKey newKey =
                    new PollingWatchKey(this, path, events, fileKey);
                keys.add(newKey);
                return newKey;
            }
        }
    }


    void implClose() throws IOException {
        synchronized (keys) {
            Iterator<PollingWatchKey> i = keys.iterator();
            while (i.hasNext()) {
                i.next().invalidate();
                i.remove();
            }
        }
    }

    /**
     * Polling thread to poll each WatchKey registered with the WatchService
     */
    private class Poller implements Runnable {
        public void run() {
            for (;;) {
                try {
                    Thread.sleep(sleepTime);
                } catch (InterruptedException x) { }

                // watch service has been closed
                if (!isOpen()) {
                    return;
                }

                // take a snapshot of the keys to poll - this allows new registrations
                // to be added at the same time as polling.
                List<PollingWatchKey> shadowKeys;
                synchronized (keys) {
                    shadowKeys = new ArrayList<PollingWatchKey>(keys);
                }

                // poll each registered key
                for (PollingWatchKey key: shadowKeys) {
                    key.poll();
                }
            }
        }
    }

    /**
     * Entry in directory cache to record file last-modified-time and tick-count
     */
    private static class CacheEntry {
        private long lastModified;
        private int lastTickCount;

        CacheEntry(long lastModified, int lastTickCount) {
            this.lastModified = lastModified;
            this.lastTickCount = lastTickCount;
        }

        int lastTickCount() {
            return lastTickCount;
        }

        long lastModified() {
            return lastModified;
        }

        void update(long lastModified, int tickCount) {
            this.lastModified = lastModified;
            this.lastTickCount = tickCount;
        }
    }

    /**
     * WatchKey implementation that encapsulates a map of the entries of the
     * entries in the directory. Polling the key causes it to re-scan the
     * directory and queue keys when entries are added, modified, or deleted.
     */
    private class PollingWatchKey extends AbstractWatchKey {
        private final Path dir;
        private final Object fileKey;
        private Set<? extends WatchEvent.Kind<?>> events;
        private volatile boolean valid;
        private int tickCount;

        // map of entries in directory
        private Map<Path,CacheEntry> entries;

        PollingWatchKey(PollingWatchService watcher,
                        Path dir,
                        Set<? extends WatchEvent.Kind<?>> events,
                        Object fileKey)
            throws IOException
        {
            super(watcher);
            this.dir = dir;
            this.fileKey = fileKey;
            this.events = events;
            this.valid = true;
            this.tickCount = 0;
            this.entries = new HashMap<Path,CacheEntry>();

            // get the initial entries in the directory
            DirectoryStream<Path> stream = dir.newDirectoryStream();
            try {
                for (Path entry: stream) {
                    // don't follow links
                    long lastModified = Attributes
                        .readBasicFileAttributes(entry, LinkOption.NOFOLLOW_LINKS)
                        .lastModifiedTime();
                    entries.put(entry.getName(),
                                new CacheEntry(lastModified, tickCount));
                }
            } catch (ConcurrentModificationException cme) {
                // thrown if directory iteration fails
                Throwable cause = cme.getCause();
                if (cause != null && cause instanceof IOException)
                    throw (IOException)cause;
                throw new AssertionError(cme);
            } finally {
                stream.close();
            }
        }

        FileRef directory() {
            return dir;
        }

        Object fileKey() {
            return fileKey;
        }

        synchronized void setEvents(Set<? extends WatchEvent.Kind<?>> events) {
            this.events = events;
        }


        public boolean isValid() {
            return valid;
        }

        void invalidate() {
            valid = false;
        }


        public void cancel() {
            valid = false;
            synchronized (keys) {
                keys.remove(this);
            }
        }

        /**
         * Polls the directory to detect for new files, modified files, or
         * deleted files.
         */
        synchronized void poll() {
            if (!valid) {
                return;
            }

            // update tick
            tickCount++;

            // open directory
            DirectoryStream<Path> stream = null;
            try {
                stream = dir.newDirectoryStream();
            } catch (IOException x) {
                // directory is no longer accessible so cancel key
                cancel();
                signal();
                return;
            }

            // iterate over all entries in directory
            try {
                for (Path entry: stream) {
                    long lastModified = 0L;
                    try {
                        lastModified = Attributes
                            .readBasicFileAttributes(entry, LinkOption.NOFOLLOW_LINKS)
                            .lastModifiedTime();
                    } catch (IOException x) {
                        // unable to get attributes of entry. If file has just
                        // been deleted then we'll report it as deleted on the
                        // next poll
                        continue;
                    }

                    // lookup cache
                    CacheEntry e = entries.get(entry.getName());
                    if (e == null) {
                        // new file found
                        entries.put(entry.getName(),
                                     new CacheEntry(lastModified, tickCount));

                        // queue ENTRY_CREATE if event enabled
                        if (events.contains(StandardWatchEventKind.ENTRY_CREATE)) {
                            signalEvent(StandardWatchEventKind.ENTRY_CREATE, entry.getName());
                            continue;
                        } else {
                            // if ENTRY_CREATE is not enabled and ENTRY_MODIFY is
                            // enabled then queue event to avoid missing out on
                            // modifications to the file immediately after it is
                            // created.
                            if (events.contains(StandardWatchEventKind.ENTRY_MODIFY)) {
                                signalEvent(StandardWatchEventKind.ENTRY_MODIFY, entry.getName());
                            }
                        }
                        continue;
                    }

                    // check if file has changed
                    if (e.lastModified != lastModified) {
                        if (events.contains(StandardWatchEventKind.ENTRY_MODIFY)) {
                            signalEvent(StandardWatchEventKind.ENTRY_MODIFY, entry.getName());
                        }
                    }
                    // entry in cache so update poll time
                    e.update(lastModified, tickCount);

                }
            } catch (ConcurrentModificationException x) {
                // FIXME - should handle this
            } finally {

                // close directory stream
                try {
                    stream.close();
                } catch (IOException x) {
                    // ignore
                }
            }

            // iterate over cache to detect entries that have been deleted
            Iterator<Path> i = entries.keySet().iterator();
            while (i.hasNext()) {
                Path name = i.next();
                CacheEntry entry = entries.get(name);

                if (entry.lastTickCount() != tickCount) {
                    // remove from map and queue delete event (if enabled)
                    i.remove();
                    if (events.contains(StandardWatchEventKind.ENTRY_DELETE)) {
                        signalEvent(StandardWatchEventKind.ENTRY_DELETE, name);
                    }
                }
            }
        }
    }
}
