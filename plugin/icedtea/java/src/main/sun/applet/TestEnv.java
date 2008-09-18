/* TestEnv -- test JavaScript-to-Java calls
   Copyright (C) 2008  Red Hat

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

IcedTea is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

package sun.applet;

public class TestEnv
{
    public static int intField = 103;
    public int intInstanceField = 7822;
    public String stringField = "hello";
    // z <musical G clef> <chinese water>
    public String complexStringField = "z\uD834\uDD1E\u6C34";

    public static void TestIt() {
        System.out.println("TestIt");
    }

    public static void TestItBool(boolean arg) {
        System.out.println("TestItBool: " + arg);
    }

    public static void TestItByte(byte arg) {
        System.out.println("TestItByte: " + arg);
    }

    public static void TestItChar(char arg) {
        System.out.println("TestItChar: " + arg);
    }

    public static void TestItShort(short arg) {
        System.out.println("TestItShort: " + arg);
    }

    public static void TestItInt(int arg) {
        System.out.println("TestItInt: " + arg);
    }

    public static void TestItLong(long arg) {
        System.out.println("TestItLong: " + arg);
    }

    public static void TestItFloat(float arg) {
        System.out.println("TestItFloat: " + arg);
    }

    public static void TestItDouble(double arg) {
        System.out.println("TestItDouble: " + arg);
    }

    public static void TestItObject(TestEnv arg) {
        System.out.println("TestItObject: " + arg);
    }

    public static void TestItObjectString(String arg) {
        System.out.println("TestItObjectString: " + arg);
    }

    public static void TestItIntArray(int[] arg) {
        System.out.println("TestItIntArray: " + arg);
        for (int i = 0; i < arg.length; i++)
            System.out.println ("ELEMENT: " + i + " " + arg[i]);
    }

    public static void TestItObjectArray(String[] arg) {
        System.out.println("TestItObjectArray: " + arg);
        for (int i = 0; i < arg.length; i++)
            System.out.println ("ELEMENT: " + i + " " + arg[i]);
    }

    public static void TestItObjectArrayMulti(String[][] arg) {
        System.out.println("TestItObjectArrayMulti: " + arg);
        for (int i = 0; i < arg.length; i++)
            for (int j = 0; j < arg[i].length; j++)
                System.out.println ("ELEMENT: " + i + " " + j + " " + arg[i][j]);
    }

    public static boolean TestItBoolReturnTrue() {
        return true;
    }

    public static boolean TestItBoolReturnFalse() {
        return false;
    }

    public static byte TestItByteReturn() {
        return (byte) 0xfe;
    }

    public static char TestItCharReturn() {
        return 'K';
    }

    public static char TestItCharUnicodeReturn() {
        return '\u6C34';
    }

    public static short TestItShortReturn() {
        return 23;
    }

    public static int TestItIntReturn() {
        return 3445;
    }

    public static long TestItLongReturn() {
        return 3242883;
    }

    public static float TestItFloatReturn() {
        return 9.21E4f;
    }

    public static double TestItDoubleReturn() {
        return 8.33E88;
    }

    public static Object TestItObjectReturn() {
        return new String("Thomas");
    }

    public static int[] TestItIntArrayReturn() {
        return new int[] { 6, 7, 8 };
    }

    public static String[] TestItObjectArrayReturn() {
        return new String[] { "Thomas", "Brigitte" };
    }

    public static String[][] TestItObjectArrayMultiReturn() {
        return new String[][] { {"Thomas", "Brigitte"},
                                {"Lindsay", "Michael"} };
    }

    public int TestItIntInstance(int arg) {
        System.out.println("TestItIntInstance: " + this + " " + arg);
        return 899;
    }
}
