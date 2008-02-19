/* AccessWarningPane.java
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

package netx.jnlp.security;

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
import java.util.List;
import java.security.cert.Certificate;
import java.security.cert.CertPath;
import sun.swing.DefaultLookup;
import netx.jnlp.runtime.JNLPRuntime;
import netx.jnlp.JNLPFile;
import netx.jnlp.tools.KeyTool;

/**
 * Provides the look and feel for a SecurityWarningDialog. These dialogs are
 * used to warn the user when either signed code (with or without signing 
 * issues) is going to be run, or when service permission (file, clipboard,
 * printer, etc) is needed with unsigned code.
 *
 * @author <a href="mailto:jsumali@redhat.com">Joshua Sumali</a>
 */
public class AccessWarningPane extends SecurityDialogUI {

	JCheckBox alwaysAllow;

	public AccessWarningPane(JComponent x) {
		super(x);
	}

	/**
	 * Creates the actual GUI components, and adds it to <code>optionPane</code>
	 */
	protected void installComponents() {
		SecurityWarningDialog.AccessType type =
		    ((SecurityWarningDialog)optionPane).getType();
		JNLPFile file =
		    ((SecurityWarningDialog)optionPane).getFile();

		String name = "";
		String publisher = "";
		String from = "";

		//We don't worry about exceptions when trying to fill in
		//these strings -- we just want to fill in as many as possible.
		try {
			name = file.getInformation().getTitle();
		} catch (Exception e) {
		}

		try {
			publisher = file.getInformation().getVendor();
		} catch (Exception e) {
		}

		try {
			from = file.getInformation().getHomepage().toString();
		} catch (Exception e) {
		}

		//Top label
		String topLabelText = "";
		String propertyName = "";
		switch (type) {
			case READ_FILE:
				topLabelText = R("SFileReadAccess");
				propertyName = "OptionPane.warningIcon";
				break;
			case WRITE_FILE:
				topLabelText = R("SFileWriteAccess");
				propertyName = "OptionPane.warningIcon";
				break;
			case CLIPBOARD_READ:
				topLabelText = R("SClipboardReadAccess");
				propertyName = "OptionPane.warningIcon";
				break;
			case CLIPBOARD_WRITE:
				topLabelText = R("SClipboardWriteAccess");
				propertyName = "OptionPane.warningIcon";
				break;
			case PRINTER:
				topLabelText = R("SPrinterAccess");
				propertyName = "OptionPane.warningIcon";
				break;
		}
		
		//TODO: Get system icons and add them to our dialogs.
		//Icon icon = (Icon)DefaultLookup.get(optionPane,this,propertyName);
		JLabel topLabel = new JLabel(htmlWrap(topLabelText));
		topLabel.setFont(new Font(topLabel.getFont().toString(), 
			Font.BOLD, 12));
		JPanel topPanel = new JPanel(new BorderLayout());
		topPanel.setBackground(Color.WHITE);
		topPanel.add(topLabel, BorderLayout.CENTER);
		topPanel.setPreferredSize(new Dimension(400,60));
		topPanel.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));

		//application info
		JLabel nameLabel = new JLabel("Name:   " + name);
		nameLabel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
		JLabel publisherLabel = new JLabel("Publisher: " + publisher);
		publisherLabel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
		JLabel fromLabel = new JLabel("From:   " + from);
		fromLabel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));

		alwaysAllow = new JCheckBox("Always allow this action");
		alwaysAllow.setEnabled(false);

		JPanel infoPanel = new JPanel(new GridLayout(4,1));
		infoPanel.add(nameLabel);
		infoPanel.add(publisherLabel);
		infoPanel.add(fromLabel);
		infoPanel.add(alwaysAllow);
		infoPanel.setBorder(BorderFactory.createEmptyBorder(25,25,25,25));

		//run and cancel buttons
		JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		
		JButton run = new JButton("Allow");
		JButton cancel = new JButton("Cancel");
		run.addActionListener(createButtonActionListener(0));
		run.addActionListener(new CheckBoxListener());
		cancel.addActionListener(createButtonActionListener(1));
		initialFocusComponent = cancel;
		buttonPanel.add(run);
		buttonPanel.add(cancel);
		buttonPanel.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));

		//all of the above
		JPanel main = new JPanel();
		main.setLayout(new BoxLayout(main, BoxLayout.Y_AXIS));
		main.add(topPanel);
		main.add(infoPanel);
		main.add(buttonPanel);

		optionPane.add(main, BorderLayout.CENTER);
	}

	private static String R(String key) {
        return JNLPRuntime.getMessage(key);
    }

	protected String htmlWrap (String s) {
        return "<html>"+s+"</html>";
    }

	private class CheckBoxListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (alwaysAllow != null && alwaysAllow.isSelected()) {
				// TODO: somehow tell the ApplicationInstance
				// to stop asking for permission
			}
		}
	}

}
