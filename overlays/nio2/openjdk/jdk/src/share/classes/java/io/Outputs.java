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
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA conne02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

package java.io;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.UnsupportedCharsetException;
import java.nio.channels.Channels;
import java.nio.channels.WritableByteChannel;
import java.util.Arrays;
import java.util.List;

import java.nio.file.FileRef;

import static java.nio.file.StandardOpenOption.CREATE;
import static java.nio.file.StandardOpenOption.TRUNCATE_EXISTING;
import static java.nio.file.StandardOpenOption.WRITE;

/**
 * {@note experimental}
 * This class consists exclusively of static methods that operate on output
 * destinations.
 *
 * <p> The methods to write lines of text output a line terminator following
 * each line. The line terminator that is output is platform line terminated,
 * as defined by the {@code line.separator} system property.
 *
 * @since 1.7
 */

public final class Outputs {
    private Outputs() { }

    // checks that charset is supported
    private static void ensureCharsetIsSupported(String csn) {
        if (csn == null)
            throw new NullPointerException("'csn' is null");
        if (!Charset.isSupported(csn))
            throw new UnsupportedCharsetException(csn);
    }

    /**
     * Writes a byte array to a file. The file is created if it does not exist.
     * If the file already exists, it is first truncated.
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void write(FileRef file, byte[] bytes) throws IOException {
        write(file, bytes, 0, bytes.length);
    }

    /**
     * Writes a byte array to a file. The file is created if it does not exist.
     * If the file already exists, it is first truncated.
     *
     * @throws  IndexOutOfBoundsException
     *          If {@code off} or {@code len} is negative, or {@code off+len}
     *          is greater than the length of the array
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void write(FileRef file, byte[] bytes, int off, int len)
        throws IOException
    {
        WritableByteChannel wbc = file.newByteChannel(WRITE, CREATE, TRUNCATE_EXISTING);
        try {
            int pos = off;
            while (pos < len) {
                int size = Math.min(len-pos, 8192);
                ByteBuffer bb = ByteBuffer.wrap(bytes, pos, size);
                int n = wbc.write(bb);
                pos += n;
            }
        } finally {
            wbc.close();
        }
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the specified charset. When all
     * lines have been written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The list of lines to write (in order)
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  java.nio.charset.UnsupportedCharsetException
     *          If no support for the named charset is available
     *          in this instance of the Java virtual machine
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(FileRef file, List<String> lines, String csn)
        throws IOException
    {
        ensureCharsetIsSupported(csn);
        WritableByteChannel wbc = file.newByteChannel(WRITE, CREATE, TRUNCATE_EXISTING);
        BufferedWriter writer = new BufferedWriter(Channels.newWriter(wbc, csn));
        try {
            implWriteLines(writer, lines);
        } finally {
            writer.close();
        }
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}. When all lines have been
     * written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The list of lines to write (in order)
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(FileRef file, List<String> lines)
        throws IOException
    {
        writeLines(file, lines, Charset.defaultCharset().name());
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}. When all lines have been
     * written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The array of lines to write (in order)
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(FileRef file, String... lines)
        throws IOException
    {
        writeLines(file, Arrays.asList(lines), Charset.defaultCharset().name());
    }

    /**
     * Writes a byte array to a file. The file is created if it does not exist.
     * If the file already exists, it is first truncated.
     *
     * @param   file
     *          The file
     * @param   bytes
     *          The byte array to write to the file
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void write(File file, byte[] bytes) throws IOException {
        write(file, bytes, 0, bytes.length);
    }

    /**
     * Writes a byte array to a file. The file is created if it does not exist.
     * If the file already exists, it is first truncated.
     *
     * @throws  IndexOutOfBoundsException
     *          If {@code off} or {@code len} is negative, or {@code off+len}
     *          is greater than the length of the array
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void write(File file, byte[] bytes, int off, int len)
        throws IOException
    {
        FileOutputStream out = new FileOutputStream(file);
        try {
            out.write(bytes, off, len);
        } finally {
            out.close();
        }
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the specified charset. When all
     * lines have been written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The list of lines to write (in order)
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  java.nio.charset.UnsupportedCharsetException
     *          If no support for the named charset is available
     *          in this instance of the Java virtual machine
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(File file, List<String> lines, String csn)
        throws IOException
    {
        ensureCharsetIsSupported(csn);
        FileOutputStream out = new FileOutputStream(file);
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out, csn));
        try {
            implWriteLines(writer, lines);
        } finally {
            writer.close();
        }
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}. When all lines have been
     * written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The list of lines to write (in order)
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(File file, List<String> lines)
        throws IOException
    {
        writeLines(file, lines, Charset.defaultCharset().name());
    }

    /**
     * Writes the given lines of text to the specified file. The characters in
     * each line are encoded into bytes using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}. When all lines have been
     * written, or an I/O error occurs, then the file is closed.
     *
     * @param   file
     *          The file
     * @param   lines
     *          The array of lines to write (in order)
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(File file, String... lines)
        throws IOException
    {
        writeLines(file, Arrays.asList(lines), Charset.defaultCharset().name());
    }

    /**
     * Writes the given lines of text to the specified output stream. The
     * characters in each line are encoded into bytes using the specified charset.
     *
     * @param   out
     *          The output stream
     * @param   lines
     *          The list of lines to write (in order)
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(OutputStream out, List<String> lines, String csn)
        throws IOException
    {
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out, csn));
        implWriteLines(writer, lines);
        writer.flush();
    }

    /**
     * Writes the given lines of text to the specified output stream. The
     * characters in each line are encoded into bytes using the underlying
     * platform's {@linkplain Charset#defaultCharset() default charset}.
     *
     * @param   out
     *          The output stream
     * @param   lines
     *          The list of lines to write (in order)
     *
     * @throws  java.nio.charset.UnmappableCharacterException
     *          Where a line contains a character that cannot be mapped to an
     *          output byte sequence
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static void writeLines(OutputStream out, List<String> lines)
        throws IOException
    {
        writeLines(out, lines, Charset.defaultCharset().name());
    }

    private static void implWriteLines(BufferedWriter writer, List<String> lines)
        throws IOException
    {
        for (String line: lines) {
            writer.write(line);
            writer.newLine();
        }
    }
}
