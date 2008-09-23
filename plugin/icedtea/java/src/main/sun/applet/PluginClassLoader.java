package sun.applet;

public class PluginClassLoader extends ClassLoader {

	public PluginClassLoader() {
		super();
	}

	public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
		return super.loadClass(name, resolve);
	}
	
}
