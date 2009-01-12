/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
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

/* @test
 * @bug 4313887
 * @summary Unit test for java.nio.file.attribute.NamedAttributeView
 * @library ../..
 */

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.file.*;
import static java.nio.file.LinkOption.*;
import java.nio.file.attribute.*;
import java.util.Iterator;
import java.util.Random;
import java.io.IOException;

public class Basic {

    private static Random rand = new Random();

    private static final String ATTR_NAME = "user.mime_type";
    private static final String ATTR_VALUE = "text/plain";
    private static final String ATTR_VALUE2 = "text/html";

    static boolean hasAttribute(NamedAttributeView view, String attr)
        throws IOException
    {
        for (String name: view.list()) {
            if (name.equals(ATTR_NAME))
                return true;
        }
        return false;
    }

    static void test(Path file, LinkOption... options) throws IOException {
        NamedAttributeView view = file
            .getFileAttributeView(NamedAttributeView.class, options);
        ByteBuffer buf = rand.nextBoolean() ?
            ByteBuffer.allocate(100) : ByteBuffer.allocateDirect(100);

        // Test: write
        buf.put(ATTR_VALUE.getBytes()).flip();
        int size = buf.remaining();
        int nwrote = view.write(ATTR_NAME, buf);
        if (nwrote != size)
            throw new RuntimeException("Unexpected number of bytes written");

        // Test: size
        if (view.size(ATTR_NAME) != size)
            throw new RuntimeException("Unexpected size");

        // Test: read
        buf.clear();
        int nread = view.read(ATTR_NAME, buf);
        if (nread != size)
            throw new RuntimeException("Unexpected number of bytes read");
        buf.flip();
        String value = Charset.defaultCharset().decode(buf).toString();
        if (!value.equals(ATTR_VALUE))
            throw new RuntimeException("Unexpected attribute value");

        // Test: read with insufficient space
        try {
            view.read(ATTR_NAME, ByteBuffer.allocateDirect(size-1));
            throw new RuntimeException("Read expected to fail");
        } catch (IOException x) {
        }

        // Test: replace value
        buf.clear();
        buf.put(ATTR_VALUE2.getBytes()).flip();
        size = buf.remaining();
        view.write(ATTR_NAME, buf);
        if (view.size(ATTR_NAME) != size)
            throw new RuntimeException("Unexpected size");

        // Test: list
        if (!hasAttribute(view, ATTR_NAME))
            throw new RuntimeException("Attribute name not in list");

        // Test: delete
        view.delete(ATTR_NAME);
        if (hasAttribute(view, ATTR_NAME))
            throw new RuntimeException("Attribute name in list");
    }

    static void miscTests(Path file) throws IOException {
        NamedAttributeView view = file
            .getFileAttributeView(NamedAttributeView.class);
        view.write(ATTR_NAME, ByteBuffer.wrap(ATTR_VALUE.getBytes()));

        // NullPointerException
        ByteBuffer buf = ByteBuffer.allocate(100);
        try {
            view.read(null, buf);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }
        try {
            view.read(ATTR_NAME, null);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }
        try {
            view.write(null, buf);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }
        try {
            view.write(ATTR_NAME, null);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }
        try {
            view.size(null);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }
        try {
            view.delete(null);
            throw new RuntimeException("NPE expected");
        } catch (NullPointerException x) { }

        // Read-only buffer
        buf = ByteBuffer.wrap(ATTR_VALUE.getBytes()).asReadOnlyBuffer();
        view.write(ATTR_NAME, buf);
        buf.flip();
        try {
            view.read(ATTR_NAME, buf);
            throw new RuntimeException("IAE expected");
        } catch (IllegalArgumentException x) { }

        // Zero bytes remaining
        buf = ByteBuffer.allocateDirect(100);
        buf.position(buf.capacity());
        try {
            view.read(ATTR_NAME, buf);
            throw new RuntimeException("IOE expected");
        } catch (IOException x) { }
    }

    public static void main(String[] args) throws IOException {
        // create temporary directory to run tests
        Path dir = TestUtil.createTemporaryDirectory();
        try {
            if (!dir.getFileStore().supportsFileAttributeView("xattr")) {
                System.out.println("NamedAttributeView not supported - skip test");
                return;
            }

            // test access to named attributes of regular file
            Path file = dir.resolve("foo.html").createFile();
            try {
                test(file);
            } finally {
                file.delete();
            }

            // test access to named attributes of directory
            file = dir.resolve("foo").createDirectory();
            try {
                test(file);
            } finally {
                file.delete();
            }

            // test access to named attributes of sym link
            if (TestUtil.supportsLinks(dir)) {
                Path target = dir.resolve("doesnotexist");
                Path link = dir.resolve("link").createSymbolicLink(target);
                try {
                    test(link, NOFOLLOW_LINKS);
                } catch (IOException x) {
                    // access to attributes of sym link may not be supported
                } finally {
                    link.delete();
                }
            }

            // misc. tests
            try {
                file = dir.resolve("foo.txt").createFile();
                miscTests(dir);
            } finally {
                file.delete();
            }

        } finally {
            TestUtil.removeAll(dir);
        }
    }
 }
