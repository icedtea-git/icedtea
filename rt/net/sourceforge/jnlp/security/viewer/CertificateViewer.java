/* CertificateViewer.java
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

package net.sourceforge.jnlp.security.viewer;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.plaf.OptionPaneUI;

public class CertificateViewer extends JOptionPane {

	private boolean initialized = false;
	
	public CertificateViewer() throws Exception {

		initialized = true;
		updateUI();
	}
		
	public boolean isInitialized(){
		return initialized;
	}
	
	public void updateUI() {
		setUI((OptionPaneUI) new CertificatePane(this));
	}
	
	private static void centerDialog(JDialog dialog) {
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension dialogSize = dialog.getSize();

		dialog.setLocation((screen.width - dialogSize.width)/2,
			(screen.height - dialogSize.height)/2);
	}
	
	//Modified from javax.swing.JOptionPane
	private JDialog createDialog() {
		String dialogTitle = "Certificates";
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
                	event.getSource() == CertificateViewer.this &&
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
	
	public static void showCertificateViewer() throws Exception {
		CertificateViewer cv = new CertificateViewer();
		JDialog dialog = cv.createDialog();
		cv.selectInitialValue();
		dialog.setResizable(true);
		centerDialog(dialog);
		dialog.setVisible(true);
		dialog.dispose();	
	}
	
	public static void main(String[] args) throws Exception {
		CertificateViewer.showCertificateViewer();
	}
}

