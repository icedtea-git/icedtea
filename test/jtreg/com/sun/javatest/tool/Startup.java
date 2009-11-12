/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
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
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */
package com.sun.javatest.tool;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.Label;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.awt.Window;
import java.net.URL;
import java.util.ResourceBundle;

import com.sun.javatest.ProductInfo;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * A lightweight class to display a startup (splash) image.
 * The class ins intentionally as lightweight as possible,
 * to be fast during startup. It does not use Swing.
 */
public class Startup
{
    /**
     * Create an object to display a default startup image.
     * The image is defined by the resource "startup.icon"
     * in the resource bundle com.sun.javatest.tool.i18n.properties.
     */
    public Startup() {
        showDefault();
    }

    /**
     * Create an object to display a startup screen.
     * @param i18n A resource bundle that defines the content of the startup screen.
     * @param prefix A prefix for resources in the resource file to display in
     * the startup screen. The object checks to see if the resource
     * "<i>prefix</i>.icon" is defined: if so, it is used as the URL of a
     * resource containing an image to be display. If "<i>prefix</i>.icon" is
     * not defined, the Startup object checks for three strings
     * "<i>prefix</i>.line1", "<i>prefix</i>.line2" and "<i>prefix</i>.line3",
     * which will be displayed in a simple Frame containing three lines of text.
     */
    public Startup(I18NResourceBundle i18n, String prefix) {
        show(i18n, prefix);
    }

    /**
     * Create an object to display a startup screen.
     * This constructor is special in that it takes a non-package-specific
     * resource bundle.
     * If the image cannot be located or loaded, the default harness splash screen
     * will be used.
     * @param i18n A resource bundle that defines the content of the startup screen.
     * @param prefix A prefix for resources in the resource file to display in
     * the startup screen. The object checks to see if the resource
     * "<i>prefix</i>.icon" is defined: if so, it is used as the URL of a
     * resource containing an image to be display. If "<i>prefix</i>.icon" is
     * not defined, the Startup object checks for three strings
     * "<i>prefix</i>.line1", "<i>prefix</i>.line2" and "<i>prefix</i>.line3",
     * which will be displayed in a simple Frame containing three lines of text.
     * @param path Path to load the file from, use <code>null</code> if this class'
     *        class loader should be used to load the image resource.  The full
     *        path is constructed by adding the resource filename to the
     *        <i>prefix</i>.
     */
    public Startup(URL url) {
        // note, this gets the image from the given URL, but uses the standard
        // javatest bundle for the rest of the properties
        show(url, I18NResourceBundle.getBundleForClass(Startup.class), "startup");
    }

    private void showDefault() {
        show(I18NResourceBundle.getBundleForClass(Startup.class), "startup");
    }

    private void show(I18NResourceBundle i18n, String prefix) {
        String iconName = i18n.getOptionalString(prefix + ".icon");
        URL url = null;

        if (iconName != null) {
            url = getClass().getClassLoader().getResource(iconName);
        }

        if (url == null)
            showDefault();
        else
            show(url, i18n, prefix);
    }

    private void show(URL iconURL, I18NResourceBundle i18n, String prefix) {
        image = Toolkit.getDefaultToolkit().createImage(iconURL);

        Canvas c = new Canvas() {
            public Dimension getPreferredSize() {
                return imageSize;
            }

            public void update(Graphics g) {
                paint(g);
            }

            public void paint(Graphics g) {
                g.drawImage(image, 0, 0, null);
            }
        };
        c.setFocusable(false);

        try {
            MediaTracker t = new MediaTracker(c);
            t.addImage(image, 0);
            t.waitForAll();
            imageSize = new Dimension(image.getWidth(null), image.getHeight(null));
            frame = new Frame();
            setAccessibleInfo(frame, i18n, prefix + ".splash");
            frame.setUndecorated(true);
            frame.setFocusableWindowState(false);
            frame.add(c);
        }
        catch (InterruptedException e) {
        }

        if (imageSize == null) {
            frame = new Frame(ProductInfo.getName() + " " + ProductInfo.getVersion());
            setAccessibleInfo(frame, i18n, prefix + ".splash");
            frame.setBackground(Color.white);
            frame.add(new Label(i18n.getString(prefix + ".line1"), Label.CENTER), "North");
            frame.add(new Label(i18n.getString(prefix + ".line2"), Label.CENTER), "Center");
            frame.add(new Label(i18n.getString(prefix + ".line3"), Label.CENTER), "South");
            frame.add(new Label("          "), "East");
            frame.add(new Label("           "), "West");
            frame.setResizable(false);
        }

        frame.pack();

        //Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension size = frame.getSize();
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        //frame.setLocation(screenSize.width/2 - size.width/2, screenSize.height/2 - size.height/2);
        frame.setLocation(ge.getCenterPoint().x - size.width/2,
                          ge.getCenterPoint().y - size.height/2);
        frame.setVisible(true);
    }

    Window getWindow() {
        return frame;
    }

    /**
     * Dispose of the window resources used by this object.
     */
    public void dispose() {
        frame.dispose();
    }

    /**
     * Dispose of the window resources used by this object,
     * by scheduling an event to be run on the AWT Event Queue.
     */
    public void disposeLater() {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                dispose();
            }
        });
    }

    private void setAccessibleInfo(Frame f, ResourceBundle i18n, String uiKey) {
        f.getAccessibleContext().setAccessibleName(i18n.getString(uiKey + ".name"));
        f.getAccessibleContext().setAccessibleDescription(i18n.getString(uiKey + ".desc"));
    }

    private Frame frame;
    private Image image;
    private Dimension imageSize;
}
