package staptest;

import java.io.File;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;

public class SystemtapTester {
	static String throwAwayString;
	public static void main(String[] args) {
		SystemtapTester tester = new SystemtapTester();
		throwAwayString = (args.length < 1 ? "NOTHING"
				   : ("ARG: " + args[0]));
		System.out.println(throwAwayString);
		if (args.length < 1) {
			System.exit(1);
		} else if (args[0].equals("vm_init_begin") ||
				args[0].equals("vm_init_end") ||
				args[0].equals("vm_shutdown") ||
				args[0].equals("object_alloc") ||
				args[0].equals("method_entry") ||
				args[0].equals("method_return")) {
			// In this case, hotspot has already triggered these
			// probes (or will shortly).  No need to continue.
			System.exit(0);
		} else if (args[0].equals("gc_begin") ||
				args[0].equals("gc_end") ||
				args[0].equals("mem_pool_gc_begin") ||
				args[0].equals("mem_pool_gc_end")) {
			tester.triggerGarbageCollect();
		} else if (args[0].equals("method_compile_begin") ||
				args[0].equals("method_compile_end") ||
				args[0].equals("compiled_method_load")) {
			tester.triggerCompileMethod();
			// The jit compiler might be working in the background
			// wait a little to make sure it is finished. Then
			// repeat the test with a "hot jit".
			try {
				Thread.sleep(1000);
			} catch (InterruptedException ie) { }
			tester.triggerCompileMethod();
		} else if (args[0].equals("compiled_method_unload")) {
			tester.triggerUnloadMethod();
			// The jit compiler might be working in the background
			// wait a little to make sure it is finished. Then
			// repeat the test with a "hot jit".
			try {
				Thread.sleep(1000);
			} catch (InterruptedException ie) { }
			tester.triggerUnloadMethod();
		} else if (args[0].equals("thread_start") ||
				args[0].equals("thread_stop")) {
			tester.triggerThread();
		} else if (args[0].equals("class_loaded") ||
				args[0].equals("class_unloaded")) {
			tester.triggerClassLoader();
			tester.triggerGarbageCollect();
		} else if (args[0].equals("monitor_contended_enter") || 
				args[0].equals("monitor_contended_entered") ||
				args[0].equals("monitor_contended_exit")) {
			tester.triggerContended();
		} else if (args[0].equals("monitor_wait") ||
				args[0].equals("monitor_waited") ||
				args[0].equals("monitor_notify")) {
			tester.triggerWait(TestingRunner.NOTIFY);
		} else if (args[0].equals("monitor_notifyAll")) {
			tester.triggerWait(TestingRunner.NOTIFYALL);
		}
		System.out.println(throwAwayString);
	}

	public void triggerGarbageCollect() {
		for (int i = 0; i < 1000; i++) {
			throwAwayString = allocateForNoReason(i);
		}
		forceGarbageCollect();
	}

	public void triggerThread() {
		Thread basicThread = new Thread(
				new TestingRunner(TestingRunner.NOOP));
		basicThread.start();
		while (basicThread.isAlive()) {
			try {
				basicThread.join();
			} catch (Exception ignore) {}
		}
	}

	public void triggerCompileMethod() {
		for (int i = 0; i < 105; i++) {
			Integer iobj = new Integer(i);
			throwAwayString = triggerCompileMethodLoop(iobj);
		}
	}

	public String triggerCompileMethodLoop(Integer iobj) {
		for (int i = 0; i < 105; i++) {
			throwAwayString = allocateForNoReason(iobj.intValue());
		}
		return throwAwayString;
	}

	public void triggerUnloadMethod() {
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
			Class<?> runClass = loader.loadClass("staptest.RunWrapper");
			String[] aargs = new String[] { "staptest.RunWrapper", 
					"staptest.ClassUnloadedProbeTester"};
			Class[] argTypes = new Class[] { aargs.getClass() };
			Method main = runClass.getDeclaredMethod("main", argTypes);
			String[] bargs = new String[1];
			bargs[0] = new String(aargs[1]);
			Thread.currentThread().setContextClassLoader(loader);
			main.invoke(null, new Object[] { bargs });
			Thread.currentThread().setContextClassLoader(base);
			loader = null;
			runClass = null;
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		triggerGarbageCollect();
	}

	public void triggerClassLoader() {
		ThreadGroup aThreadGroup = new ThreadGroup("CustomClassLoadingThreadGroup");
		Thread classLoadingThread = new Thread(aThreadGroup,
				new TestingRunner(TestingRunner.LOADER));
		classLoadingThread.start();
		while (classLoadingThread.isAlive()) {
			try {
				classLoadingThread.join();
			} catch (Exception ignore) {}
		}
		aThreadGroup.destroy();
	}

	public void triggerContended() {
		Thread contendThread1 =
			new Thread(new TestingRunner(TestingRunner.CONTENDER));
		Thread contendThread2 =
			new Thread(new TestingRunner(TestingRunner.CONTENDER));
		contendThread1.start();
		contendThread2.start();
		while (contendThread1.isAlive() || contendThread2.isAlive()) {
			try {
				contendThread1.join();
				contendThread2.join();
			} catch (Exception ignore) {}
		}
	}

	public void triggerWait(int whichNotify) {
		TestingRunner myRunner = new TestingRunner(whichNotify);
		Thread target = new Thread(myRunner);
		synchronized (myRunner) {
			target.start();
			try {
				myRunner.wait();
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
		while (target.isAlive()) {
			try {
				target.join();
			} catch (Exception ignore) {
			}
		}
	}

	private void forceGarbageCollect() {
		Runtime rt = Runtime.getRuntime();
		long memFree = rt.freeMemory();
		long beginTime = System.currentTimeMillis();
		long currentTime = beginTime;
		do {
			rt.gc();
			try {
				Thread.sleep(1000);
			} catch (Exception ignore) {}
			currentTime = System.currentTimeMillis();
		} while ((memFree <= rt.freeMemory()) && 
			((currentTime - beginTime) < 5000));
	}

	String allocateForNoReason(int i) {
		String aString = new String("sometextinastring" + i);
		return aString;
	}

}

