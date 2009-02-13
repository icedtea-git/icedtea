/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2009 Red Hat, Inc.
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

package org.classpath.icedtea.java.io;

import java.io.Closeable;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import java.nio.charset.Charset;
import java.nio.charset.UnsupportedCharsetException;
import java.nio.channels.Channels;

import java.util.Collections;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;


import org.classpath.icedtea.java.nio.file.FileRef;

import org.classpath.icedtea.java.nio.file.attribute.Attributes;

import org.classpath.icedtea.java.util.Scanner;

/**
 * {@note experimental}
 * This class consists exclusively of static methods that operate on input
 * sources.
 *
 * <p> The methods to read lines of text defined by this class recognize the
 * following as Unicode line terminators:
 * <ul>
 *   <li> <code>&#92;u000D</code> followed by <code>&#92;u000A</code>,
 *     CARRIAGE RETURN followed by LINE FEED </li>
 *   <li> <code>&#92;u000A</code>, LINE FEED </li>
 *   <li> <code>&#92;u000D</code>, CARRIAGE RETURN </li>
 *   <li> <code>&#92;u2028</code>, LINE SEPARATOR </li>
 *   <li> <code>&#92;u2029</code>, PARAGRAPH SEPARATOR </li>
 *   <li> <code>&#92;u0085</code>, NEXT LINE (NEL)</li>
 * </ul>
 *
 * @since 1.7
 */

public final class Inputs {
    private Inputs() { }

    // initial buffer size when reading (or writing)
    private static final int INITIAL_BUFFER_SIZE = 8192;

    // checks that charset is supported
    private static void ensureCharsetIsSupported(String csn) {
        if (csn == null)
            throw new NullPointerException("'csn' is null");
        if (!Charset.isSupported(csn))
            throw new UnsupportedCharsetException(csn);
    }

    /**
     * Closes the given data source by invoking its {@link Closeable#close close}
     * method. If the {@code close} method throws an {@code IOException} then it
     * is silently ignored. If the source is already closed then invoking this
     * method has no effect.
     *
     * <p> This method should not be used to close destinations or output
     * streams that may be buffered. An I/O error may occur when flushing
     * buffered data.
     *
     * @param   source
     *          The data source
     */
    public static void closeUnchecked(Closeable source) {
        try {
            source.close();
        } catch (IOException ignore) { }
    }

    /**
     * Read all bytes from the specified file. When all bytes have been read, or
     * an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     *
     * @return  A byte array containing the bytes read from the file
     *
     * @throws  IOException
     *          If an I/O error occurs
     * @throws  OutOfMemoryError
     *          If the required array size is too large
     */
    public static byte[] readAllBytes(FileRef source) throws IOException {
        long size = Attributes.readBasicFileAttributes(source).size();
        if (size > (long)Integer.MAX_VALUE)
            throw new OutOfMemoryError("Required array size too large");
        InputStream in = Channels.newInputStream(source.newByteChannel());
        try {
            return implReadAllBytes(in, Math.min((int)size, INITIAL_BUFFER_SIZE));
        } finally {
            in.close();
        }
    }

    /**
     * Read all bytes from the specified file. When all bytes have been read, or
     * an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     *
     * @return  A byte array containing the bytes read from the file
     *
     * @throws  IOException
     *          If an I/O error occurs
     * @throws  OutOfMemoryError
     *          If the required array size is too large
     */
    public static byte[] readAllBytes(File source) throws IOException {
        InputStream in = new FileInputStream(source);
        try {
            return implReadAllBytes(in, INITIAL_BUFFER_SIZE);
        } finally {
            in.close();
        }
    }

    /**
     * Read all bytes from the specified input stream.
     *
     * <p> <b>Usage Example:</b>
     * Suppose we want to open a connection to a resource identified by a URI,
     * and read all bytes:
     * <pre>
     *   URI uri = ...
     *   byte[] content = InputOutput.readAllBytes(uri.toURL().openStream());
     * </pre>
     *
     * <p> On return, the input stream will be at end of stream.
     *
     * @param   source
     *          The data source
     *
     * @return  A byte array containing the bytes read from the source
     *
     * @throws  IOException
     *          If an I/O error occurs
     * @throws  OutOfMemoryError
     *          If the required array size is too large
     */
    public static byte[] readAllBytes(InputStream source) throws IOException {
        return implReadAllBytes(source, INITIAL_BUFFER_SIZE);
    }

    private static byte[] implReadAllBytes(InputStream source, int initialSize)
        throws IOException
    {
        byte[] buf = new byte[initialSize];
        int nread = 0;
        int rem = buf.length;
        int n;
        while ((n = source.read(buf, nread, rem)) > 0) {
            nread += n;
            rem -= n;
            if (rem <= 0) {
                int newSize = buf.length << 1;
                if (newSize <= buf.length) {
                    if (buf.length == Integer.MAX_VALUE)
                        throw new OutOfMemoryError("Required array size too large");
                    newSize = Integer.MAX_VALUE;
                }
                rem = newSize - buf.length;
                buf = Arrays.copyOf(buf, newSize);
            }
        }
        return (buf.length == nread) ? buf : Arrays.copyOf(buf, nread);
    }

    /**
     * Read all lines from the specified file. Bytes from the file are
     * converted into characters using the specified charset. When all lines
     * have been read, or an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  java.nio.charset.UnsupportedCharsetException
     *          If no support for the named charset is available
     *          in this instance of the Java virtual machine
     * @throws  java.nio.charset.MalformedInputException
     *          If the file contains a byte sequence that is not legal for the
     *          charset
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(FileRef source, String csn)
        throws IOException
    {
        ensureCharsetIsSupported(csn);
        Scanner s = new Scanner(source, csn);
        try {
            return implReadAllLines(s);
        } finally {
            s.close();
        }
    }

    /**
     * Read all lines from the specified file. Bytes from the file are
     * converted into characters using the underlying platform's {@linkplain
     * java.nio.charset.Charset#defaultCharset() default charset}. When all lines
     * have been read, or an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     *
     * @throws  java.nio.charset.MalformedInputException
     *          If the file contains a byte sequence that is not legal for the
     *          default charset
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(FileRef source) throws IOException {
        Scanner s = new Scanner(source);
        try {
            return implReadAllLines(s);
        } finally {
            s.close();
        }
    }

    /**
     * Read all lines from the specified file. Bytes from the file are
     * converted into characters using the specified charset. When all lines
     * have been read, or an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  UnsupportedCharsetException
     *          If no support for the named charset is available
     *          in this instance of the Java virtual machine
     * @throws  java.nio.charset.MalformedInputException
     *          If the file contains a byte sequence that is not legal for the
     *          charset
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(File source, String csn)
        throws IOException
    {
        ensureCharsetIsSupported(csn);
        Scanner s = new Scanner(source, csn);
        try {
            return implReadAllLines(s);
        } finally {
            s.close();
        }
    }

    /**
     * Read all lines from the specified file. Bytes from the file are
     * converted into characters using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}. When all lines have been read,
     * or an I/O error occurs, then the file is closed.
     *
     * @param   source
     *          The data source
     * @throws  java.nio.charset.MalformedInputException
     *          If the file contains a byte sequence that is not legal for the
     *          default charset
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(File source) throws IOException {
        Scanner s = new Scanner(source);
        try {
            return implReadAllLines(s);
        } finally {
            s.close();
        }
    }

    /**
     * Read all lines from the specified input stream. Bytes from the stream are
     * converted into characters using the specified charset.
     *
     * <p> On return, the input stream will be at end of stream.
     *
     * @param   source
     *          The input stream to read from
     * @param   csn
     *          The name of the charset to be used
     *
     * @throws  UnsupportedCharsetException
     *          If no support for the named charset is available
     *          in this instance of the Java virtual machine
     * @throws  java.nio.charset.MalformedInputException
     *          If a byte sequence that is not legal for the charset is read
     *          from the input
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(InputStream source, String csn)
        throws IOException
    {
        ensureCharsetIsSupported(csn);
        return implReadAllLines(new Scanner(source, csn));
    }

    /**
     * Read all lines from the specified input stream. Bytes from the stream are
     * converted into characters using the underlying platform's {@linkplain
     * Charset#defaultCharset() default charset}.
     *
     * <p> On return, the input stream will be at end of stream.
     *
     * @param   source
     *          The input stream to read from
     *
     * @return  An unmodifiable list of the lines read from the input stream
     *
     * @throws  java.nio.charset.MalformedInputException
     *          If a byte sequence that is not legal for the default charset is
     *          read from the input
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(InputStream source) throws IOException {
        return implReadAllLines(new Scanner(source));
    }

    /**
     * Read all lines from the from the specified source.
     *
     * <p> On return, the input source will be at end of stream.
     *
     * @param   source
     *          The input stream to read from
     *
     * @return  An unmodifiable list of the lines read from source
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    public static List<String> readAllLines(Readable source) throws IOException {
        return implReadAllLines(new Scanner(source));
    }

    private static List<String> implReadAllLines(Scanner s) throws IOException {
        try {
            List<String> result = new ArrayList<String>();
            while (s.hasNextLine()) {
                result.add(s.nextLine());
            }
            return Collections.unmodifiableList(result);
        } finally {
            IOException ioe = s.ioException();
            if (ioe != null)
                throw ioe;
        }
    }
}
