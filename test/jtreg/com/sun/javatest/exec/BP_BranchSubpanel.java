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

import java.awt.Color;
import javax.swing.JPanel;

import com.sun.javatest.TestResultTable;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * Base class for the individual displays of the BranchPanel.
 */
abstract class BP_BranchSubpanel extends JPanel {
    BP_BranchSubpanel(String name, UIFactory uif, BP_Model model, TestTreeModel ttm,
                      String uiKey) {
        setName(name);
        setBackground(Color.white);
        uif.setAccessibleInfo(this, uiKey);

        this.uif = uif;
        this.model = model;
        this.ttm = ttm;
    }

    boolean isUpdateRequired(TestResultTable.TreeNode currNode) {
        return (subpanelNode != currNode);
    }


    protected void updateSubpanel(TestResultTable.TreeNode currNode) {
        subpanelNode = currNode;
    }

    /**
     * The test filters have either completely changed or they have
     * changed state.  In either case, subpanels should update
     * anything which depends on the filters.
     */
    protected void invalidateFilters() {
        filtersInvalidated = true;
    }

    protected void showMessage(String msg) {
        // this not on the event thread yet, it gets switched in
        // BranchModel
        lastMsg = msg;

        if (isVisible()) {
            model.showMessage(lastMsg);
        }
    }

    protected void showMesasge(I18NResourceBundle i18n, String key) {
        showMessage(i18n.getString(key));
    }

    // this method is to allow the GUI to restore the message if the
    // active panel is changed
    protected String getLastMessage() {
        return lastMsg;
    }

    protected TestResultTable.TreeNode subpanelNode;
    protected UIFactory uif;
    protected String lastMsg;
    protected BP_Model model;
    protected TestTreeModel ttm;
    protected boolean filtersInvalidated;
}
