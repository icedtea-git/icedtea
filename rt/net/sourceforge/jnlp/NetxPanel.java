/*
 * Copyright 2007 Red Hat, Inc.
 * This file is part of IcedTea, http://icedtea.classpath.org
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
 */

package net.sourceforge.jnlp;

import net.sourceforge.jnlp.runtime.AppletInstance;
import net.sourceforge.jnlp.runtime.JNLPRuntime;

import java.net.URL;
import java.util.Hashtable;
import sun.applet.*;

/**
 * This panel calls into netx to run an applet, and pipes the display
 * into a panel from gcjwebplugin.
 *
 * @author      Francis Kung <fkung@redhat.com>
 */
public class NetxPanel extends AppletViewerPanel
{
    private PluginBridge bridge = null;

    public NetxPanel(URL documentURL, Hashtable atts)
    {
        super(documentURL, atts);
    }

    public void run()
    {
        Thread curThread = Thread.currentThread();
        boolean disposed = false;
        while (!disposed && !curThread.isInterrupted()) {
            AppletEvent evt;
            try {
                evt = getNextEvent();
            } catch (InterruptedException e) {
                showAppletStatus("bail");
                return;
            }

            try {
                switch (evt.getID()) {
                  case APPLET_LOAD:
	          case APPLET_INIT:
                      // TODO: split out the load/init/start steps in Netx
                      break;

                  case APPLET_START:
                      // FIXME: in sun.applet.AppletPanel, the validation and running
                      // of the applet all happen in the event dispatch thread.  Need
                      // to carefully audit what they do and where, and see if we need
                      // to do the same here or in netx code.

                      bridge = new PluginBridge(baseURL,
                                                getDocumentBase(),
                                                getJarFiles(),
                                                getCode(),
                                                getWidth(), getHeight(),
                                                atts);

                      if (! JNLPRuntime.isInitialized())
                          JNLPRuntime.initialize();

                      Launcher l = new Launcher();
                      dispatchAppletEvent(APPLET_LOADING_COMPLETED, null); // not quite..

                      applet = ((AppletInstance)l.launch(bridge, this)).getApplet();

                      status = APPLET_START;
                      showAppletStatus("started");
                      break;

                case APPLET_STOP:
                    if (status != APPLET_START) {
                        showAppletStatus("notstarted");
                        break;
                    }

                    status = APPLET_STOP;

                    applet.setVisible(false);
                    applet.stop();

                    showAppletStatus("stopped");
                    break;

               case APPLET_DESTROY:
                    if (status != APPLET_STOP && status != APPLET_INIT) {
                        showAppletStatus("notstopped");
                        break;
                    }
                    status = APPLET_DESTROY;

                    applet.destroy();
                    showAppletStatus("destroyed");
                    break;

                case APPLET_DISPOSE:
                    if (status != APPLET_DESTROY && status != APPLET_LOAD) {
                        showAppletStatus("notdestroyed");
                        break;
                    }
                    status = APPLET_DISPOSE;

                    remove(applet);
                    applet = null;
                    showAppletStatus("disposed");
                    disposed = true;
                    break;

                case APPLET_QUIT:
                    return;
                }
            } catch (Exception e) {
                status = APPLET_ERROR;
                if (e.getMessage() != null) {
                    showAppletStatus("exception2", e.getClass().getName(),
                                     e.getMessage());
                } else {
                    showAppletStatus("exception", e.getClass().getName());
                }
                showAppletException(e);
            } catch (ThreadDeath e) {
                showAppletStatus("death");
                return;
            } catch (Error e) {
                status = APPLET_ERROR;
                if (e.getMessage() != null) {
                    showAppletStatus("error2", e.getClass().getName(),
                                     e.getMessage());
                } else {
                    showAppletStatus("error", e.getClass().getName());
                }
                showAppletException(e);
            }
            clearLoadAbortRequest();
        }
    }

}
