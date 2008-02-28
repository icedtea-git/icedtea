/* SecurityDialogUI.java
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

import java.awt.*;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.plaf.OptionPaneUI;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.ComponentAdapter;

import net.sourceforge.jnlp.JNLPFile;
import net.sourceforge.jnlp.runtime.JNLPRuntime;

/**
 * Provides a base for JNLP warning dialogs.
 *
 * @author <a href="mailto:jsumali@redhat.com">Joshua Sumali</a>
 */
public abstract class SecurityDialogUI extends OptionPaneUI {

	/** The JOptionPane that we're providing the L&F for */
	protected JOptionPane optionPane;

	/** Component to receive focus when messaged with selectInitialValue. */
	Component initialFocusComponent;

	/** PropertyChangeListener for <code>optionPane</code> */
	private PropertyChangeListener propertyChangeListener;
	private Handler handler;

	public SecurityDialogUI(JComponent x){
		optionPane = (JOptionPane)x;
	}

	/**
	 * Installs the user interface for the SecurityWarningDialog.
	 */
	public void installUI(JComponent c) {

		//Only install the UI when type and file in SecurityWarningDialog
		//have been set.
		if (((SecurityWarningDialog)c).isInitialized()) {
			setSystemLookAndFeel();
			optionPane = (JOptionPane)c;
			optionPane.setLayout(new BorderLayout());
			installComponents();
			installListeners();
		}
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	protected void installListeners() {
		if ((propertyChangeListener = getHandler()) != null)
			optionPane.addPropertyChangeListener(propertyChangeListener);
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	protected void uninstallComponents() {
		initialFocusComponent = null;
		optionPane.removeAll();
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	private Handler getHandler() {
		if (handler == null)
			handler = new Handler();
		return handler;
	}

	//Inherited from OptionPaneUI
	//Modified from javax.swing.plaf.basic.BasicOptionPaneUI
	public void selectInitialValue(JOptionPane op) {
		if (initialFocusComponent != null)
			initialFocusComponent.requestFocus();

		if (initialFocusComponent instanceof JButton) {
			JRootPane root = SwingUtilities.getRootPane(initialFocusComponent);
			if (root != null)
				root.setDefaultButton((JButton) initialFocusComponent);
		}
	}

	//Inherited from OptionPaneUI
	public boolean containsCustomComponents(JOptionPane op) {
		return false;
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	protected ActionListener createButtonActionListener(int buttonIndex) {
		return new ButtonActionListener(buttonIndex);
	}

	private static String R(String key) {
		return JNLPRuntime.getMessage(key);
	}

	/**
	 * Needed to get word wrap working in JLabels.
	 */
	private String htmlWrap (String s) {
		return "<html>"+s+"</html>";
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	private class Handler implements PropertyChangeListener {
		public void propertyChange(PropertyChangeEvent e) {
			if (e.getSource() == optionPane) {
				String changeName = e.getPropertyName();
				if (changeName == JOptionPane.OPTIONS_PROPERTY ||
				        changeName == JOptionPane.INITIAL_VALUE_PROPERTY ||
				        changeName == JOptionPane.ICON_PROPERTY ||
				        changeName == JOptionPane.MESSAGE_TYPE_PROPERTY ||
				        changeName == JOptionPane.OPTION_TYPE_PROPERTY ||
				        changeName == JOptionPane.MESSAGE_PROPERTY ||
				        changeName == JOptionPane.SELECTION_VALUES_PROPERTY ||
				        changeName == JOptionPane.INITIAL_SELECTION_VALUE_PROPERTY ||
				        changeName == JOptionPane.WANTS_INPUT_PROPERTY) {
					uninstallComponents();
					installComponents();
					optionPane.validate();
				} else if (changeName == "componentOrientation") {
					ComponentOrientation o = (ComponentOrientation)e.getNewValue();
					JOptionPane op = (JOptionPane)e.getSource();
					if (o != (ComponentOrientation)e.getOldValue()) {
						op.applyComponentOrientation(o);
					}
				}
			}
		}
	}

	//Taken from javax.swing.plaf.basic.BasicOptionPaneUI
	public class ButtonActionListener implements ActionListener {
		protected int buttonIndex;

		public ButtonActionListener(int buttonIndex) {
			this.buttonIndex = buttonIndex;
		}

		public void actionPerformed(ActionEvent e) {
			if (optionPane != null) {
				optionPane.setValue(new Integer(buttonIndex));
			}
		}
	}

	private void setSystemLookAndFeel() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			//don't worry if we can't.
		}
	}

	//this is for the different dialogs to fill in.
	protected abstract void installComponents();
}
