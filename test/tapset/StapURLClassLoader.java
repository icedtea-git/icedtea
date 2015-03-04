package staptest;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;

/* Let superclass handle everything, we just need a separate Classloader that
 * will be unloaded (along with classes and their associated hot-compiled
 * methods) while testing the hotspot.compiled_method_unload probe.
 */
public class StapURLClassLoader extends URLClassLoader {
	public StapURLClassLoader(URL[] urls, ClassLoader parent) {
		super(urls, parent);
	}

	public Class loadClass(String name)  throws ClassNotFoundException {
		return super.loadClass(name);
	}

	protected Class findClass(String name) throws ClassNotFoundException {
		return super.findClass(name);
	}
}

