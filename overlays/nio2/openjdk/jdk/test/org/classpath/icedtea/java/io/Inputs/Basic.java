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
 * @summary Basic unit test for java.io.Inputs class
 */

import java.io.*;
import java.util.*;
import java.nio.charset.UnsupportedCharsetException;
import java.nio.charset.MalformedInputException;

import org.classpath.icedtea.java.io.Inputs;

import org.classpath.icedtea.java.nio.file.Path;

public class Basic {
    static final Random rand = new Random();

    public static void main(String[] args) throws IOException {
        Path dir = createTempDir();
        try {
            testReadAllBytesMethods(dir);
            testReadAllLinesMethods(dir);
        } finally {
            dir.delete();
        }
    }

    static void testReadAllBytesMethods(Path dir) throws IOException {
        // create file with random bytes
        byte[] bytes = new byte[1 + rand.nextInt(64*1024)];
        rand.nextBytes(bytes);
        Path file = dir.resolve("foo");
        Outputs.write(file, bytes);

        try {
            byte[] result;

            // check all bytes are read
            result = Inputs.readAllBytes(file);
            if (!Arrays.equals(bytes, result))
                throw new RuntimeException("Unexpected bytes");
            result = Inputs.readAllBytes(new File(file.toString()));
            if (!Arrays.equals(bytes, result))
                throw new RuntimeException("Unexpected bytes");

            // via URL connection
            InputStream in = file.toUri().toURL().openConnection().getInputStream();
            try {
                result = Inputs.readAllBytes(in);
            if (!Arrays.equals(bytes, result))
                throw new RuntimeException("Unexpected bytes");
            } finally {
                in.close();
            }

            // test zero-length file
            Outputs.write(file, new byte[0]);
            if (Inputs.readAllBytes(file).length != 0)
                throw new RuntimeException("Unexpected bytes");

            // NullPointerException
            try {
                Inputs.readAllBytes((FileRef)null);
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }

            try {
                Inputs.readAllBytes((File)null);
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }

            try {
                Inputs.readAllBytes((InputStream)null);
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }

        } finally {
            file.delete();
        }
    }

    static void testReadAllLinesMethods(Path dir) throws IOException {
        String[] poem = { "I met a traveler from an antique land",
                          "Who said: Two vast and trunkless legs of stone",
                          "Stand in the desert. Near them, on the sand",
                          "Half sunk, a shattered visage lies, whose frown,",
                          "And wrinkled lip, and sneer of cold command,",
                          "Tell that its sculptor well those passions read",
                          "Which yet survive, stamped on these lifeless things,",
                          "The hand that mocked them, and the heart that fed;",
                          "And on the pedestal these words appear:",
                          "My name is Ozymandias, king of kings:",
                          "Look upon my works, ye Mighty, and despair!",
                          "Nothing beside remains. Round the decay",
                          "Of that colossal wreck, boundless and bare",
                          "The lone and level sands stretch far away." };
        List<String> poemAsList = Arrays.asList(poem);

        Path file = dir.resolve("Shelley");
        try {
            Outputs.writeLines(file, poem);

            checkEquals(poemAsList, Inputs.readAllLines(file));
            checkEquals(poemAsList, Inputs.readAllLines(file, "UTF-8"));
            checkEquals(poemAsList, Inputs.readAllLines(new File(file.toString())));
            checkEquals(poemAsList,
                Inputs.readAllLines(new File(file.toString()), "UTF-8"));
            InputStream in = file.newInputStream();
            try {
                checkEquals(poemAsList, Inputs.readAllLines(in));
            } finally {
                in.close();
            }
            InputStreamReader reader = new InputStreamReader(file.newInputStream());
            try {
                checkEquals(poemAsList, Inputs.readAllLines(reader));
            } finally {
                reader.close();
            }

            // IOException
            try {
                Inputs.readAllLines(dir.resolve("doesNotExist"));
                throw new RuntimeException("IOException expected");
            } catch (IOException ignore) { }

            // MalformedInputException
            OutputStream out = file.newOutputStream();
            try {
                out.write((byte)0xC2);  // malformed 2-byte sequence
                out.write((byte)0x00);
            } finally {
                out.close();
            }
            try {
                Inputs.readAllLines(file, "UTF-8");
                throw new RuntimeException("MalformedInputException expected");
            } catch (MalformedInputException ignore) { }

            // UnsupportedCharsetException
            try {
                Inputs.readAllLines(file, "BAD-CHARSET");
                throw new RuntimeException("UnsupportedCharsetException expected");
            } catch (UnsupportedCharsetException ignore) { }

            try {
                Inputs.readAllLines(new File(file.toString()), "BAD-CHARSET");
                throw new RuntimeException("UnsupportedCharsetException expected");
            } catch (UnsupportedCharsetException ignore) { }
            in = file.newInputStream();
            try {
                try {
                    Inputs.readAllLines(in, "BAD-CHARSET");
                    throw new RuntimeException("UnsupportedCharsetException expected");
                } catch (UnsupportedCharsetException ignore) { }
            } finally {
                in.close();
            }

            // NulPointerException
            try {
                Inputs.readAllLines((FileRef)null);
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }
            try {
                Inputs.readAllLines((FileRef)null, "UTF-8");
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }

            try {
                Inputs.readAllLines(file, null);
                throw new RuntimeException("NullPointerException expected");
            } catch (NullPointerException npe) { }

        } finally {
            file.delete();
        }
    }

    static void checkEquals(List<String> expected, List<String> actual) {
        if (!actual.equals(expected))
            throw new RuntimeException();
    }


    static Path createTempDir() throws IOException {
        Path tmpdir = Paths.get(System.getProperty("java.io.tmpdir"));
        Random r = new Random();

        Path dir;
        do {
            dir = tmpdir.resolve("name" + r.nextInt());
        } while (dir.exists());
        return dir.createDirectory();
    }
}
