package staptest;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;

public class TestingRunner implements Runnable {
	public static int NOOP = 0;
	public static int LOADER = 1;
	public static int CONTENDER = 2;
	public static int NOTIFY = 3;
	public static int NOTIFYALL = 4;

	private int myType;

	public TestingRunner(int type) {
		myType = type;
	}

	public void run() {
		if (myType == LOADER) {
			doLoadClass();
		} else if (myType == CONTENDER) {
			doSynchronized();
		} else if (myType == NOTIFY) {
			doNotify();
		} else if (myType == NOTIFYALL) {
			doNotifyAll();
		}
	}

	private void doLoadClass() {
		/* We really just want to load the class and then let it
		 * get garbage collected.
		 */
		try {
			ClassLoader base = ClassLoader.getSystemClassLoader();
			URL[] urls;
			if (base instanceof URLClassLoader) {
				urls = ((URLClassLoader) base).getURLs();
			} else {
				urls = new URL[] {
						new File(".").toURI().toURL() };
			}
			StapURLClassLoader loader = new StapURLClassLoader(
						urls, base.getParent());
			Class testClass = Class.forName("staptest.ClassUnloadedProbeTester", true, loader);
			testClass = null;
			loader = null;
		} catch (Exception ignore) {
			ignore.printStackTrace();
		}
	}

	private void doNotify() {
		synchronized (this) {
			waitFiveSeconds();
			notify();
		}
	}

	private void doNotifyAll() {
		synchronized (this) {
			waitFiveSeconds();
			notifyAll();
		}
	}

	private static synchronized void doSynchronized() {
		int anInt = 5;
		for (int i = 0; i < 10; i++) {
			anInt += i;
		}
		waitFiveSeconds();
	}

	private static void waitFiveSeconds() {
		try {
			Thread.sleep(5000);
		} catch (Exception ignore) {}
	}
}
