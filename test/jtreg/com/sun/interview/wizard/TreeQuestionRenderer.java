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
package com.sun.interview.wizard;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.*;
import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.event.EventListenerList;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.plaf.metal.MetalLookAndFeel;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import com.sun.interview.Question;
import com.sun.interview.TreeQuestion;

class TreeQuestionRenderer
    implements QuestionRenderer
{
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        final TreeQuestion tq = (TreeQuestion) qq;
        final TQTree tree = new TQTree(tq.getModel(), listener);
        tree.setName("tree");
        tree.setToolTipText(i18n.getString("tree.tip"));
        tree.getAccessibleContext().setAccessibleName(tree.getName());
        tree.getAccessibleContext().setAccessibleDescription(tree.getToolTipText());
        JScrollPane sp = new JScrollPane(tree);

        tree.setSelection(tq.getValue());

        Runnable valueSaver = new Runnable() {
                public void run() {
                    tq.setValue(tree.getSelection());
                }
            };

        sp.putClientProperty(VALUE_SAVER, valueSaver);

        return sp;
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private class TQTree extends JTree
    {
        TQTree(TreeQuestion.Model model, ActionListener editedListener) {
            clientModel = model;

            listener = new TQListener(editedListener);
            addTreeSelectionListener(listener);
            //SB
            addTreeExpansionListener(listener);
            root = new TQNode(this, null, clientModel.getRoot());
            setModel(new TQModel(root));
            setCellRenderer(new TQRenderer());
            setExpandsSelectedPaths(false);
        }

        String[] getSelection() {
            Vector v = new Vector();
            getSelection(root, v);

            String[] paths = new String[v.size()];
            v.copyInto(paths);
            return paths;
        }

        private void getSelection(TQNode node, Vector v) {
            switch (node.selectState) {
            case NONE_SELECTED:
                break;

            case SOME_SELECTED:
                for (int i = 0; i < node.getChildCount(); i++)
                    getSelection(node.getChild(i), v);
                break;

            case ALL_SELECTED:
                v.add(getPath(node));
                break;
            }
        }

        void setSelection(String[] paths) {
            if (paths == null || paths.length == 0)
                root.setSelected(true);
            else {
                root.setSelected(false);
                if (paths != null) {
                    for (int i = 0; i < paths.length; i++) {
                        TQNode node = getNode(root, paths[i]);
                        if (node != null)
                            node.setSelected(true);
                    }
                }
            }
        }

        void addToSelection(TQNode node) {
            listener.setIgnoreEvents(true);
            TreePath p = getTreePath(node);
            getSelectionModel().addSelectionPath(p);
            listener.setIgnoreEvents(false);
        }

        void removeFromSelection(TQNode node) {
            listener.setIgnoreEvents(true);
            TreePath p = getTreePath(node);
            getSelectionModel().removeSelectionPath(p);
            listener.setIgnoreEvents(false);
        }

        private TQNode getNode(TQNode node, String path) {
            if (node == null)
                throw new NullPointerException();

            if (path.length() == 0)
                return node;

            String head;
            String tail;

            int sep = path.indexOf("/");
            if (sep == -1) {
                head = path;
                tail = null;
            }
            else {
                head = path.substring(0, sep);
                tail = path.substring(sep + 1);
            }

            for (int i = 0; i < node.getChildCount(); i++) {
                TQNode c = node.getChild(i);
                if (c.getName().equals(head))
                    return (tail == null ? c : getNode(c, tail));
            }

            return null;
        }

        String getPath(TQNode node) {
            return getPath(node, 0).toString();
        }

        private StringBuffer getPath(TQNode node, int length) {
            if (node.parent == null)
                return new StringBuffer(length);
            else {
                String nodeName = node.getName();
                StringBuffer sb = getPath(node.parent, 1 + nodeName.length() + length);
                if (sb.length() > 0)
                    sb.append("/");
                sb.append(nodeName);
                return sb;
            }
        }

        TreePath getTreePath(TQNode node) {
            if (node.parent == null)
                return new TreePath(node);
            else
                return getTreePath(node.parent).pathByAddingChild(node);
        }

        TreeQuestion.Model clientModel;
        TQListener listener;
        TQNode root;
    };

    private class TQNode {
        TQNode(TQTree tree, TQNode parent, Object clientNode) {
            if (tree == null || clientNode == null)
                throw new NullPointerException();

            this.tree = tree;
            this.parent = parent;
            this.clientNode = clientNode;
        }

        String getName() {
            return (tree.clientModel.getName(clientNode));
        }

        int getChildCount() {
            return (children == null ? tree.clientModel.getChildCount(clientNode) : children.length);
        }

        TQNode getChild(int index) {
            if (children == null)
                initChildren();
            return children[index];
        }

        boolean isLeaf() {
            return (tree.clientModel.isLeaf(clientNode));
        }

        int getIndexOfChild(TQNode child) {
            if (children == null)
                initChildren();
            for (int i = 0; i < children.length; i++) {
                if (child == children[i])
                    return i;
            }
            return -1;
        }

        void setSelected(boolean b) {
            if ( (b && (selectState == ALL_SELECTED))
                 || (!b && selectState == NONE_SELECTED))
                return;

            setSelectState(b ? ALL_SELECTED : NONE_SELECTED);

            if (children != null) {
                for (int i = 0; i < children.length; i++)
                    children[i].setSelected(b);
            }

            TQNode n = this;
            TQNode p = parent;
            while (p != null) {
                int state = p.children[0].selectState;
                for (int i = 1; i < p.children.length && state != SOME_SELECTED; i++) {
                    TQNode c = p.children[i];
                    if (state == ALL_SELECTED && c.selectState != ALL_SELECTED
                        || state == NONE_SELECTED && c.selectState != NONE_SELECTED)
                        state = SOME_SELECTED;
                }
                if (p.selectState == state)
                    break;
                p.setSelectState(state);
                n = p;
                p = n.parent;
            }
        }

        private void setSelectState(int newState) {
            if (selectState == newState)
                return;

            // if the old state was ALL_SELECTED (and the new state won't be)
            // then we remove the node from the selection
            if (selectState == ALL_SELECTED)
                tree.removeFromSelection(this);

            selectState = newState;
            ((TQModel) (tree.getModel())).fireTreeNodesChanged(tree, this);

            // if the new state is ALL_SELECTED, add it to the system selection
            if (selectState == ALL_SELECTED)
                tree.addToSelection(this);
        }

        private void initChildren() {
            children = new TQNode[getChildCount()];
            for (int i = 0; i < children.length; i++)
                children[i] = new TQNode(tree, this, tree.clientModel.getChild(clientNode, i));

            if (selectState == ALL_SELECTED) {
                for (int i = 0; i < children.length; i++)
                    children[i].setSelectState(ALL_SELECTED);
            }
        }

        TQTree tree;
        TQNode parent;
        TQNode[] children;
        int selectState;
        Object clientNode;
    };

    private static final int NONE_SELECTED = 0;
    private static final int SOME_SELECTED = 1;
    private static final int ALL_SELECTED = 2;

    private class TQModel implements TreeModel {
        TQModel(TQNode root) {
            this.root = root;
        }

        public String getName(Object node) {
            return ((TQNode) node).getName();
        }

        public Object getChild(Object parent, int index) {
            return ((TQNode) parent).getChild(index);
        }

        public int getChildCount(Object parent) {
            return ((TQNode) parent).getChildCount();
        }

        public int getIndexOfChild(Object parent, Object child) {
            return ((TQNode) parent).getIndexOfChild((TQNode) child);
        }

        public Object getRoot() {
            return root;
        }

        public boolean isLeaf(Object node) {
            return ((TQNode) node).isLeaf();
        }

        public void addTreeModelListener(TreeModelListener l) {
            listenerList.add(TreeModelListener.class, l);
        }

        public void removeTreeModelListener(TreeModelListener l) {
            listenerList.remove(TreeModelListener.class, l);
        }

        public void valueForPathChanged(TreePath path, Object newValue) {
            throw new UnsupportedOperationException();
        }

        void fireTreeNodesChanged(TQTree tree, TQNode node) {
            TreeModelEvent event = null;
            // Guaranteed to return a non-null array
            Object[] listeners = listenerList.getListenerList();
            // Process the listeners last to first, notifying
            // those that are interested in this event
            for (int i = listeners.length-2; i>=0; i-=2) {
                if (listeners[i] == TreeModelListener.class) {
                    // Lazily create the event:
                    if (event == null)
                        event = new TreeModelEvent(tree, tree.getTreePath(node));
                    ((TreeModelListener) listeners[i+1]).treeNodesChanged(event);
         }
     }
        }

        private TQNode root;
        private EventListenerList listenerList = new EventListenerList();
    };

    private class TQRenderer extends DefaultTreeCellRenderer {
        TQRenderer() {
            if (folderIcons == null) {
                folderIcons = new FolderIcon[3];
                folderIcons[NONE_SELECTED] = new FolderIcon(NONE_SELECTED);
                folderIcons[SOME_SELECTED] = new FolderIcon(SOME_SELECTED);
                folderIcons[ALL_SELECTED] = new FolderIcon(ALL_SELECTED);
            }
            if (leafIcons == null) {
                leafIcons = new LeafIcon[3];
                leafIcons[NONE_SELECTED] = new LeafIcon(NONE_SELECTED);
                leafIcons[ALL_SELECTED] = new LeafIcon(ALL_SELECTED);
            }
        }

        public Component getTreeCellRendererComponent(JTree tree,
                                                      Object value,
                                                      boolean sel,
                                                      boolean expanded,
                                                      boolean leaf,
                                                      int row,
                                                      boolean hasFocus) {
            TQModel model = (TQModel) (tree.getModel());
            TQNode node = (TQNode) value;
            value = model.getName(node);
            style = node.selectState;
            // should set the icons that will be used
            // should override sel(ection)
            return (super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus));

        }

        public Icon getOpenIcon() {
            return folderIcons[style];
        }

        public Icon getClosedIcon() {
            return folderIcons[style];
        }

        public Icon getLeafIcon() {
            return leafIcons[style];
        }

        private int style;
    };

    private static String stringOf(TreePath p) {
        StringBuffer sb = new StringBuffer();
        Object[] path = p.getPath();
        for (int i = 0; i < path.length; i++) {
            if (sb.length() > 0)
                sb.append("/");
            sb.append(((TQNode) path[i]).getName());
        }
        return sb.toString();
    }

    private class TQListener implements TreeExpansionListener, TreeSelectionListener
    {
        TQListener(ActionListener editedListener) {
            this.editedListener = editedListener;
        }

        public void treeCollapsed(TreeExpansionEvent e) {
            TreePath p = e.getPath();
            TQNode node = (TQNode) (p.getLastPathComponent());
            vector = new Vector();
            vector.add(p);
            TQTree tree = (TQTree)e.getSource();

            TreePath[] paths = tree.getSelectionPaths();
            if (paths != null) {
                for(int i = 0; i < paths.length; i++) {
                    if(p.isDescendant(paths[i]))
                        vector.add(paths[i]);
                }
            }
        }

        public void treeExpanded(TreeExpansionEvent e) {
            vector = null;
        }

        public void valueChanged(TreeSelectionEvent e) {

            if (ignoreEvents)
                return;

            TQTree tree = (TQTree) (e.getSource());

            TreePath[] paths = e.getPaths();
            //----------------
            //This code inserted to consume selection event when collapsed subtree had
            //some selected nodes.
            if(vector != null) {
                boolean contains = true;
                for(int i = 0; i < paths.length; i++) {
                   if(!vector.contains(paths[i])) {
                       contains = false;
                       break;
                   }
                }
                if(!contains) {         //selection event was fired not because of tree collapse
                    vector = null;
                }
                else {
                    Set added = new HashSet();
                    Set removed = new HashSet();
                    for(int i = 0; i < paths.length; i++) {
                        //TQNode node = (TQNode) (paths[i].getLastPathComponent());
                        if(!e.isAddedPath(paths[i])) {
                            //tree.addToSelection(node);
                            added.add(paths[i]);
                        }
                        else  {
                            //tree.removeFromSelection(node);
                            removed.add(paths[i]);
                        }
                    }
                    setIgnoreEvents(true);
                    tree.getSelectionModel().addSelectionPaths((TreePath[])added.toArray(new TreePath[0]));
                    tree.getSelectionModel().removeSelectionPaths((TreePath[])removed.toArray(new TreePath[0]));
                    setIgnoreEvents(false);
                    return;
                }
            }
//----------------------------------------------------------------
            for (int i = 0; i < paths.length; i++) {
                TreePath p = paths[i];
                TQNode node = (TQNode) (p.getLastPathComponent());
                if (e.isAddedPath(p)) {
                    node.setSelected(true);
                } else node.setSelected(false);
            }
            /*  This block is placed to force nodes
             selection if they were selected before*/
                TreePath p = e.getNewLeadSelectionPath();
                if (p != null) {
                    TQNode node = (TQNode) (p.getLastPathComponent());
                    node.setSelected(true);
                }
// ---------------------------------------------------------------
            ActionEvent ae = new ActionEvent(e.getSource(),
                                             ActionEvent.ACTION_PERFORMED,
                                             EDITED);
            editedListener.actionPerformed(ae);
        }

        void setIgnoreEvents(boolean b) {
            ignoreEvents = b;
        }

        private boolean ignoreEvents;
        private ActionListener editedListener;
        private Vector vector;
    }

////////-----------------------------------------------------------------------------------------------------------

    static class FolderIcon implements Icon {
        FolderIcon(int style) {
            this.style = style;
        }

        public int getIconWidth() {
            return width;
        }

        public int getIconHeight() {
            return height;
        }

        public void paintIcon(Component c, Graphics g, int x, int y) {
            if (image == null) {
                image = new BufferedImage(getIconWidth(), getIconHeight(),
                                          BufferedImage.TYPE_INT_ARGB);
                Graphics imageG = image.getGraphics();
                paintMe(c,imageG);
                imageG.dispose();

            }
            g.drawImage(image, x, y - 1, null); // shift icon up a bit
        }


        private void paintMe(Component c, Graphics g) {

            int right = width - 1;
            int bottom = height - 1;

            // Draw tab top
            g.setColor( MetalLookAndFeel.getPrimaryControlDarkShadow() );
            g.drawLine( right - 5, 3, right, 3 );
            g.drawLine( right - 6, 4, right, 4 );

            // Draw folder front
            g.setColor( MetalLookAndFeel.getPrimaryControl() );
            g.fillRect( 2, 7, 13, 8 );

            switch (style) {
            case NONE_SELECTED:
                g.setColor( Color.white );
                g.fillRect( 2, 7, 13, 8 );
                break;

            case SOME_SELECTED:
                g.setColor( Color.white );
                for (int i = 0; i < 8; i++)
                    g.drawLine(4 + i, 7 + i, 18, 7 + i);
                break;

            case ALL_SELECTED:
                break;
            }

            // Draw tab bottom
            g.setColor( MetalLookAndFeel.getPrimaryControlShadow() );
            g.drawLine( right - 6, 5, right - 1, 5 );

            // Draw outline
            g.setColor( MetalLookAndFeel.getPrimaryControlInfo() );
            g.drawLine( 0, 6, 0, bottom );            // left side
            g.drawLine( 1, 5, right - 7, 5 );         // first part of top
            g.drawLine( right - 6, 6, right - 1, 6 ); // second part of top
            g.drawLine( right, 5, right, bottom );    // right side
            g.drawLine( 0, bottom, right, bottom );   // bottom

            // Draw highlight
            g.setColor( MetalLookAndFeel.getPrimaryControlHighlight() );
            g.drawLine( 1, 6, 1, bottom - 1 );
            g.drawLine( 1, 6, right - 7, 6 );
            g.drawLine( right - 6, 7, right - 1, 7 );

        }

        private int style;
        private Image image;
        private static final int width = 16;
        private static final int height = 16;
    }

    private static class LeafIcon implements Icon {
        LeafIcon(int style) {
            this.style = style;
        }

        public int getIconWidth() {
            return width;
        }

        public int getIconHeight() {
            return height;
        }

        public void paintIcon(Component c, Graphics g, int x, int y) {
            if (image == null) {
                image = new BufferedImage(getIconWidth(), getIconHeight(),
                                          BufferedImage.TYPE_INT_ARGB);
                Graphics imageG = image.getGraphics();
                paintMe(c,imageG);
                imageG.dispose();

            }
            g.drawImage(image, x, y, null);

        }

        private void paintMe(Component c, Graphics g) {

            int right = width - 1;
            int bottom = height - 1;

            // Draw fill
            g.setColor( MetalLookAndFeel.getWindowBackground() );
            g.fillRect( 4, 2, 9, 12 );

            // Draw frame
            g.setColor( MetalLookAndFeel.getPrimaryControlInfo() );
            g.drawLine( 2, 0, 2, bottom );                 // left
            g.drawLine( 2, 0, right - 4, 0 );              // top
            g.drawLine( 2, bottom, right - 1, bottom );    // bottom
            g.drawLine( right - 1, 6, right - 1, bottom ); // right
            g.drawLine( right - 6, 2, right - 2, 6 );      // slant 1
            g.drawLine( right - 5, 1, right - 4, 1 );      // part of slant 2
            g.drawLine( right - 3, 2, right - 3, 3 );      // part of slant 2
            g.drawLine( right - 2, 4, right - 2, 5 );      // part of slant 2

            switch (style) {
            case NONE_SELECTED:
                // Draw highlight
                g.setColor( MetalLookAndFeel.getPrimaryControl() );
                g.drawLine( 3, 1, 3, bottom - 1 );                  // left
                g.drawLine( 3, 1, right - 6, 1 );                   // top
                g.drawLine( right - 2, 7, right - 2, bottom - 1 );  // right
                g.drawLine( right - 5, 2, right - 3, 4 );           // slant
                g.drawLine( 3, bottom - 1, right - 2, bottom - 1 ); // bottom
                break;

            case ALL_SELECTED:
                // Fill
                g.setColor( MetalLookAndFeel.getPrimaryControl() );
                g.drawLine( 4, 2, 9, 2);
                g.drawLine( 4, 3, 9, 3);
                g.drawLine( 4, 4, 10, 4);
                g.drawLine( 4, 5, 10, 5);
                g.drawLine( 4, 6, 11, 6);
                g.fillRect( 4, 6, 10, 9);
                break;
            }
        }

        private int style;
        private Image image;
        private static final int width = 16;
        private static final int height = 16;
    }

    private static Icon[] folderIcons;
    private static Icon[] leafIcons;
    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
}
