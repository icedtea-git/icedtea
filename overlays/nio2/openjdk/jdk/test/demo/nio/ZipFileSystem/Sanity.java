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

import java.util.*;
import java.net.URI;
import java.io.IOException;

import org.classpath.icedtea.java.nio.file.FileSystem;
import org.classpath.icedtea.java.nio.file.FileVisitResult;
import org.classpath.icedtea.java.nio.file.Path;
import org.classpath.icedtea.java.nio.file.Paths;

import org.classpath.icedtea.java.nio.file.attribute.Attributes;

import org.classpath.icedtea.java.nio.file.spi.FileSystemProvider;

/**
 * Sanity check zip provider by running a few simple tests.
 */

public class Sanity {
    static int count;

    public static void main(String[] args) throws Exception {
        Path zipfile = Paths.get(args[0]);

        // Test: zip should should be returned in provider list
        boolean found = false;
        for (FileSystemProvider provider: FileSystemProvider.installedProviders()) {
            if (provider.getScheme().equalsIgnoreCase("zip")) {
                found = true;
                break;
            }
        }
        if (!found)
            throw new RuntimeException("'zip' provider not installed");

        // Test: FileSystems#newFileSystem(FileRef)
        Map<String,?> env = new HashMap<String,Object>();
        FileSystems.newFileSystem(zipfile, env, null).close();

        // Test: FileSystems#newFileSystem(URI)
        URI uri = URI.create("zip" + zipfile.toUri().toString().substring(4));
        FileSystem fs = FileSystems.newFileSystem(uri, env, null);

        // Test: exercise toUri method
        String expected = uri.toString() + "#/foo";
        String actual = fs.getPath("/foo").toUri().toString();
        if (!actual.equals(expected)) {
            throw new RuntimeException("toUri returned '" + actual +
                "', expected '" + expected + "'");
        }

        // Test: exercise directory iterator and retrieval of basic attributes
        Files.walkFileTree(fs.getPath("/"), new FileTreePrinter());

        // Test: DirectoryStream with glob
        count = 0;
        Files.withDirectory(fs.getPath("/"), "M*-INF", new FileAction<Path>() {
            public void invoke(Path file) {
                if (file.toString().equals("META-INF"))
                    throw new RuntimeException("Unexpected match: " + file);
                count++;
            }
        });
        if (count != 1)
            throw new RuntimeException("Expected to match 1 file in directory");

        // Test: copy file from zip file to current (scratch) directory
        Path source = fs.getPath("/META-INF/services/java.nio.file.spi.FileSystemProvider");
        if (source.exists()) {
            Path target = Paths.get(source.getName().toString());
            source.copyTo(target, StandardCopyOption.REPLACE_EXISTING);
            try {
                long s1 = Attributes.readBasicFileAttributes(source, true).size();
                long s2 = Attributes.readBasicFileAttributes(target, true).size();
                if (s2 != s1)
                    throw new RuntimeException("target size != source size");
            } finally {
                target.delete(true);
            }
        }

        // Test: readAttributes
        Map<String,?> attrs = Attributes.readAttributes(source, true, "jar:*");
        if (attrs.get("manifestAttributes") == null)
            throw new RuntimeException("manifestAttributes missing");
        long s1 = (Long)attrs.get("size");
        long s2 = Attributes.readBasicFileAttributes(source, true).size();
        if (s2 != s1)
            throw new RuntimeException("size mis-match");

        // Test: FileStore
        FileStore store = fs.getPath("/").getFileStore();
        if (!store.supportsFileAttributeView("basic"))
            throw new RuntimeException("BasicFileAttributeView should be supported");

        // Test: ClosedFileSystemException
        fs.close();
        if (fs.isOpen())
            throw new RuntimeException("FileSystem should be closed");
        try {
            fs.getPath("/missing").checkAccess(AccessMode.READ);
        } catch (ClosedFileSystemException x) { }
    }

    // FileVisitor that pretty prints a file tree
    static class FileTreePrinter extends SimpleFileVisitor<Path> {
        private int indent = 0;

        private void indent() {
            StringBuilder sb = new StringBuilder(indent);
            for (int i=0; i<indent; i++) sb.append(" ");
            System.out.print(sb);
        }


        public FileVisitResult preVisitDirectory(Path dir) {
            if (dir.getName() != null) {
                indent();
                System.out.println(dir.getName() + "/");
                indent++;
            }
            return FileVisitResult.CONTINUE;
        }


        public FileVisitResult visitFile(Path file,
                                         BasicFileAttributes attrs)
        {
            indent();
            System.out.print(file.getName());
            if (attrs.isRegularFile())
                System.out.format(" (%d)", attrs.size());
            System.out.println();
            return FileVisitResult.CONTINUE;
        }


        public FileVisitResult postVisitDirectory(Path dir, IOException exc) {
            if (exc != null)
                super.postVisitDirectory(dir, exc);
            if (dir.getName() != null)
                indent--;
            return FileVisitResult.CONTINUE;
        }
    }
}
