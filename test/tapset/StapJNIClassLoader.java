package staptest;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.IOException;

/* Loads classes only in default package in working directory, must be
 * under 4096 bytes.  Intentionally breaks classloader hierarchy.
 */
public class StapJNIClassLoader extends ClassLoader {

	public StapJNIClassLoader() {
		super();
	}

	public StapJNIClassLoader(ClassLoader parent) {
		super(parent);
	}

	private static final int BUF_LEN = 4096;

	protected synchronized Class loadClass(String className,
			boolean resolve) throws ClassNotFoundException {
		Class aClass = findLoadedClass(className);

		if (aClass != null) {
			return aClass;
		}

		String fileName = className.replace('.', '/').concat(".class");
		byte[] classBytes = null;
		try {
			InputStream classIn = getResourceAsStream(fileName);
			byte[] buffer = new byte[BUF_LEN];
			ByteArrayOutputStream temp = new ByteArrayOutputStream();
			int bytes_read = -1;
			while ((bytes_read = classIn.read(buffer, 0, BUF_LEN)) != -1) {
				temp.write(buffer, 0, bytes_read);
			}
			classBytes = temp.toByteArray();
		} catch (IOException ignore) {
			// Possible error condition(s) resulting from thrown
			// exception checked after.
		}

		if (classBytes == null) {
			throw new ClassNotFoundException("Could not load class: " + className);
		}

		try {
			aClass = defineClass(className, classBytes, 0, classBytes.length);
			if (aClass == null) {
				System.out.println("Gaah!");
			}
			if (resolve) {
				resolveClass(aClass);
			}
		} catch (SecurityException ex) {
			aClass = super.loadClass(className, resolve);
		}

		return aClass;
	}
}
