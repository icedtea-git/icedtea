/* CertWarningPane.java
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
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;

import net.sourceforge.jnlp.JNLPFile;
import net.sourceforge.jnlp.PluginBridge;
import net.sourceforge.jnlp.runtime.JNLPRuntime;
import net.sourceforge.jnlp.tools.KeyTool;

/**
 * Provides the look and feel for a SecurityWarningDialog. These dialogs are
 * used to warn the user when either signed code (with or without signing 
 * issues) is going to be run, or when service permission (file, clipboard,
 * printer, etc) is needed with unsigned code.
 *
 * @author <a href="mailto:jsumali@redhat.com">Joshua Sumali</a>
 */
public class CertWarningPane extends SecurityDialogUI {

	JCheckBox alwaysTrust;

	public CertWarningPane(JComponent x) {
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
			if (file instanceof PluginBridge)
				name = file.getTitle();
			else
				name = file.getInformation().getTitle();
		} catch (Exception e) {
		}

		try {
			//Really ugly way of getting the publisher...
			if (file instanceof PluginBridge) {
				Certificate c = ((SecurityWarningDialog)optionPane)
					.getJarSigner().getPublisher();
				if (c instanceof X509Certificate) {
					publisher = getCN(((X509Certificate)c)
						.getSubjectX500Principal().getName());
				}
			}
			else
				publisher = file.getInformation().getVendor();
		} catch (Exception e) {
		}

		try {
			if (file instanceof PluginBridge)
				from = file.getCodeBase().getHost();
			else
				from = file.getInformation().getHomepage().toString();
		} catch (Exception e) {
		}

		//Top label
		String topLabelText = "";
		String propertyName = "";
		switch (type) {
		case VERIFIED:
			topLabelText = R("SSigVerified");
			propertyName = "OptionPane.informationIcon";
			break;
		case UNVERIFIED:
			topLabelText = R("SSigUnverified");
			propertyName = "OptionPane.warningIcon";
			break;
		case SIGNING_ERROR:
			topLabelText = R("SSignatureError");
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

		alwaysTrust = new JCheckBox(
		"Always trust content from this publisher");
		alwaysTrust.setEnabled(true);

		JPanel infoPanel = new JPanel(new GridLayout(4,1));
		infoPanel.add(nameLabel);
		infoPanel.add(publisherLabel);
		infoPanel.add(fromLabel);
		infoPanel.add(alwaysTrust);
		infoPanel.setBorder(BorderFactory.createEmptyBorder(25,25,25,25));

		//run and cancel buttons
		JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		JButton run = new JButton("Run");
		JButton cancel = new JButton("Cancel");
		int buttonWidth = Math.max(run.getMinimumSize().width, 
			cancel.getMinimumSize().width);
		int buttonHeight = run.getMinimumSize().height;
		Dimension d = new Dimension(buttonWidth, buttonHeight);
		run.setPreferredSize(d);
		cancel.setPreferredSize(d);
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

		JLabel bottomLabel;
		JButton moreInfo = new JButton("More information...");
		moreInfo.addActionListener(new MoreInfoButtonListener());
		
		//TODO: This should check if the X500Issuer is in the cacerts file.
		if (((SecurityWarningDialog)optionPane).getJarSigner().getRootInCacerts())
			bottomLabel = new JLabel(htmlWrap(R("STrustedSource")));
		else
			bottomLabel = new JLabel(htmlWrap(R("SUntrustedSource")));

		JPanel bottomPanel = new JPanel();
		bottomPanel.setLayout(new BoxLayout(bottomPanel, BoxLayout.X_AXIS));
		bottomPanel.add(bottomLabel);
		bottomPanel.add(moreInfo);
		bottomPanel.setPreferredSize(new Dimension(500,100));
		bottomPanel.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
		main.add(bottomPanel);

		optionPane.add(main, BorderLayout.CENTER);
	}

	private static String R(String key) {
        return JNLPRuntime.getMessage(key);
    }

	protected String htmlWrap (String s) {
        return "<html>"+s+"</html>";
    }


    /**
     * Extracts the CN field from a Certificate principal string.
     */
    private String getCN(String principal) {
        int start = principal.indexOf("CN=");
        int end = principal.indexOf(",", start);

        if (end == -1) {
            end = principal.length();
        }

        if (start >= 0)
            return principal.substring(start+3, end);
        else
            return principal;
    }

	private class MoreInfoButtonListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			
			// TODO: Can we change this to just
			// optionPane.showMoreInfo(); ?
			SecurityWarningDialog.showMoreInfoDialog(
				((SecurityWarningDialog)optionPane).getJarSigner(), 
				optionPane);
		}
	}

	/**
	 * Updates the user's KeyStore of trusted Certificates.
	 */
	private class CheckBoxListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (alwaysTrust != null && alwaysTrust.isSelected()) {
				try {
					KeyTool kt = new KeyTool();
					Certificate c =
						((SecurityWarningDialog)optionPane).getJarSigner().getPublisher();
					kt.importCert(c);
				} catch (Exception ex) {
					//TODO: Let NetX show a dialog here notifying user 
					//about being unable to add cert to keystore
				}
			}
		}
	}

}
