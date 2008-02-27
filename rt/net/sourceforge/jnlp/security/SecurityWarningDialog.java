/* SecurityWarningDialog.java
   Copyright (C) 2008 Red Hat, Inc.

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.

IcedTea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version.
*/

package net.sourceforge.jnlp.security;

import net.sourceforge.jnlp.JNLPFile;
import net.sourceforge.jnlp.tools.JarSigner;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import javax.swing.plaf.OptionPaneUI;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.util.ArrayList;
import java.security.cert.CertPath;

/**
 * Provides methods for showing security warning dialogs
 * for a wide range of JNLP security issues.
 *
 * @author <a href="mailto:jsumali@redhat.com">Joshua Sumali</a>
 */
public class SecurityWarningDialog extends JOptionPane {

	/** Types of dialogs we can create */
	public static enum DialogType {
		CERT_WARNING,
		MORE_INFO,
		CERT_INFO,
		ACCESS_WARNING,
		APPLET_WARNING
	}
	
	/** The types of access which may need user permission. */
	public static enum AccessType {
        READ_FILE,
        WRITE_FILE,
        CLIPBOARD_READ,
        CLIPBOARD_WRITE,
        PRINTER,
        VERIFIED,
        UNVERIFIED,
        SIGNING_ERROR
    }

	/** The type of dialog we want to show */
	private DialogType dialogType;

	/** The type of access that this dialog is for */
	private AccessType accessType;

	/** The application file assocated with this security warning */
	private JNLPFile file;

	private JarSigner jarSigner;
	
	/** Whether or not this object has been fully initialized */
	private boolean initialized = false;

	public SecurityWarningDialog(DialogType dialogType, AccessType accessType,
			JNLPFile file) {
		this.dialogType = dialogType;
		this.accessType = accessType;
		this.file = file;
		this.jarSigner = null;
		initialized = true;
		updateUI();
	}
	
	public SecurityWarningDialog(DialogType dialogType, AccessType accessType,
			JNLPFile file, JarSigner jarSigner) {
			this.dialogType = dialogType;
			this.accessType = accessType;
			this.file = file;
			this.jarSigner = jarSigner;
			initialized = true;
			updateUI();
	}
	
	/**
	 * Returns if this dialog has been fully initialized yet.
	 * @return true if this dialog has been initialized, and false otherwise.
	 */
	public boolean isInitialized(){
		return initialized;
	}
	
	/**
	 * Shows a warning dialog for different types of system access (i.e. file
	 * open/save, clipboard read/write, printing, etc).
	 * 
	 * @param accessType the type of system access requested.
	 * @param file the jnlp file associated with the requesting application.
	 * @return true if permission was granted by the user, false otherwise.
	 */
	public static boolean showAccessWarningDialog(AccessType accessType, 
		JNLPFile file) {
		SecurityWarningDialog swd = new SecurityWarningDialog(
				DialogType.ACCESS_WARNING, accessType, file);
		JDialog dialog = swd.createDialog();
		swd.selectInitialValue();
		dialog.setResizable(true);
		centerDialog(dialog);
		dialog.setVisible(true);
		dialog.dispose();

		Object selectedValue = swd.getValue();
		if (selectedValue == null) {
			return false;
		} else if (selectedValue instanceof Integer) {
			if (((Integer)selectedValue).intValue() == 0)
				return true;
			else
				return false;
		} else {
			return false;
		}
	}
	
	/**
	 * Shows a security warning dialog according to the specified type of
	 * access. If <code>type</code> is one of AccessType.VERIFIED or
	 * AccessType.UNVERIFIED, extra details will be available with regards
	 * to code signing and signing certificates.
	 *
	 * @param accessType the type of warning dialog to show
	 * @param file the JNLPFile associated with this warning
	 * @param jarSigner the JarSigner used to verify this application
	 */
	public static boolean showCertWarningDialog(AccessType accessType, 
			JNLPFile file, JarSigner jarSigner) {
		SecurityWarningDialog swd = 
			new SecurityWarningDialog(DialogType.CERT_WARNING, accessType, file,
			jarSigner);
		JDialog dialog = swd.createDialog();
		swd.selectInitialValue();
		dialog.setResizable(true);
		centerDialog(dialog);
		dialog.setVisible(true);
		dialog.dispose();

		Object selectedValue = swd.getValue();
		if (selectedValue == null) {
			return false;
		} else if (selectedValue instanceof Integer) {
			if (((Integer)selectedValue).intValue() == 0)
				return true;
			else
				return false;
		} else {
			return false;
		}
	}
	
	/**
	 * Shows more information regarding jar code signing
	 *
	 * @param jarSigner the JarSigner used to verify this application
	 * @param parent the parent option pane
	 */
	public static void showMoreInfoDialog(
		JarSigner jarSigner, JOptionPane parent) {

		SecurityWarningDialog swd =
			new SecurityWarningDialog(DialogType.MORE_INFO, null, null,
			jarSigner);
		JDialog dialog = swd.createDialog();
		dialog.setLocationRelativeTo(parent);
		swd.selectInitialValue();
		dialog.setResizable(true);
		dialog.setVisible(true);
		dialog.dispose();
	}

	/**
	 * Displays CertPath information in a readable table format.
	 *
	 * @param certs the certificates used in signing.
	 */
	public static void showCertInfoDialog(JarSigner jarSigner,
		JOptionPane parent) {
		SecurityWarningDialog swd = new SecurityWarningDialog(DialogType.CERT_INFO,
			null, null, jarSigner);
		JDialog dialog = swd.createDialog();
		dialog.setLocationRelativeTo(parent);
		swd.selectInitialValue();
		dialog.setResizable(true);
		dialog.setVisible(true);
		dialog.dispose();
	}

	public static int showAppletWarning() {
        	SecurityWarningDialog swd = new SecurityWarningDialog(DialogType.APPLET_WARNING,
            		null, null, null);
        	JDialog dialog = swd.createDialog();
		centerDialog(dialog);
        	swd.selectInitialValue();
        	dialog.setResizable(true);
        	dialog.setVisible(true);
        	dialog.dispose();

		Object selectedValue = swd.getValue();

		//result 0 = Yes, 1 = No, 2 = Cancel
		if (selectedValue == null) {
			return 2;
		} else if (selectedValue instanceof Integer) {
			return ((Integer)selectedValue).intValue();
		} else {
			return 2;
		}
	}

	//Modified from javax.swing.JOptionPane
	private JDialog createDialog() {
		String dialogTitle = "";
		if (dialogType == DialogType.CERT_WARNING)
			dialogTitle = "Warning - Security";
		else if (dialogType == DialogType.MORE_INFO)
			dialogTitle = "More Information";
		else if (dialogType == DialogType.CERT_INFO)
			dialogTitle = "Details - Certificate";
		else if (dialogType == DialogType.ACCESS_WARNING)
			dialogTitle = "Security Warning";
		else if (dialogType == DialogType.APPLET_WARNING)
			dialogTitle = "Applet Warning";

		final JDialog dialog = new JDialog((Frame)null, dialogTitle, true);
		
		Container contentPane = dialog.getContentPane();
		contentPane.setLayout(new BorderLayout());
		contentPane.add(this, BorderLayout.CENTER);
		dialog.pack();

		WindowAdapter adapter = new WindowAdapter() {
            private boolean gotFocus = false;
            public void windowClosing(WindowEvent we) {
                setValue(null);
            }
            public void windowGainedFocus(WindowEvent we) {
                // Once window gets focus, set initial focus
                if (!gotFocus) {
                    selectInitialValue();
                    gotFocus = true;
                }
            }
        };
		dialog.addWindowListener(adapter);
		dialog.addWindowFocusListener(adapter);

		dialog.addComponentListener(new ComponentAdapter() {
            public void componentShown(ComponentEvent ce) {
                // reset value to ensure closing works properly
                setValue(JOptionPane.UNINITIALIZED_VALUE);
            }
        });

		addPropertyChangeListener( new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent event) {
                // Let the defaultCloseOperation handle the closing
                // if the user closed the window without selecting a button
                // (newValue = null in that case).  Otherwise, close the dialog.
                if (dialog.isVisible() && 
                	event.getSource() == SecurityWarningDialog.this &&
                	(event.getPropertyName().equals(VALUE_PROPERTY) ||
                	event.getPropertyName().equals(INPUT_VALUE_PROPERTY)) &&
                	event.getNewValue() != null &&
                	event.getNewValue() != JOptionPane.UNINITIALIZED_VALUE) {
                    dialog.setVisible(false);
                }
            }
        });

		return dialog;
	}

	public AccessType getType() {
		return accessType;
	}

	public JNLPFile getFile() {
		return file;
	}
	
	public JarSigner getJarSigner() {
		return jarSigner;
	}

	/**
	 * Updates the UI using SecurityWarningOptionPane, instead of the
	 * basic dialog box.
	 */
	public void updateUI() {

		if (dialogType == DialogType.CERT_WARNING)
			setUI((OptionPaneUI) new CertWarningPane(this));
		else if (dialogType == DialogType.MORE_INFO)
			setUI((OptionPaneUI) new MoreInfoPane(this));
		else if (dialogType == DialogType.CERT_INFO)
			setUI((OptionPaneUI) new CertsInfoPane(this));
		else if (dialogType == DialogType.ACCESS_WARNING)
			setUI((OptionPaneUI) new AccessWarningPane(this));
		else if (dialogType == DialogType.APPLET_WARNING)
			setUI((OptionPaneUI) new AppletWarningPane(this));
	}

	private static void centerDialog(JDialog dialog) {
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension dialogSize = dialog.getSize();

		dialog.setLocation((screen.width - dialogSize.width)/2,
			(screen.height - dialogSize.height)/2);
	}
}
