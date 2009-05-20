// Copyright (C) 2001-2003 Jon A. Maxwell (JAM)
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


package net.sourceforge.jnlp;

import java.applet.*;
import java.awt.Container;
import java.io.*;
import java.net.*;
import java.util.jar.JarFile;
import java.lang.reflect.*;

import net.sourceforge.jnlp.cache.*;
import net.sourceforge.jnlp.runtime.*;
import net.sourceforge.jnlp.util.*;

/**
 * Launches JNLPFiles either in the foreground or background.<p>
 *
 * An optional LaunchHandler can be specified that is notified of
 * warning and error condition while launching and that indicates
 * whether a launch may proceed after a warning has occurred.  If
 * specified, the LaunchHandler is notified regardless of whether
 * the file is launched in the foreground or background.<p>
 *
 * @author <a href="mailto:jmaxwell@users.sourceforge.net">Jon A. Maxwell (JAM)</a> - initial author
 * @version $Revision: 1.22 $ 
 */
public class Launcher {

    // defines class Launcher.BgRunner, Launcher.TgThread

    /** shortcut for resources */
    private static String R(String key) { return JNLPRuntime.getMessage(key); }

    /** shared thread group */
    private static final ThreadGroup mainGroup = new ThreadGroup(R("LAllThreadGroup"));

    /** the handler */
    private LaunchHandler handler = null;

    /** the update policy */
    private UpdatePolicy updatePolicy = JNLPRuntime.getDefaultUpdatePolicy();

    /** whether to create an AppContext (if possible) */
    private boolean context = true;

    /** If the application should call System.exit on fatal errors */
    private boolean exitOnFailure = true;

    /**
     * Create a launcher with the runtime's default update policy
     * and launch handler.
     */
    public Launcher() {
        this(null, null);

        if (handler == null)
            handler = JNLPRuntime.getDefaultLaunchHandler();
    }
    
    /**
     * Create a launcher with the runtime's default update policy
     * and launch handler.
     * 
     * @param exitOnError Exit if there is an error (usually default, but false when being used from the plugin)
     */
    public Launcher(boolean exitOnFailure) {
        this(null, null);

        if (handler == null)
            handler = JNLPRuntime.getDefaultLaunchHandler();
        
        this.exitOnFailure = exitOnFailure;
    }

    /**
     * Create a launcher with the specified handler and the
     * runtime's default update policy.
     *
     * @param handler the handler to use or null for no handler.
     */
    public Launcher(LaunchHandler handler) {
        this(handler, null);
    }

    /**
     * Create a launcher with an optional handler using the
     * specified update policy and launch handler.
     *
     * @param handler the handler to use or null for no handler.
     * @param policy the update policy to use or null for default policy.
     */
    public Launcher(LaunchHandler handler, UpdatePolicy policy) {
        if (policy == null)
            policy = JNLPRuntime.getDefaultUpdatePolicy();

        this.handler = handler;
        this.updatePolicy = policy;
    }

    /**
     * Sets the update policy used by launched applications.
     */
    public void setUpdatePolicy(UpdatePolicy policy) {
        if (policy == null)
            throw new IllegalArgumentException(R("LNullUpdatePolicy"));

        this.updatePolicy = policy;
    }

    /**
     * Returns the update policy used when launching applications.
     */
    public UpdatePolicy getUpdatePolicy() {
        return updatePolicy;
    }

    /**
     * Sets whether to launch the application in a new AppContext
     * (a separate event queue, look and feel, etc).  If the
     * sun.awt.SunToolkit class is not present then this method
     * has no effect.  The default value is true.
     */
    public void setCreateAppContext(boolean context) {
        this.context = context;
    }

    /**
     * Returns whether applications are launched in their own
     * AppContext.
     */
    public boolean isCreateAppContext() {
        return this.context;
    }

    /**
     * Launches a JNLP file by calling the launch method for the
     * appropriate file type.  The application will be started in
     * a new window.
     *
     * @param file the JNLP file to launch
     * @return the application instance
     * @throws LaunchException if an error occurred while launching (also sent to handler)
     */
    public ApplicationInstance launch(JNLPFile file) throws LaunchException {
        return launch(file, null);
    }

    /**
     * Launches a JNLP file inside the given container if it is an applet.  Specifying a
     * container has no effect for Applcations and Installers.
     *
     * @param file the JNLP file to launch
     * @param cont the container in which to place the application, if it is an applet
     * @return the application instance
     * @throws LaunchException if an error occurred while launching (also sent to handler)
     */
    public ApplicationInstance launch(JNLPFile file, Container cont) throws LaunchException {
        TgThread tg;

        if (file instanceof PluginBridge && cont != null)
        	tg = new TgThread(file, cont, true);
        else if (cont == null)
        	tg = new TgThread(file);
        else
        	tg = new TgThread(file, cont);

        tg.start();

        try {
            tg.join();
        }
        catch (InterruptedException ex) {
			//By default, null is thrown here, and the message dialog is shown.
            throw launchWarning(new LaunchException(file, ex, R("LSMinor"), R("LCSystem"), R("LThreadInterrupted"), R("LThreadInterruptedInfo")));
        }

        if (tg.getException() != null)
            throw tg.getException(); // passed to handler when first created

        if (handler != null)
            handler.launchCompleted(tg.getApplication());

        return tg.getApplication();
    }    

    /**
     * Launches a JNLP file by calling the launch method for the
     * appropriate file type.
     *
     * @param location the URL of the JNLP file to launch
     * @throws LaunchException if there was an exception 
     * @return the application instance
     */
    public ApplicationInstance launch(URL location) throws LaunchException {
        return launch(toFile(location));
    }

    /** 
     * Launches a JNLP file by calling the launch method for the
     * appropriate file type in a different thread.
     *
     * @param file the JNLP file to launch
     */
    public void launchBackground(JNLPFile file) {
        BgRunner runner = new BgRunner(file, null);
        new Thread(runner).start();
    }

    /**
     * Launches the JNLP file at the specified location in the
     * background by calling the launch method for its file type.
     *
     * @param location the location of the JNLP file
     */
    public void launchBackground(URL location) {
        BgRunner runner = new BgRunner(null, location);
        new Thread(runner).start();
    }

    /**
     * Launches the JNLP file in a new JVM instance.  The launched
     * application's output is sent to the system out and it's
     * standard input channel is closed.
     *
     * @param file the JNLP file to launch
     * @throws LaunchException if there was an exception 
     */
    public void launchExternal(JNLPFile file) throws LaunchException {
        if (file.getSourceLocation() != null)
            launchExternal(file.getSourceLocation());
        else if (file.getFileLocation() != null)
            launchExternal(file.getFileLocation());
        else
            launchError(new LaunchException(file, null, R("LSFatal"), R("LCExternalLaunch"), R("LNullLocation"), R("LNullLocationInfo")));
    }

    /**
     * Launches the JNLP file at the specified location in a new JVM
     * instance.  The launched application's output is sent to the
     * system out and it's standard input channel is closed.
     *
     * @param location the URL of the JNLP file to launch
     * @throws LaunchException if there was an exception 
     */
    public void launchExternal(URL location) throws LaunchException {
        try {
            URL cs = Launcher.class.getProtectionDomain().getCodeSource().getLocation();
            if (JNLPRuntime.isDebug())
                System.out.println("netx.jar path: "+cs.getPath());

            File netxFile = new File(cs.getPath());
            if (!netxFile.exists())
                throw launchError(new LaunchException(null, null, R("LSFatal"), R("LCExternalLaunch"), R("LNetxJarMissing"), R("LNetxJarMissingInfo")));

            String command[] = {
                "javaw",
                "-jar",
                netxFile.toString(),
                "-jnlp",
                location.toString(),
                "-verbose",
            };

            Process p = Runtime.getRuntime().exec(command);
            new StreamEater(p.getErrorStream()).start();
            new StreamEater(p.getInputStream()).start();
            p.getOutputStream().close();

        }
        catch (NullPointerException ex) {
            throw launchError(new LaunchException(null, null, R("LSFatal"), R("LCExternalLaunch"), R("LNetxJarMissing"), R("LNetxJarMissingInfo")));
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(null, ex, R("LSFatal"), R("LCExternalLaunch"), R("LCouldNotLaunch"), R("LCouldNotLaunchInfo")));
        }
    }

    /**
     * Returns the JNLPFile for the URL, with error handling.
     */
    private JNLPFile toFile(URL location) throws LaunchException {
        try { 
            JNLPFile file = null;

            try {
                file = new JNLPFile(location, null, true, updatePolicy); // strict
            }
            catch (ParseException ex) {
                file = new JNLPFile(location, null, false, updatePolicy);

                // only here if strict failed but lax did not fail 
                LaunchException lex = 
                    launchWarning(new LaunchException(file, ex, R("LSMinor"), R("LCFileFormat"), R("LNotToSpec"), R("LNotToSpecInfo")));

                if (lex != null)
                    throw lex;
            }

            return file;
        }
        catch (Exception ex) {
            if (ex instanceof LaunchException)
                throw (LaunchException) ex; // already sent to handler when first thrown
            else  // IO and Parse
                throw launchError(new LaunchException(null, ex, R("LSFatal"), R("LCReadError"), R("LCantRead"), R("LCantReadInfo")));
        }
    }

    /** 
     * Launches a JNLP application.  This method should be called
     * from a thread in the application's thread group.
     */
    protected ApplicationInstance launchApplication(JNLPFile file) throws LaunchException {
        if (!file.isApplication())
            throw launchError(new LaunchException(file, null, R("LSFatal"), R("LCClient"), R("LNotApplication"), R("LNotApplicationInfo")));

        try {
            final int preferredWidth = 500;
            final int preferredHeight = 400;
            JNLPSplashScreen splashScreen = null;
            URL splashImageURL = file.getInformation().getIconLocation(
                    IconDesc.SPLASH, preferredWidth, preferredHeight);
            if (splashImageURL != null) {
                ResourceTracker resourceTracker = new ResourceTracker(true);
                resourceTracker.addResource(splashImageURL, "SPLASH", file.getFileVersion(), updatePolicy);
                splashScreen = new JNLPSplashScreen(resourceTracker, null, null);
                splashScreen.setSplashImageURL(splashImageURL);
                if (splashScreen.isSplashScreenValid()) {
                    splashScreen.setVisible(true);
                }
            }


            ApplicationInstance app = createApplication(file);
            app.initialize();

            String mainName = file.getApplication().getMainClass();
            
            // When the application-desc field is empty, we should take a 
            // look at the main jar for the main class.
            if (mainName == null) {
            	JARDesc mainJarDesc = file.getResources().getMainJAR();
            	File f = CacheUtil.getCacheFile(mainJarDesc.getLocation(), null);
            	if (f != null) {
            		JarFile mainJar = new JarFile(f);
            		mainName = mainJar.getManifest().
            			getMainAttributes().getValue("Main-Class");
            	}
            }
            
            if (mainName == null)
            	throw launchError(new LaunchException(file, null, 
            		R("LSFatal"), R("LCClient"), R("LCantDetermineMainClass") , 
            		R("LCantDetermineMainClassInfo")));
            
            Class mainClass = app.getClassLoader().loadClass(mainName);

            Method main = mainClass.getDeclaredMethod("main", new Class[] {String[].class} );
            String args[] = file.getApplication().getArguments();

            // required to make some apps work right
            Thread.currentThread().setContextClassLoader(app.getClassLoader());

            if (splashScreen != null) {
                if (splashScreen.isSplashScreenValid()) {
                    splashScreen.setVisible(false);
                }
                splashScreen.dispose();
            }

            main.invoke(null, new Object[] { args } );

            return app;
        }
        catch (LaunchException lex) {
            throw launchError(lex);
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(file, ex, R("LSFatal"), R("LCLaunching"), R("LCouldNotLaunch"), R("LCouldNotLaunchInfo")));
        }
    }

    /** 
     * Launches a JNLP applet. This method should be called from a
     * thread in the application's thread group.<p>
     *
     * The enableCodeBase parameter adds the applet's codebase to
     * the locations searched for resources and classes.  This can
     * slow down the applet loading but allows browser-style applets
     * that don't use JAR files exclusively to be run from a applet
     * JNLP file.  If the applet JNLP file does not specify any
     * resources then the code base will be enabled regardless of
     * the specified value.<p>
     *
     * @param file the JNLP file
     * @param enableCodeBase whether to add the codebase URL to the classloader
     */
    protected ApplicationInstance launchApplet(JNLPFile file, boolean enableCodeBase, Container cont) throws LaunchException {
        if (!file.isApplet())
            throw launchError(new LaunchException(file, null, R("LSFatal"), R("LCClient"), R("LNotApplet"), R("LNotAppletInfo")));

        try {
            AppletInstance applet = createApplet(file, enableCodeBase, cont);
            applet.initialize();
            
            applet.getAppletEnvironment().startApplet(); // this should be a direct call to applet instance
            return applet;
        }
        catch (LaunchException lex) {
            throw launchError(lex);
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(file, ex, R("LSFatal"), R("LCLaunching"), R("LCouldNotLaunch"), R("LCouldNotLaunchInfo")));
        }
    }
    
    /**
     * Gets an ApplicationInstance, but does not launch the applet.
     */
    protected ApplicationInstance getApplet(JNLPFile file, boolean enableCodeBase, Container cont) throws LaunchException {
        if (!file.isApplet())
            throw launchError(new LaunchException(file, null, R("LSFatal"), R("LCClient"), R("LNotApplet"), R("LNotAppletInfo")));

        try {
            AppletInstance applet = createApplet(file, enableCodeBase, cont);
            applet.initialize();
            return applet;
        }
        catch (LaunchException lex) {
            throw launchError(lex);
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(file, ex, R("LSFatal"), R("LCLaunching"), R("LCouldNotLaunch"), R("LCouldNotLaunchInfo")));
        }
    }

    /** 
     * Launches a JNLP installer.  This method should be called from
     * a thread in the application's thread group.
     */
    protected ApplicationInstance launchInstaller(JNLPFile file) throws LaunchException {
        throw launchError(new LaunchException(file, null, R("LSFatal"), R("LCNotSupported"), R("LNoInstallers"), R("LNoInstallersInfo")));
    }

    /**
     * Create an AppletInstance.
     *
     * @param enableCodeBase whether to add the code base URL to the classloader
     */
    protected AppletInstance createApplet(JNLPFile file, boolean enableCodeBase, Container cont) throws LaunchException {
        try {
            JNLPClassLoader loader = JNLPClassLoader.getInstance(file, updatePolicy);

            if (enableCodeBase || file.getResources().getJARs().length == 0)
                loader.enableCodeBase();

            AppThreadGroup group = (AppThreadGroup) Thread.currentThread().getThreadGroup();

            String appletName = file.getApplet().getMainClass();

			//Classloader chokes if there's '/' in the path to the main class.
			//Must replace with '.' instead.
			appletName = appletName.replace('/', '.');
            Class appletClass = loader.loadClass(appletName);
            Applet applet = (Applet) appletClass.newInstance();

            AppletInstance appletInstance;
            if (cont == null)
              appletInstance = new AppletInstance(file, group, loader, applet);
            else
              appletInstance = new AppletInstance(file, group, loader, applet, cont);

            group.setApplication(appletInstance);
            loader.setApplication(appletInstance);

            return appletInstance;
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(file, ex, R("LSFatal"), R("LCInit"), R("LInitApplet"), R("LInitAppletInfo")));
        }
    }

    /**
     * Creates an Applet object from a JNLPFile. This is mainly to be used with
     * gcjwebplugin.
     * @param file the PluginBridge to be used.
     * @param enableCodeBase whether to add the code base URL to the classloader.
     */
    protected Applet createAppletObject(JNLPFile file, boolean enableCodeBase, Container cont) throws LaunchException {
        try {
            JNLPClassLoader loader = JNLPClassLoader.getInstance(file, updatePolicy);

            if (enableCodeBase || file.getResources().getJARs().length == 0)
                loader.enableCodeBase();

            String appletName = file.getApplet().getMainClass();

			//Classloader chokes if there's '/' in the path to the main class.
			//Must replace with '.' instead.
			appletName = appletName.replace('/', '.');
            Class appletClass = loader.loadClass(appletName);
            Applet applet = (Applet) appletClass.newInstance();

            return applet;
        }
        catch (Exception ex) {
            throw launchError(new LaunchException(file, ex, R("LSFatal"), R("LCInit"), R("LInitApplet"), R("LInitAppletInfo")));
        }
    }
    
    /**
     * Creates an Application.
     */
    protected ApplicationInstance createApplication(JNLPFile file) throws LaunchException {
        try {
            JNLPClassLoader loader = JNLPClassLoader.getInstance(file, updatePolicy);
            AppThreadGroup group = (AppThreadGroup) Thread.currentThread().getThreadGroup();

            ApplicationInstance app = new ApplicationInstance(file, group, loader);
            group.setApplication(app);
            loader.setApplication(app);

            return app;
        }
        catch (Exception ex) {
            throw new LaunchException(file, ex, R("LSFatal"), R("LCInit"), R("LInitApplet"), R("LInitAppletInfo"));
        }
    }

    /**
     * Create a thread group for the JNLP file.
     */
    protected AppThreadGroup createThreadGroup(JNLPFile file) {
        return new AppThreadGroup(mainGroup, file.getTitle());
    }

    /**
     * Send n launch error to the handler, if set, and also to the
     * caller.
     */
    private LaunchException launchError(LaunchException ex) {
        if (handler != null)
            handler.launchError(ex);

        return ex;
    }

    /**
     * Send a launch error to the handler, if set, and to the
     * caller only if the handler indicated that the launch should
     * continue despite the warning.
     *
     * @return an exception to throw if the launch should be aborted, or null otherwise
     */
    private LaunchException launchWarning(LaunchException ex) {
        if (handler != null)
            if (!handler.launchWarning(ex))
                // no need to destroy the app b/c it hasn't started
                return ex; // chose to abort

        return null; // chose to continue, or no handler
    }



    /**
     * This runnable is used to call the appropriate launch method
     * for the application, applet, or installer in its thread group.
     */
    private class TgThread extends Thread { // ThreadGroupThread
        private JNLPFile file;
        private ApplicationInstance application;
        private LaunchException exception;
        private Container cont;
        private boolean isPlugin = false;

        TgThread(JNLPFile file) {
            this(file, null);
        }

        TgThread(JNLPFile file, Container cont) {
            super(createThreadGroup(file), file.getTitle());

            this.file = file;
            this.cont = cont;
        }
        
        TgThread(JNLPFile file, Container cont, boolean isPlugin) {
            super(createThreadGroup(file), file.getTitle());
            this.file = file;
            this.cont = cont;
            this.isPlugin = isPlugin;
        }

        public void run() {
            try {
                // Do not create new AppContext if we're using NetX and gcjwebplugin.
                if (context && !isPlugin)
                	new Reflect().invokeStatic("sun.awt.SunToolkit", "createNewAppContext");

                if (isPlugin) {
                	// Do not display download indicators if we're using gcjwebplugin.
                	JNLPRuntime.setDefaultDownloadIndicator(null);
                	application = getApplet(file, true, cont);
                } else {
                	if (file.isApplication())
                		application = launchApplication(file);
                	else if (file.isApplet())
                		application = launchApplet(file, true, cont); // enable applet code base
                	else if (file.isInstaller())
                		application = launchInstaller(file);
                	else 
                		throw launchError(new LaunchException(file, null, 
                				R("LSFatal"), R("LCClient"), R("LNotLaunchable"), 
                				R("LNotLaunchableInfo")));
                }
            }
            catch (LaunchException ex) {
                ex.printStackTrace();
                exception = ex;
                // Exit if we can't launch the application.
                if (exitOnFailure)
                	System.exit(0);
            }
        }

        public LaunchException getException() {
            return exception;
        }

        public ApplicationInstance getApplication() {
            return application;
        }
        
    };


    /**
     * This runnable is used by the <code>launchBackground</code>
     * methods to launch a JNLP file from a separate thread.
     */
    private class BgRunner implements Runnable {
        private JNLPFile file;
        private URL location;

        BgRunner(JNLPFile file, URL location) {
            this.file = file;
            this.location = location;
        }

        public void run() {
            try {
                if (file != null)
                    launch(file);
                if (location != null)
                    launch(location);
            }
            catch (LaunchException ex) {
                // launch method communicates error conditions to the
                // handler if it exists, otherwise we don't care because
                // there's nothing that can be done about the exception.
            }
        }
    };

    /**
     * This class reads the output from a launched process and
     * writes it to stdout.
     */
    private static class StreamEater extends Thread {
        private InputStream stream;

        StreamEater(InputStream stream) {
            this.stream = new BufferedInputStream(stream);
        }

        public void run() {
            try {
                while (true) {
                    int c = stream.read();
                    if (c == -1)
                        break;

                    System.out.write(c);
                }
            }
            catch (IOException ex) {
            }
        }
    };

}


