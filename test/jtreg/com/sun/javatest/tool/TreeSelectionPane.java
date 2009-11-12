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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.util.*;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
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

import com.sun.javatest.util.I18NResourceBundle;

/**
 * A component to allow selection of nodes in a tree.
 */
public class TreeSelectionPane extends JComponent implements Accessible
{
    /**
     * The model for the tree whose nodes can be selected in a TreeSelectionPane.
     */
    public static interface Model
    {
        /**
         * Get the root node of the tree.
         * @return the root node of the tree
         */
        Object getRoot();

        /**
         * Get the number of children for a node.
         * @param node the node for which the number of children is required
         * @return the number of children of the specified node
         */
        int getChildCount(Object node);

        /**
         * Get a specific child of a node.
         * @param node the node for which the child is required
         * @param index the index of the desired child; this should be
         * in the range [0..getChildCount())
         * @return the specified child node
         */
        Object getChild(Object node, int index);

        /**
         * Get the name of a node.
         * The name should identify the child within the set of its parent's children.
         * @param node the node for which the name is required
         * @return the name of the node
         */
        String getName(Object node);

        /**
         * Get the path of a node.
         * The path is a series of names, separated by '/', which identify
         * successive children, initially relative to the root node.
         * @param node the node for which the path is required
         * @return the path of the node
         */
        String getPath(Object node);

        /**
         * Check whether or not a node is a leaf node.
         * This is simply a semantic distinction for a node, that will be
         * used to determine how the node should be displayed; it is independent
         * of whether a node has any children or not.
         * @param node the node to be checked
         * @return true if the node is a leaf node, and false otherwise
         */
        boolean isLeaf(Object node);
    };

    /**
     * Create a TreeSelectionPane, using a specified tree model.
     * @param model the model for the tree from which nodes may be selected
     */
    TreeSelectionPane(Model model) {
        uif = new UIFactory(getClass());

        setLayout(new BorderLayout());
        tree = new TQTree(model);
        tree.setName("tsp.tree");
        AccessibleContext ac = tree.getAccessibleContext();
        ac.setAccessibleName(i18n.getString("tsp.tree.name"));
        ac.setAccessibleDescription(i18n.getString("tsp.tree.desc"));

        JScrollPane sp= uif.createScrollPane(tree);
        add(sp);
    }

    /**
     * Get the accessible context for this pane.
     * @return the accessible context for this pane
     */
    public AccessibleContext getAccessibleContext() {
        if (accessibleContext == null)
            accessibleContext = new AccessibleJComponent() { };
        return accessibleContext;
    }

    /**
     * Get the current selection, represented as a set of paths to the
     * selected nodes.
     * @return the current selection, represented as a set of paths to the
     * selected nodes
     * @see #setSelection
     */
    public String[] getSelection() {
        return tree.getSelection();
    }

    /**
     * Set the current selection, by means of a set of paths to the
     * nodes to be selected.
     * @param paths a set of paths to the nodes to be selected
     * @see #getSelection
     * @see #clear
     */
    public void setSelection(String[] paths) {
        tree.setSelection(paths);
    }

    /**
     * Check if the selection is empty.
     * @return true if the selection is empty
     */
    public boolean isSelectionEmpty() {
        return tree.isSelectionEmpty();
    }

    /**
     * Clear the current selection.
     * @see #getSelection
     * @see #setSelection
     */
    public void clear() {
        tree.setSelection(null);
    }

    public void setEnabled(boolean b) {
        super.setEnabled(b);
        // propogate enabled-ness onto tree
        tree.setEnabled(b);
    }

    private AccessibleContext accessibleContext;
    private TQTree tree;
    private static I18NResourceBundle i18n =
        I18NResourceBundle.getBundleForClass(TreeSelectionPane.class);


    private class TQTree extends JTree
    {
        TQTree(Model model) {
            clientModel = model;

            listener = new TQListener();
            addTreeSelectionListener(listener);
            addTreeExpansionListener(listener);
            root = new TQNode(this, null, clientModel.getRoot());
            setModel(new TQModel(root));
            setCellRenderer(new TQRenderer());
            setExpandsSelectedPaths(false);
        }

        public Dimension getPreferredScrollableViewportSize() {
            return new Dimension(200, 100);
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
            Set all = TQNode.getSubNodes(root);
            all.add(root);
            TreePath[] tps = TQNode.getTreePaths(all);
            Iterator e = all.iterator();
            if (paths == null || paths.length == 0) {
                //root.setSelected(true);
                while (e.hasNext()) {
                    TQNode node = (TQNode)e.next();
                    node.selectState = ALL_SELECTED;
                }
                getSelectionModel().addSelectionPaths(tps);
            }
            else {
                //root.setSelected(false);
              while (e.hasNext()) {
                    TQNode node = (TQNode)e.next();
                    node.selectState = NONE_SELECTED;
                }
              getSelectionModel().removeSelectionPaths(tps);

                Set nodes = new HashSet();
                for (int i = 0; i < paths.length; i++) {
                    TQNode node = getNode(root, paths[i]);
                    if (node != null && node.selectState != ALL_SELECTED)
                        nodes.add(node);
                    }

                listener.setIgnoreEvents(true);

                // process children
                Set subNodes = TQNode.getSubNodes(nodes);
                TreePath[] kids = TQNode.getTreePaths(subNodes);
                if (subNodes != null) {
                    e = subNodes.iterator();
                    while (e.hasNext())
                        ((TQNode)(e.next())).selectState = ALL_SELECTED;
                }

                // process nodes
                TreePath[] tpaths = TQNode.getTreePaths(nodes);
                e = nodes.iterator();
                while (e.hasNext())
                    ((TQNode)(e.next())).selectState = ALL_SELECTED;

                // process parents
                Set upNodes = TQNode.getUpNodes(nodes);
                TreePath[] adults = TQNode.getTreePaths(upNodes);
                if (upNodes != null) {
                    e = upNodes.iterator();
                    while (e.hasNext()) {
                        TQNode up = (TQNode)e.next();
                        up.selectState = ALL_SELECTED;
                        for (int i = 0; i < up.children.length; i++) {
                            if (up.children[i].selectState == NONE_SELECTED || up.children[i].selectState == SOME_SELECTED) {
                                up.selectState = SOME_SELECTED;
                                break;
                            }
                        }
                    }
                }

                getSelectionModel().addSelectionPaths(kids);
                getSelectionModel().addSelectionPaths(tpaths);
                getSelectionModel().addSelectionPaths(adults);
                listener.setIgnoreEvents(false);
            }
        }

        void addToSelection(TQNode node) {
            listener.setIgnoreEvents(true);
            TreePath p = TQNode.getTreePath(node);
            getSelectionModel().addSelectionPath(p);
            listener.setIgnoreEvents(false);
        }

        void removeFromSelection(TQNode node) {
            listener.setIgnoreEvents(true);
            TreePath p = TQNode.getTreePath(node);
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

        Model clientModel;
        TQListener listener;
        TQNode root;
    };

    private static class TQNode implements Comparable {
        TQNode(TQTree tree, TQNode parent, Object clientNode) {
            if (tree == null || clientNode == null)
                throw new NullPointerException();

            this.tree = tree;
            this.parent = parent;
            this.clientNode = clientNode;
        }

        /**
         * Returns difference in levels bitween two TQNodes
         * if it doesnt equal 0 else return difference in hash codes.
         */
        public int compareTo(Object o) {
            if (o instanceof TQNode) {
                TQNode other = (TQNode) o;
                int levelDiff  = other.getLevel() - getLevel();
                int hashDiff = other.hashCode() - hashCode();
                return levelDiff == 0 ? hashDiff : levelDiff;
            } else
                return -1;
        }

        /**
         * Evaluates level of current node - number of nodes
         * from root till this (including both).
         *
         * @return depth of the current node.
         */
        public int getLevel() {
            int code = 1;
            TQNode grand = parent;
            while (grand != null) {
                grand = grand.parent;
                code++;
            }
            return code;
        }

        public String toString() {
            return getName();
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

        /**
         * gets set of children and children of children and so on of specified node
         */
        static Set getSubNodes(TQNode node) {
            Set children = new HashSet();
            if (node != null && node.children != null && node.children.length != 0) {
                for (int i = 0; i < node.children.length; i++) {
                    if (node.children[i] != null) {
                        children.add(node.children[i]);
                        Set grandchildren = getSubNodes(node.children[i]);
                        if (grandchildren != null)
                            children.addAll(grandchildren);
                    }
                }
            }
            return children;
        }

        /**
         * gets set of parents and parents of parents and so on of specified node
         */
        static Set getUpNodes(TQNode node) {
            Set ancestry = new TreeSet();
            TQNode parent = node.parent;
            while (parent != null) {
                ancestry.add(parent);
                parent = parent.parent;
            }
            return ancestry;
        }

        /**
         * gets set of children and children of children and so on of specified nodes
         */
        static Set getSubNodes(Set nodes) {
            Set kids = new HashSet();
            Iterator e;
            if (!(nodes == null) && !(nodes.isEmpty())) {
                e = nodes.iterator();
                while (e.hasNext()) {
                    Set children = getSubNodes((TQNode)e.next());
                        kids.addAll(children);
                }
            }
            return kids;
        }

        /**
         * gets set of parents and parents of parents and so on of specified nodes
         */
        static Set getUpNodes(Set nodes) {
            Set adults = new TreeSet();
            if (!(nodes == null) && !(nodes.isEmpty())) {
                Iterator e = nodes.iterator();
                while (e.hasNext()) {
                    Set parents = getUpNodes((TQNode)e.next());
                        adults.addAll(parents);
                }
            }
            return adults;
        }

        static TreePath getTreePath(TQNode node) {
            if (node.parent == null)
                return new TreePath(node);
            else
                return getTreePath(node.parent).pathByAddingChild(node);
        }

        static TreePath[] getTreePaths(Collection s) {
            if (s == null || s.size() == 0) return new TreePath[0];
            TreePath[] result = new TreePath[s.size()];
            Iterator e = s.iterator();
            for (int i=0; e.hasNext(); i++) {
                TQNode tqn = (TQNode)e.next();
                result[i] = getTreePath(tqn);
            }
            return result;
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
                        event = new TreeModelEvent(tree, TQNode.getTreePath(node));
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
        public void treeCollapsed(TreeExpansionEvent e) {
            TreePath p = e.getPath();
            TQNode node = (TQNode) (p.getLastPathComponent());
            vector = new Vector();
            vector.add(p);
            TQTree tree = (TQTree)e.getSource();

            TreePath[] paths = tree.getSelectionPaths();
            for(int i = 0; i < paths.length; i++) {
                if(p.isDescendant(paths[i]))
                    vector.add(paths[i]);
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

            // remove old paths first
/*            Set added = new HashSet();
            Set removed = new HashSet();
            for (int i = 0; i < paths.length; i++) {
                TreePath p = paths[i];
                TQNode node = (TQNode) (p.getLastPathComponent());
                if (!e.isAddedPath(p)) {
                    //node.setSelected(false);
                    added.add(node);
                }
                else removed.add(node);//node.setSelected(true);
            }
            added.addAll(TQNode.getSubNodes(added));
            removed.addAll(TQNode.getSubNodes(removed));
            TreePath[] atps = TQNode.getTreePaths(added);
            TreePath[] rtps = TQNode.getTreePaths(removed);
            Iterator i = added.iterator();
                //root.setSelected(true);
                while (i.hasNext()) {
                    TQNode node = (TQNode)i.next();
                    node.selectState = ALL_SELECTED;
                }
            i = removed.iterator();
                //root.setSelected(true);
                while (i.hasNext()) {
                    TQNode node = (TQNode)i.next();
                    node.selectState = NONE_SELECTED;
                }
            tree.getSelectionModel().addSelectionPaths(atps);
            tree.getSelectionModel().removeSelectionPaths(rtps);*/
            // now add new ones
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
        }

        void setIgnoreEvents(boolean b) {
            ignoreEvents = b;
        }

        private boolean ignoreEvents;
        private Vector vector;
    }

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

    private UIFactory uif;
    private static Icon[] folderIcons;
    private static Icon[] leafIcons;
}
