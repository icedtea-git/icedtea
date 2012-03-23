package staptest;

public class JNITestClass {
	public boolean myBoolean;
	public byte myByte;
	public char myChar;
        public double myDouble;
        public float myFloat;
        public int myInt;
        public long myLong;
        public String myObject;
        public short myShort;
        
	public static boolean myStaticBoolean = false;
	public static byte myStaticByte = 1;
	public static char myStaticChar = 'b';
        public static double myStaticDouble = 5.5;
        public static float myStaticFloat = (float) 2.75;
        public static int myStaticInt = 32;
        public static long myStaticLong = 64;
        public static String myStaticObject = "aString";
        public static short myStaticShort = 16;

	public JNITestClass() {
		myBoolean = true;
		myByte = 0;
		myChar = 'a';
		myDouble = 1.5;
		myFloat = (float) 0.75;
		myInt = 4;
		myLong = 8;
                myObject = "myString";
		myShort = 2;
	}

	private native void doNothing();

	public boolean getBoolean() {
		return myBoolean;
	}

	public byte getByte() {
		return myByte;
	}

	public char getChar() {
		return myChar;
	}

	public double getDouble() {
		return myDouble;
	}

	public float getFloat() {
		return myFloat;
	}

	public int getInt() {
		return myInt;
	}

	public long getLong() {
		return myLong;
	}

	public Object getObject() {
		return (Object) new String(myObject);
	}

	public short getShort() {
		return myShort;
	}

	/* Ridiculous name and useless method, but this removes needing a
         * special case for testing CallVoidMethod[A|V] probes. */
	public void getVoid() {
		return;
        }

	public static boolean getStaticBoolean() {
		return myStaticBoolean;
	}

	public static byte getStaticByte() {
		return myStaticByte;
	}

	public static char getStaticChar() {
		return myStaticChar;
	}

	public static double getStaticDouble() {
		return myStaticDouble;
	}

	public static float getStaticFloat() {
		return myStaticFloat;
	}

	public static int getStaticInt() {
		return myStaticInt;
	}

	public static long getStaticLong() {
		return myStaticLong;
	}

	public static Object getStaticObject() {
		return (Object) new String(myStaticObject);
	}

	public static short getStaticShort() {
		return myStaticShort;
	}

	/* Ridiculous name and useless method, but this removes needing a
         * special case for testing CallStaticVoidMethod[A|V] probes. */
	public static void getStaticVoid() {
		return;
	}
}
