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
package com.sun.javatest.exec;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.io.IOException;
import java.io.StringWriter;
import javax.swing.BorderFactory;
import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.ListSelectionModel;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.plaf.metal.MetalLookAndFeel;
import javax.swing.text.html.HTMLDocument;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.html.StyleSheet;

import com.sun.javatest.JavaTestError;
import com.sun.javatest.Status;
import com.sun.javatest.TestResult;
import com.sun.javatest.tool.IconFactory;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.StringArray;

/**
 * Show the output sections for a particular test result.
 * This panel will dynamically update data if the test is running.
 */

class TP_OutputSubpanel extends TP_Subpanel {

    String currentTOCEntry = null;

    TP_OutputSubpanel(UIFactory uif) {
        super(uif, "out");
        initGUI();
    }

    protected synchronized void updateSubpanel(TestResult newTest) {
        if (subpanelTest != null)
            subpanelTest.removeObserver(observer);

        super.updateSubpanel(newTest);
        //System.err.println("TP_OS: " + newTest);

        updateTOC();

        // if it is mutable, track updates
        if (subpanelTest.isMutable())  {
            subpanelTest.addObserver(observer);
        }
    }

    private void initGUI() {
        setLayout(new BorderLayout());

        tocEntries = new DefaultListModel();
        toc = uif.createList("test.out.toc", tocEntries);
        toc.setCellRenderer(new TOCRenderer());
        toc.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        toc.setPrototypeCellValue("12345678901234567890");
        toc.setVisibleRowCount(10);
        toc.addListSelectionListener(listener);

        JScrollPane scrollableTOC =
            uif.createScrollPane(toc,
                            JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                            JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

        main = new JPanel(new BorderLayout());
        titleField = uif.createOutputField("test.out.title");
        titleField.setBackground(MetalLookAndFeel.getPrimaryControlDarkShadow());
        titleField.setForeground(MetalLookAndFeel.getWindowBackground());
        main.add(titleField, BorderLayout.NORTH);

        body = new JPanel(new CardLayout()) {
            public Dimension getPreferredSize() {
                int dpi = uif.getDotsPerInch();
                return new Dimension(3 * dpi, 3 * dpi);
            }
        };

        textArea = uif.createTextArea("test.out.textbody");
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        body.add(uif.createScrollPane(textArea,
                                 JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                                 JScrollPane.HORIZONTAL_SCROLLBAR_NEVER),
                 "text");

        htmlArea = new JEditorPane();
        htmlArea.setName("out_summ");
        htmlArea.getAccessibleContext().setAccessibleName(uif.getI18NString("test.out.summ.name"));
        htmlArea.getAccessibleContext().setAccessibleDescription(uif.getI18NString("test.out.summ.name"));
        //htmlArea.setContentType("text/html");

        // create and set a vacuous subtype of HTMLDocument, simply in order
        // to have the right classloader associated with it, that can load
        // any related OBJECT tags
        htmlArea.setDocument(new HTMLDocument(getStyleSheet()) { });
        htmlArea.setEditable(false);
        htmlArea.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        htmlArea.addHyperlinkListener(listener);
        body.add(uif.createScrollPane(htmlArea,
                                 JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                                 JScrollPane.HORIZONTAL_SCROLLBAR_NEVER),
                 "html");

        main.add(body, BorderLayout.CENTER);

        JSplitPane sp =
            new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, scrollableTOC, main);
        sp.setBorder(BorderFactory.createLoweredBevelBorder());
        sp.setResizeWeight(0); // all excess space to right hand side
        add(sp);

        initIcons();
    }

    private void initIcons() {
        streamIcon = uif.createIcon("test.out.sect.stream");
    }

    private String getStatusKey(int i) {
        String s;
        switch (i) {
        case Status.PASSED:  return "passed";
        case Status.FAILED:  return "failed";
        case Status.ERROR:   return "error";
        case Status.NOT_RUN: return "notRun";
        default:             return "unknown";
        }
    }

    private String createNotRunSummary() {
        StringWriter sw = new StringWriter();
        try {
            HTMLWriter out = new HTMLWriter(sw, uif.getI18NResourceBundle());
            out.startTag(HTMLWriter.HTML);
            out.startTag(HTMLWriter.BODY);
            //out.writeStyleAttr(bodyStyle);
            out.writeI18N("test.out.smry.testNotRun");
            out.endTag(HTMLWriter.BODY);
            out.endTag(HTMLWriter.HTML);
            out.close();
        }
        catch (IOException e) {
            // should not happen, with StringWriter
        }
        return sw.toString();
    }

    private String createSummary() {
        StringWriter sw = new StringWriter();
        try {
            HTMLWriter out = new HTMLWriter(sw, uif.getI18NResourceBundle());
            out.startTag(HTMLWriter.HTML);
            out.startTag(HTMLWriter.BODY);
            //out.writeStyleAttr(bodyStyle);

            String[] scriptAndArgs;
            try {
                scriptAndArgs = StringArray.split(subpanelTest.getProperty(TestResult.SCRIPT));
            }
            catch (TestResult.Fault e) {
                scriptAndArgs = null;
            }

            String script = (scriptAndArgs == null || scriptAndArgs.length == 0
                             ? uif.getI18NString("test.out.smry.unknownScript")
                             : scriptAndArgs[0]);
            out.writeI18N("test.out.script");
            out.startTag(HTMLWriter.TABLE);
            out.writeAttr(HTMLWriter.BORDER, "0");
            //out.writeStyleAttr(tableStyle);
            out.startTag(HTMLWriter.TR);
            out.startTag(HTMLWriter.TD);
            out.startTag(HTMLWriter.CODE);
            out.write(script);
            out.endTag(HTMLWriter.CODE);
            out.endTag(HTMLWriter.TD);
            out.endTag(HTMLWriter.TR);
            out.endTag(HTMLWriter.TABLE);
            if (scriptAndArgs != null && scriptAndArgs.length > 1) {
                out.writeI18N("test.out.scriptArgs");
                out.startTag(HTMLWriter.TABLE);
                out.writeAttr(HTMLWriter.BORDER, "0");
                //out.writeStyleAttr(tableStyle);
                for (int i = 1; i < scriptAndArgs.length; i++) {
                    out.startTag(HTMLWriter.TR);
                    out.startTag(HTMLWriter.TD);
                    out.startTag(HTMLWriter.CODE);
                    out.write(scriptAndArgs[i]);
                    out.endTag(HTMLWriter.CODE);
                    out.endTag(HTMLWriter.TD);
                    out.endTag(HTMLWriter.TR);
                }
                out.endTag(HTMLWriter.TABLE);
            }

            if (subpanelTest.getSectionCount() > 0) {
                TestResult.Section s = subpanelTest.getSection(0);
                if (s.getTitle().equals(TestResult.MSG_SECTION_NAME)) {
                    out.writeI18N("test.out.smry.scriptLog.txt");
                    out.startTag(HTMLWriter.TABLE);
                    out.writeAttr(HTMLWriter.BORDER, "0");
                    //out.writeStyleAttr(tableStyle);
                    String[] names = s.getOutputNames();
                    for (int i = 0; i < names.length; i++) {
                        String name = names[i];
                        String text = s.getOutput(name);
                        out.startTag(HTMLWriter.TR);
                        out.startTag(HTMLWriter.TD);
                        out.writeLink("#" + name, name/*, linkStyle*/);
                        out.endTag(HTMLWriter.TD);
                        out.endTag(HTMLWriter.TR);
                    }
                    out.endTag(HTMLWriter.TABLE);
                }
            }

            // generate a table showing the various sections
            if (subpanelTest.getSectionCount() > 0) {
                out.startTag(HTMLWriter.H3);
                //out.writeStyleAttr(h3Style);
                out.writeI18N("test.out.smry.sections.head");
                out.endTag(HTMLWriter.H3);
                out.writeI18N("test.out.smry.sections.txt");
                out.startTag(HTMLWriter.TABLE);
                out.writeAttr(HTMLWriter.BORDER, "0");
                //out.writeStyleAttr(tableStyle);
                for (int i = 0; i < subpanelTest.getSectionCount(); i++) {
                    TestResult.Section s = subpanelTest.getSection(i);
                    if (s.getTitle().equals(TestResult.MSG_SECTION_NAME))
                        continue; // already done, above
                    out.startTag(HTMLWriter.TR);
                    out.startTag(HTMLWriter.TD);
                    out.startTag(HTMLWriter.OBJECT);
                    out.writeAttr(HTMLWriter.CLASSID, "com.sun.javatest.tool.IconLabel");
                    out.writeParam("type", "testSection");
                    out.writeParam("state", getStatusKey(s.getStatus().getType()));
                    out.endTag(HTMLWriter.OBJECT);
                    out.writeLink(String.valueOf(i), s.getTitle()/*, linkStyle*/);
                    out.endTag(HTMLWriter.TD);
                    out.endTag(HTMLWriter.TR);
                }
                out.endTag(HTMLWriter.TABLE);

                out.startTag(HTMLWriter.H3);
                //out.writeStyleAttr(h3Style);
                out.writeI18N("test.out.outcome.head");
                out.endTag(HTMLWriter.H3);
                out.writeI18N("test.out.testResultForOutput.txt");
            }
            else {
                out.startTag(HTMLWriter.H3);
                //out.writeStyleAttr(h3Style);
                out.writeI18N("test.out.outcome.head");
                out.endTag(HTMLWriter.H3);
                out.writeI18N("test.out.testResultNoOutput.txt");
            }

            Status s = subpanelTest.getStatus();
            out.startTag(HTMLWriter.TABLE);
            out.writeAttr(HTMLWriter.BORDER, "0");
            //out.writeStyleAttr(tableStyle);
            out.startTag(HTMLWriter.TR);
            out.startTag(HTMLWriter.TD);
            out.startTag(HTMLWriter.OBJECT);
            out.writeAttr(HTMLWriter.CLASSID, "com.sun.javatest.tool.IconLabel");
            out.writeParam("type", "test");
            out.writeParam("state", getStatusKey(s.getType()));
            out.endTag(HTMLWriter.OBJECT);
            out.endTag(HTMLWriter.TD);
            out.startTag(HTMLWriter.TD);
            out.write(s.toString());
            out.endTag(HTMLWriter.TD);
            out.endTag(HTMLWriter.TR);
            out.endTag(HTMLWriter.TABLE);

            out.endTag(HTMLWriter.BODY);
            out.endTag(HTMLWriter.HTML);
            out.close();
        }
        catch (IOException e) {
            // should not happen, writing to StringWriter
        }
        catch (TestResult.ReloadFault e) {
            throw new JavaTestError("Error loading result file for " +
                                    subpanelTest.getTestName());
        }

        return sw.toString();
    }

    private String createSectionSummary(TestResult.Section section) {
        StringWriter sw = new StringWriter();
        try {
            HTMLWriter out = new HTMLWriter(sw, uif.getI18NResourceBundle());
            out.startTag(HTMLWriter.HTML);
            out.startTag(HTMLWriter.BODY);
            //out.writeStyleAttr(bodyStyle);

            // generate a table showing the size of the various output streams
            out.startTag(HTMLWriter.H3);
            out.writeStyleAttr("margin-top: 0");
            out.writeI18N("test.out.outputSummary.head");
            out.endTag(HTMLWriter.H3);
            out.writeI18N("test.out.outputSummary.txt");
            out.startTag(HTMLWriter.TABLE);
            out.writeAttr(HTMLWriter.BORDER, "0");
            //out.writeStyleAttr(tableStyle);
            out.startTag(HTMLWriter.TR);
            out.startTag(HTMLWriter.TH);
            out.writeAttr(HTMLWriter.ALIGN, HTMLWriter.LEFT);
            out.writeI18N("test.out.outputName.txt");
            out.endTag(HTMLWriter.TH);
            out.startTag(HTMLWriter.TH);
            out.writeAttr(HTMLWriter.ALIGN, HTMLWriter.LEFT);
            out.writeStyleAttr("margin-left:10");
            out.writeI18N("test.out.outputSize.txt");
            out.endTag(HTMLWriter.TH);
            out.endTag(HTMLWriter.TR);
            String[] names = section.getOutputNames();
            for (int i = 0; i < names.length; i++) {
                String name = names[i];
                String text = section.getOutput(name);
                out.startTag(HTMLWriter.TR);
                out.startTag(HTMLWriter.TD);
                if (text.length() == 0)
                    out.write(name);
                else
                    out.writeLink("#" + name, name/*, linkStyle*/);
                out.endTag(HTMLWriter.TD);
                out.startTag(HTMLWriter.TD);
                out.writeStyleAttr("margin-left:10");
                if (text.length() == 0)
                    out.writeI18N("test.out.empty.txt");
                else
                    out.write(String.valueOf(text.length()));
                out.endTag(HTMLWriter.TD);
                out.endTag(HTMLWriter.TR);
            }
            out.endTag(HTMLWriter.TABLE);

            // if there is a status, show it
            Status s = section.getStatus();
            if (s != null) {
                out.startTag(HTMLWriter.H3);
                //out.writeStyleAttr(h3Style);
                out.writeI18N("test.out.outcome.head");
                out.endTag(HTMLWriter.H3);
                out.writeI18N("test.out.sectionResult.txt");
                out.startTag(HTMLWriter.P);
                out.writeStyleAttr("margin-left:30; margin-top:0; font-size: 12pt");
                out.startTag(HTMLWriter.OBJECT);
                out.writeAttr(HTMLWriter.CLASSID, "com.sun.javatest.tool.IconLabel");
                out.writeParam("type", "testSection");
                out.writeParam("state", getStatusKey(s.getType()));
                out.endTag(HTMLWriter.OBJECT);
                out.write(s.toString());
                out.endTag(HTMLWriter.P);
            }

            out.endTag(HTMLWriter.BODY);
            out.endTag(HTMLWriter.HTML);
            out.close();
        }
        catch (IOException e) {
            // should not happen with StringWriter
        }
        return sw.toString();
    }

    private String createStatusSummary() {
        StringWriter sw = new StringWriter();
        try {
            HTMLWriter out = new HTMLWriter(sw, uif.getI18NResourceBundle());
            out.startTag(HTMLWriter.HTML);
            out.startTag(HTMLWriter.BODY);
            //out.writeStyleAttr(bodyStyle);

            if (subpanelTest.getSectionCount() > 0)
                out.writeI18N("test.out.testResultForOutput.txt");
            else
                out.writeI18N("test.out.testResultNoOutput.txt");

            Status s = subpanelTest.getStatus();
            out.startTag(HTMLWriter.TABLE);
            out.writeAttr(HTMLWriter.BORDER, "0");
            //out.writeStyleAttr(tableStyle);
            out.startTag(HTMLWriter.TR);
            out.startTag(HTMLWriter.TD);
            out.startTag(HTMLWriter.OBJECT);
            out.writeAttr(HTMLWriter.CLASSID, "com.sun.javatest.tool.IconLabel");
            out.writeParam("type", "test");
            out.writeParam("state", getStatusKey(s.getType()));
            out.endTag(HTMLWriter.OBJECT);
            out.endTag(HTMLWriter.TD);
            out.startTag(HTMLWriter.TD);
            out.write(s.toString());
            out.endTag(HTMLWriter.TD);
            out.endTag(HTMLWriter.TR);
            out.endTag(HTMLWriter.TABLE);
            out.endTag(HTMLWriter.BODY);
            out.endTag(HTMLWriter.HTML);
            out.close();
        }
        catch (IOException e) {
            // should not happen, for StringWriter
        }
        return sw.toString();
    }

    void updateTOC() {
        try {
            TOCEntry newSelectedEntry = null;
            tocEntries.setSize(0);
            for (int i = 0; i < subpanelTest.getSectionCount(); i++) {
                TestResult.Section s = subpanelTest.getSection(i);
                TOCEntry e = new TOCEntry(s);
                if (e.isScriptMessagesSection() && (currentTOCEntry == null) ||
                    (e.getID().equals(currentTOCEntry)))
                    newSelectedEntry = e;
                tocEntries.addElement(e);
                String[] names = s.getOutputNames();
                for (int j = 0; j < names.length; j++) {
                    e = new TOCEntry(s, names[j]);
                    if (e.getID().equals(currentTOCEntry)) {
                        newSelectedEntry = e;
                    }
                    tocEntries.addElement(e);
                }
            }

            TOCEntry e = new TOCEntry(); // for final status
            if (newSelectedEntry == null)
                newSelectedEntry = e;
            tocEntries.addElement(e);

            currentTOCEntry = newSelectedEntry.getID();
            toc.setSelectedValue(newSelectedEntry, true);
        }
        catch (TestResult.ReloadFault e) {
            throw new JavaTestError("Error loading result file for " +
                                    subpanelTest.getTestName());
        }
    }

    private void updateTOCLater() {
        if (EventQueue.isDispatchThread())
            updateTOC();
        else {
            EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        updateTOC(); // will also update current entry
                    }
                });
        }
    }

    private void showHTML(String s) {
        // high cost method call
        htmlArea.setContentType("text/html");

        // try resetting doc to an empty doc before setting new text

        // create and set a vacuous subtype of HTMLDocument, simply in order
        // to have the right classloader associated with it, that can load
        // any related OBJECT tags
        HTMLDocument doc = new HTMLDocument(getStyleSheet()) { };
        htmlArea.setDocument(doc);
        htmlArea.setText(s);

        /*
        StyleSheet styles = doc.getStyleSheet();
        Enumeration rules = styles.getStyleNames();
        while (rules.hasMoreElements()) {
            String name = (String) rules.nextElement();
            System.out.println(styles.getStyle(name));
        }
        */

        ((CardLayout)(body.getLayout())).show(body, "html");
    }

    private void showText(String s) {
        if (s.length() == 0) {
            textArea.setText(uif.getI18NString("test.out.empty.txt"));
            textArea.setEnabled(false);
        }
        else {
            textArea.setText(s);
            textArea.setEnabled(true);
        }

        ((CardLayout)(body.getLayout())).show(body, "text");
    }

    private StyleSheet getStyleSheet() {
        if (htmlEditorKit == null)
            htmlEditorKit = new HTMLEditorKit();

        if (styleSheet == null) {
            styleSheet = new StyleSheet();
            styleSheet.addStyleSheet(htmlEditorKit.getStyleSheet());
            styleSheet.addRule("body  { font-family: SansSerif; font-size: 12pt }");
            styleSheet.addRule("h3    { margin-top:15 }");
            styleSheet.addRule("table { margin-left:30; margin-top:0 }");
        }
        return styleSheet;
    }

    private Icon streamIcon;
    private JList toc;
    private JTextField titleField;
    private JPanel body;
    private JPanel main;
    private JTextArea textArea;

    private JEditorPane htmlArea;

    private TestResult currTest;
    private DefaultListModel tocEntries;
    private Listener listener = new Listener();
    private TRObserver observer = new TRObserver();

    private StyleSheet styleSheet;
    private HTMLEditorKit htmlEditorKit;

    //------------------------------------------------------------------------------------

    private class Listener implements HyperlinkListener, ListSelectionListener
    {
        public void hyperlinkUpdate(HyperlinkEvent e) {
            if (e.getEventType().equals(HyperlinkEvent.EventType.ACTIVATED)) {
                String desc = e.getDescription();
                if (desc.startsWith("#")) {
                    String outputName = desc.substring(1);
                    // search for the output entry for the currently selected section
                    int index = toc.getSelectedIndex();
                    if (index != -1) {
                        for (int i = index + 1; i < tocEntries.size(); i++) {
                            TOCEntry entry = (TOCEntry) (tocEntries.get(i));
                            String entryOutputName = entry.getOutputName();
                            if (entryOutputName == null)
                                // name not found, reached next section entry
                                break;
                            else if (entryOutputName.equals(outputName)) {
                                // found match, select this entry
                                toc.setSelectedIndex(i);
                                return;
                            }
                        }
                    }
                }
                else {
                    try {
                        int sectIndex = Integer.parseInt(desc);
                        TestResult.Section s = subpanelTest.getSection(sectIndex);
                        for (int i = 0; i < tocEntries.size(); i++) {
                            TOCEntry entry = (TOCEntry) (tocEntries.get(i));
                            if (entry.getSection() == s) {
                                // found match, select this entry
                                toc.setSelectedIndex(i);
                                return;
                            }
                        }

                    }
                    catch (TestResult.ReloadFault f) {
                        throw new JavaTestError("Error loading result file for " +
                                                subpanelTest.getTestName());
                    }
                }
            }
        }

        public void valueChanged(ListSelectionEvent e) {
            JList l = (JList) (e.getSource());
            TOCEntry entry = (TOCEntry) (l.getSelectedValue());
            if (entry == null)
                return;
            titleField.setText(entry.getTitle());
            currentTOCEntry = entry.getID();
            String outputName = entry.getOutputName();
            if (entry.section == null) {
                if (subpanelTest.getStatus().getType() == Status.NOT_RUN)
                    showHTML(createNotRunSummary());
                else
                    showHTML(createStatusSummary());
            }
            else if (outputName != null)
                showText(entry.getSection().getOutput(outputName));
            else if (entry.isScriptMessagesSection())
                showHTML(createSummary());
            else
                showHTML(createSectionSummary(entry.getSection()));
        }
    }

    //------------------------------------------------------------------------------------

    private class TRObserver
        implements TestResult.Observer
    {
        public void completed(TestResult tr) {
            //System.err.println("TPOS_TRO: completed: " + tr.getWorkRelativePath());
            updateTOCLater();
            tr.removeObserver(this);
        }

        public void createdSection(TestResult tr, TestResult.Section section) {
            //System.err.println("TPOS_TRO: created section[" + section.getTitle() + "]: " + tr.getWorkRelativePath());
        }

        public void completedSection(TestResult tr, TestResult.Section section) {
            //System.err.println("TPOS_TRO: completed section[" + section.getTitle() + "]: " + tr.getWorkRelativePath());
            updateTOCLater();
        }

        public void createdOutput(TestResult tr, TestResult.Section section,
                                  String outputName) {
            //System.err.println("TPOS_TRO: created output[" + section.getTitle() + "/" + outputName + "]: " + tr.getWorkRelativePath());
        }

        public void completedOutput(TestResult tr, TestResult.Section section,
                                    String outputName) {
            //System.err.println("TPOS_TRO: completed output[" + section.getTitle() + "/" + outputName + "]: " + tr.getWorkRelativePath());
        }

        public void updatedOutput(TestResult tr, TestResult.Section section,
                                  String outputName,
                                  int start, int end, String text) {
            //System.err.println("TPOS_TRO: written output[" + section.getTitle() + "/" + outputName + "]: " + tr.getWorkRelativePath());
        }

        public void updatedProperty(TestResult tr, String name, String value) {
            // ignore
        }
    }

    //------------------------------------------------------------------------------------

    private class TOCEntry {
        // create an entry that will show the test result status
        TOCEntry() {
            section = null;
            outputName = null;
        }

        // create an entry for a section summary
        TOCEntry(TestResult.Section s) {
            section = s;
            outputName = null;
        }

        // create an entry for a block of section output
        TOCEntry(TestResult.Section s, String n) {
            section = s;
            outputName = n;
        }

        boolean isScriptMessagesSection() {
            return (section != null && section.getTitle().equals(TestResult.MSG_SECTION_NAME));
        }

        TestResult.Section getSection() {
            return section;
        }

        String getOutputName() {
            return outputName;
        }

        String getTitle() {
            if (section == null) {
                if (subpanelTest.getStatus().getType() == Status.NOT_RUN)
                    return uif.getI18NString("test.out.notRunTitle");
                else
                    return uif.getI18NString("test.out.statusTitle");
            }
            else if (isScriptMessagesSection()) {
                if (outputName == null)
                    return uif.getI18NString("test.out.summary");
                else
                    return uif.getI18NString("test.out.scriptMessages");
            }
            else {
                if (outputName == null)
                    return uif.getI18NString("test.out.sectionTitle", section.getTitle());
                else
                    return uif.getI18NString("test.out.streamTitle",
                                             new Object[] { section.getTitle(), outputName });
            }
        }

        String getText() {
            if (section == null){
                if (subpanelTest.getStatus().getType() == Status.NOT_RUN)
                    return uif.getI18NString("test.out.notRunTitle");
                else
                    return uif.getI18NString("test.out.statusTitle");
            }
            else if (isScriptMessagesSection()) {
                if (outputName == null)
                    return uif.getI18NString("test.out.summary");
                else
                    return uif.getI18NString("test.out.scriptMessages");
            }
            else {
                if (outputName == null)
                    return section.getTitle();
                else
                    return outputName;
            }
        }

        Icon getIcon() {
            if (section == null)
                return IconFactory.getTestIcon(subpanelTest.getStatus().getType(), false, true);
            else if (outputName != null)
                return streamIcon;
            else {
                Status s = section.getStatus();
                //return (s == null ? null : sectIcons[s.getType()]);
                return (s == null ? null : IconFactory.getTestSectionIcon(s.getType()));
            }
        }

        String getID() {
            String s = "";
            if (section != null ) {
                s = section.getTitle() + ":" +
                        (outputName == null ? "" : outputName);
            }
            return s;
        }

        private TestResult.Section section;
        private String outputName;  // null for section entry
    }

    private class TOCRenderer extends DefaultListCellRenderer {
        public Component getListCellRendererComponent(JList list,
                                                      Object value,
                                                      int index,
                                                      boolean isSelected,
                                                      boolean cellHasFocus) {
            JLabel l = (JLabel) super.getListCellRendererComponent(list, null, index,
                                                                   isSelected, cellHasFocus);
            if (value instanceof TOCEntry) {
                TOCEntry e = (TOCEntry) value;
                l.setText(e.getText());
                l.setIcon(e.getIcon());
            }
            else
                l.setText(value.toString());
            return l;
        }
    }


}
