package staptest;

import java.lang.reflect.Method;

public class RunWrapper {
	public static void main(String[] args) {
		if (args.length >= 1) {
			try {
				Class<?> mainClass = RunWrapper.class.
					getClassLoader().loadClass(args[0]);
				Class[] types = new Class[] { args.getClass() };
				Method main = mainClass.getDeclaredMethod(
					"main", types);
				String[] runArgs = new String[args.length-1];
				System.arraycopy(args, 1, runArgs, 0,
					runArgs.length);
				main.invoke(null, new Object[] { runArgs });
			} catch (Exception ignore) {}
		}
	}
}
