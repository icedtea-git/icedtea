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

import java.util.Enumeration;
import java.util.Vector;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;

/**
 * A space-efficient string to string map.
 * This class is similar to java.util.Properties.
 * For this class, space is more important than speed.  Use this class when
 * you care much more about wasted space than wasting time doing reference
 * juggling in memory.  Arrays in this class must correspond to this format:
 * <br>
 * <Pre>
 * {"key1", "value1", "key2", "value2", ...}
 * </Pre>
 */

 // This code was derived from that in com.sun.javatest.util.Properties.

public class PropertyArray {
    /**
     * A class used to report problems that may occur when using PropertyArray.
     */
    public static class PropertyArrayError extends Error {
        /**
         * Create a PropertyArrayError object.
         */
        public PropertyArrayError() {
            super();
        }

        /**
         * Create a PropertyArrayError object.
         * @param msg a detail message for the error
         */
        public PropertyArrayError(String msg) {
            super(msg);
        }
    }

    /**
     * Create a mutable object.
     */
    public PropertyArray() {
        locked = false;
    }

    /**
     * Create a mutable object.
     * @param initSize the initial capacity of the array
     */
    public PropertyArray(int initSize) {
        // This method probably should be some kind of optimization, but if
        // we pre-create the array, how will be know the size?  Another data
        // member I suppose...
        locked = false;
    }

    /**
     * Create a immutable object, from data read from on a stream in
     * the format of a standard Java properties file.
     * @param in the stream from which to read the properties
     * @throws IOException if a problem occurred while reading the data
     */
    public PropertyArray(Reader in) throws IOException {
        dataA = load(in);
        locked = true;
    }

    /**
     * Create a immutable PropertyArray object from a standard Properties object.
     * @param props the object from which to initialize the array
     */
    public PropertyArray(Properties props) {
        dataA = getArray(props);

        locked = true;
    }

    /**
     * Create a immutable PropertyArray object from data in a compact array of
     * names and values.
     * @param data an array containing pairs of entries: even-numbered entries
     * identify the names of properties, odd-numbered entries give the value
     * for the preceding property name.
     */
    public PropertyArray(String[] data) {
        locked = true;
        dataA = new String[data.length];

        // shallow copy the array
        for (int i = 0; i < data.length; i++) {
            dataA[i] = data[i];
        }
    }

    // --------------- STATIC METHODS ----------------

    /**
     * Get a compact array containing the names and values of entries
     * from a standard Properties object.
     * @param props the Properties object from which to get the data
     * @return an array containing the names of the properties in even-numbered
     * entries, and the corresponding values in the adjacent odd-numbered entries
     */
    public static String[] getArray(Properties props) {
        Enumeration values = props.elements();
        Enumeration keys = props.keys();
        Vector data = new Vector(props.size(),2);

        for (; keys.hasMoreElements() ;) {
            insert(data, (String)(keys.nextElement()),
                   (String)(values.nextElement()));
        }

        String[] arr = new String[data.size()];
        data.copyInto(arr);

        return arr;
    }

    /**
     * Add a mapping to an array, returning a new array.
     * @param data The array to which to the new array is to be added.  May be null.
     * @param key the name of the new value to be added
     * @param value the new value to be added
     * @return an array with the new element added
     * @exception PropertyArrayError May be thrown if a null key or value is
     *            supplied.
     */
    public static String[] put(String[] data, String key, String value) {
        Vector vec;
        String[] arr;
        String old = null;

        if (key == null || value == null) {
            // i'd like to make null values legal but...
            throw new PropertyArrayError(
                "A key or value was null.  Null keys and values are illegal");
        }

        if (data == null) {
            data = new String[0];
        }

        if (key == null)
            arr = data;
        else {
            vec = copyOutOf(data);
            old = insert(vec, key, value);
            arr = new String[vec.size()];
            vec.copyInto(arr);
        }

        return arr;
    }

    /**
     * Get a named value from the array of properties.
     * If the given data array is null or zero length, null is returned.
     * If the key paramter is null, null will be returned, no error will
     * occur.
     * @param data an array containing sequential name value pairs
     * @param key the name of the property to be returned
     * @return the value of the named entry, or null if not found
     */
    public static String get(String[] data, String key) {
        if (data == null || data.length == 0 || key == null) {
            return null;
        }

        int lower = 0;
        int upper = data.length - 2;
        int mid;

        if (upper < 0)
            return null;

        String last = data[upper];
        int cmp = key.compareTo(last);
        if (cmp > 0)
            return null;

        while (lower <= upper) {
            // in next line, take care to ensure that mid is always even
            mid = lower + ((upper - lower) / 4) * 2;
            String e = data[mid];
            cmp = key.compareTo(e);
            if (cmp < 0)
                upper = mid - 2;
            else if (cmp > 0)
                lower = mid + 2;
            else
                return data[mid+1];
        }

        // did not find an exact match
        return null;
    }

    /**
     * Remove an entry from an array of properties.
     * @param data an array of sequential name value properties
     * @param key the name of the entry to be removed
     * @return an array that does not contain the named property
     */
    public static String[] remove(String[] data, String key) {
        Vector vec = copyOutOf(data);
        int lower = 0;
        int upper = vec.size() - 2;
        int mid = 0;
        String old = null;

        if (upper < 0) {
            // no data yet
            return data;
        }

        // goes at the end
        String last = (String)vec.elementAt(upper);
        int cmp = key.compareTo(last);
        if (cmp > 0) {
            return data;
        }

        while (lower <= upper) {
            // in next line, take care to ensure that mid is always even
            mid = lower + ((upper - lower) / 4) * 2;
            String e = (String)(vec.elementAt(mid));
            cmp = key.compareTo(e);
            if (cmp < 0) {
                upper = mid - 2;
            }
            else if (cmp > 0) {
                lower = mid + 2;
            }
            else {
                // strings equal, zap key and value
                vec.removeElementAt(mid);
                old = (String)(vec.elementAt(mid));
                vec.removeElementAt(mid);
                break;
            }
        }

        String[] outData = new String[vec.size()];
        vec.copyInto(outData);
        return outData;
    }

    /**
     * Get a standard Properties object from an array of properties.
     * @param data an array of sequential name value properties
     * @return a Properties object containing data from the array
     */
    public static Properties getProperties(String[] data) {
        Properties props = null;

        if (data == null || data.length == 0) {
            props = new Properties();
        }
        else {
            props = new Properties();

            for (int i = 0; i < data.length; i+=2) {
                props.put(data[i], data[i+1]);
            }   // for
        }   // else
        return props;
    }

    /**
     * Write an array of properties to a stream.
     * The data is written using the format for standard Java property files.
     * @param data an array of sequential name value properties
     * @param out a stream to which to write the data
     * @throws IOException if a problem occurred while writing to the stream
     * @see #load(Reader)
     */
    public static void save(String[] data, Writer out) throws IOException {
        if (data == null || data.length == 0) {
            return;
        }

        for (int whichPair = 0; whichPair < data.length; whichPair+=2) {
            out.write(data[whichPair]);
            out.write('=');

            String val = data[whichPair+1];
            int len = val.length();
            boolean empty = false;

            for (int i = 0 ; i < len ; i++) {
                int ch = val.charAt(i);

                switch (ch) {
                  case '\\': out.write('\\'); out.write('\\'); break;
                  case '\t': out.write('\\'); out.write('t'); break;
                  case '\n': out.write('\\'); out.write('n'); break;
                  case '\r': out.write('\\'); out.write('r'); break;

                  default:
                    if ((ch < ' ') || (ch >= 127) || (empty && (ch == ' '))) {
                        out.write('\\');
                        out.write('u');
                        out.write(toHex((ch >> 12) & 0xF));
                        out.write(toHex((ch >>  8) & 0xF));
                        out.write(toHex((ch >>  4) & 0xF));
                        out.write(toHex((ch >>  0) & 0xF));
                    }
                    else {
                        out.write(ch);
                    }
                }   // switch
                empty = false;
            }   // for

            out.write(lineSeparator);
        }   // for
    }

    /**
     * Read an array of properties from an input stream.
     * The data will be read according to the standard format for Java
     * property files.
     * @param in the stream from which to read the data
     * @return an array of sequential name value properties
     * @throws IOException if an error occurred while reading the data
     * @see #save(String[], Writer)
     */
    public static String[] load(Reader in) throws IOException {
        Vector v = new Vector();
        int ch = 0;

        //// eat any preceding whitespace
        //do {
        //    ch = in.read();
        //} while ((ch == '\n') || (ch == '\r') || (ch == '\t') || (ch == ' '));

        ch = '\n';
        // pretend to have read newline, so that if an initial
        // blank line is read, it is treated as the end of an empty props object

    parsing:
        while (true) {
            // parse until EOF or a blank line
            switch (ch) {
            case -1:
                // EOF
                break parsing;

            case '#':
            case '!':
                // comment: skip to end of line
                do {
                    ch = in.read();
                } while ((ch >= 0) && (ch != '\n') && (ch != '\r'));
                continue;

            case '\n':
            case '\t':
            case '\r':
            case ' ':
                // skip whitespace but count newlines along the way;
                // if there is more than one, we're done. This is intended
                // to work on both \n and \r\n systems
                int lines = 0;
                do {
                    if (ch == '\n' && (++lines > 1))
                        break parsing;
                    ch = in.read();
                } while ((ch == '\n') || (ch == '\r') || (ch == '\t') || (ch == ' '));
                continue;

            default:
                // key=value
                // start by reading the key; stop at newline (unless escaped in value)
                StringBuffer key = new StringBuffer();
                while ((ch >= 0) && (ch != '=') && (ch != ':') &&
                       (ch != ' ') && (ch != '\t') && (ch != '\n') && (ch != '\r')) {
                    key.append((char)ch);
                    ch = in.read();
                }
                while ((ch == ' ') || (ch == '\t')) {
                    ch = in.read();
                }
                if ((ch == '=') || (ch == ':')) {
                    ch = in.read();
                }
                while ((ch == ' ') || (ch == '\t')) {
                    ch = in.read();
                }

                // Read the value
                StringBuffer val = new StringBuffer();
                while ((ch >= 0) && (ch != '\n') && (ch != '\r')) {
                    int next = 0;
                    if (ch == '\\') {
                        switch (ch = in.read()) {
                        case '\r':
                            if (((ch = in.read()) == '\n') || (ch == ' ') || (ch == '\t')) {
                                // fall thru to '\n' case
                            }
                            else
                                continue;
                        case '\n':
                            while (((ch = in.read()) == ' ') || (ch == '\t'))
                                /*do-nothing*/;
                            continue;
                        case 't': ch = '\t'; next = in.read(); break;
                        case 'n': ch = '\n'; next = in.read(); break;
                        case 'r': ch = '\r'; next = in.read(); break;
                        case 'u': {
                            while ((ch = in.read()) == 'u');
                            int d = 0;
                        loop:
                            for (int i = 0 ; i < 4 ; i++) {
                                next = in.read();
                                switch (ch) {
                                case '0': case '1': case '2': case '3': case '4':
                                case '5': case '6': case '7': case '8': case '9':
                                    d = (d << 4) + ch - '0';
                                    break;
                                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                                    d = (d << 4) + 10 + ch - 'a';
                                    break;
                                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                                    d = (d << 4) + 10 + ch - 'A';
                                    break;
                                default:
                                    break loop;
                                }
                                ch = next;
                            }
                            ch = d;
                            break;
                        }
                        default: next = in.read(); break;
                        }
                    }
                    else {
                        next = in.read();
                    }
                    val.append((char)ch);
                    ch = next;
                }

                //System.out.println(key + " = '" + val + "'");
                insert(v, key.toString(), val.toString());
            }
        }

        String[] data = new String[v.size()];
        v.copyInto(data);
        return data;
    }

    /**
     * Enumerate the properties in an array.
     * @param props an array of sequential name value properties
     * @return an enumeration of the properties in the array
     */
    public static Enumeration enumerate(final String[] props) {
        return new Enumeration() {
            int pos = 0;

            public boolean hasMoreElements() {
                return (props != null && pos < props.length);
            }

            public Object nextElement() {
                if (props == null || pos >= props.length) {
                   return null;
                }
                else {
                   String current = props[pos];
                   pos += 2;
                   return current;
                }
            }
        };
    }

    // --------------- INSTANCE METHODS ----------------

    /**
     * Get the data in this PropertyArray as a standard Properties object.
     * @return a Properties object containing the same data as this PropertyArray
     */
    public Properties getProperties() {
        return getProperties(dataA);
    }

    /**
     * Check if the property array is mutable.
     * @return true if data can be stored in this array, and false otherwise
     */
    public boolean isMutable() {
        return !locked;
    }

    /**
     * Get the number of properties stored in the property array.
     * @return the number of properties stored in the property array
     */
    public int size() {
        if (dataA == null) {
            return 0;
        }
        else {
            return dataA.length / 2;
        }
    }

    /**
     * Get the value of a named property.
     * @param key the name of the desired property
     * @return the value of the property, or null if it was not found
     */
    public String get(String key) {
        return get(dataA, key);
    }

    /**
     * Get a copy of the data in this PropertyArray.
     * @return a copy of the data, or null if there is no data.
     */
    public String[] getArray() {
        if (dataA == null || dataA.length == 0) {
            return null;
        }

        return shallowCopy(dataA);
    }

    /**
     * Put a property into the PropertyArray.
     * @param key the name of the property to be added
     * @param value the value of the property to be added
     * @return the previous value (if any) of this property
     * @throws PropertyArrayError if a null key or value is supplied.
     */
    public String put(String key, String value) {
        String old = null;

        if (locked == true) {
            throw new IllegalStateException("PropertyArray is immutable.");
        }

        if (key == null || value == null) {
            // i'd like to make null values legal but...
            throw new PropertyArrayError(
                "A key or value was null.  Null keys and values are illegal");
        }

        Vector vec = copyOutOf(dataA);
        old = insert(vec, key, value);
        dataA = new String[vec.size()];
        vec.copyInto(dataA);

        return old;
    }

    /**
     * Remove a property.
     * @param key the name of the property to be removed
     */
    public void remove(String key) {
        dataA = remove(dataA, key);
    }

    /**
     * Save the properties to a stream. The data is written using the format
     * for a standard Java properties file.
     * @param out the stream to which to write the data
     * @throws IOException if an error occured while writing the data
     */
    public void save(Writer out) throws IOException {
        save(dataA, out);
    }

    // --------------- PRIVATE ----------------

    /**
     * Put a new value in, overwriting existing values.
     *
     * @param vec Vector to add data to.  Must not be null!
     */
    private static String insert(Vector vec, String key, String value) {
        int lower = 0;
        int upper = vec.size() - 2;
        int mid = 0;
        String old = null;

        if (upper < 0) {
            // no data yet
            vec.addElement(key);
            vec.addElement(value);
            return old;
        }

        // goes at the end
        String last = (String)vec.elementAt(upper);
        int cmp = key.compareTo(last);
        if (cmp > 0) {
            vec.addElement(key);
            vec.addElement(value);
            return null;
        }

        while (lower <= upper) {
            // in next line, take care to ensure that mid is always even
            mid = lower + ((upper - lower) / 4) * 2;
            String e = (String)(vec.elementAt(mid));
            cmp = key.compareTo(e);
            if (cmp < 0) {
                upper = mid - 2;
            }
            else if (cmp > 0) {
                lower = mid + 2;
            }
            else {
                // strings equal
                vec.removeElementAt(mid);
                old = (String)(vec.elementAt(mid));
                vec.removeElementAt(mid);
                break;
            }
        }

        // did not find an exact match (we did not expect to)
        // adjust the insert point
        if (cmp > 0)
            mid += 2;

        vec.insertElementAt(key, mid);
        vec.insertElementAt(value, mid+1);

        return old;
    }

    private static Vector copyOutOf(String[] data) {
        Vector vec = null;

        if (data == null) {
            vec = new Vector(0,2);
        }
        else {
            vec = new Vector(data.length,2);

            for (int i = 0; i < data.length; i++) {
                vec.addElement(data[i]);
            }   // for
        }

        return vec;
    }

    private static String[] shallowCopy(String[] arrIn) {
        if (arrIn == null) {
            return null;
        }

        String[] arrOut = new String[arrIn.length];

        // shallow copy the array
        for (int i = 0; i < arrIn.length; i++) {
            arrOut[i] = arrIn[i];
        }

        return arrOut;
    }

    /**
     * Convert a nibble to a hex character
     * @param   nibble  the nibble to convert.
     */
    private static char toHex(int nibble) {
        return hexDigit[(nibble & 0xF)];
    }

    /** A table of hex digits */
    private static char[] hexDigit = {
        '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
    };

    private static final String lineSeparator = System.getProperty("line.separator");

    private String[] dataA;
    private boolean locked;


}
