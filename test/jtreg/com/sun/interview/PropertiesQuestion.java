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
package com.sun.interview;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.MissingResourceException;
import java.util.Properties;
import java.util.ResourceBundle;
import java.util.Set;

import com.sun.interview.Interview.Fault;

// todo:
//    how to sort values
//    how to sort groups for display
//    table headers - default, with set methods
//    improper use of default value (not used)

/**
 * A {@link Question question} which consists of many key-value pairs.  The
 * values are altered by the user, the key is immutable. The output of this
 * question is a Properties object.  The key in the properties object is always
 * the internal name, not the internationalized name.  If internationalized
 * key names are supplied, they are used only for presentation.
 *
 * The presentation info is store here instead of in a renderer class because
 * multiple clients need to render this question.
 *
 * @since 4.0
 */
public abstract class PropertiesQuestion extends CompositeQuestion
{
    /**
     * Create a question with a nominated tag.
     * If this constructor is used, the choices must be supplied separately.
     * @param interview The interview containing this question.
     * @param tag A unique tag to identify this specific question.
     */
    protected PropertiesQuestion(Interview interview, String tag) {
        super(interview, tag);
        clear();
        setDefaultValue(value);
    }

    /**
     * Create a question with a nominated tag.  Not recommended since this is
     * not internationalized.
     * @param interview The interview containing this question.
     * @param tag A unique tag to identify this specific question.
     * @param props The literal keys and values.  A shallow copy of this is used.
     * @throws NullPointerException if choices is null
     */
    protected PropertiesQuestion(Interview interview, String tag, Properties props) {
        super(interview, tag);

        setProperties(props);
        setDefaultValue(value);
    }

    /**
     * Create a question with a nominated tag.  The keys must be registered in the
     * resource bundle for this question for internationalization purposes.  They
     * will be looked up by this questions tag, plus each of the key values.
     * @param interview The interview containing this question.
     * @param tag A unique tag to identify this specific question.
     * @param keys Internal name of the keys
     * @throws NullPointerException if choices is null
     */
    protected PropertiesQuestion(Interview interview, String tag, String[] keys) {
        super(interview, tag);

        String blank = "";
        setKeys(keys, true);
        setDefaultValue(value);
    }

    /**
     * The current value will be set to all false;
     * @param choices The set of names for the choices for this question.
     * @see #getChoices
     * @see #getDisplayChoices
     * @throws NullPointerException if choices is null
     */
    protected void setProperties(Properties props) {
        value = ((Properties)props.clone());
        // clobber display values?
    }

    /**
     * Set the keys to be shown in the properties table.  Previous properties
     * are removed, and the new values are all the empty string.
     * The current value will be set an empty string.
     * @param choices The set of names of the choices for this question.
     * @param localize if false, the choices will be used directly
     * as the display choices; otherwise the choices will be used
     * to construct keys to get localized values from the interview's
     * resource bundle.
     * @see #getChoices
     * @see #getDisplayChoices
     * @throws NullPointerException if choices is null
     */
    protected void setKeys(String[] keys, boolean localize) {
        value = new Properties();
        String blank = "";

        // populate the properties object
        for (int i = 0; i < keys.length; i++) {
            value.put(keys[i], blank);
        }

        ResourceBundle b = interview.getResourceBundle();
        if (!localize || b == null)     // will use literal keys
            presentationKeys = null;
        else {
            presentationKeys = new HashMap();
            for (int i = 0; i < keys.length; i++) {
                String c = keys[i];
                String rn = tag + "." + c;
                try {
                    presentationKeys.put(keys[i], (c == null ? null : b.getString(rn)));
                }
                catch (MissingResourceException e) {
                    System.err.println("WARNING: missing resource " + rn);
                    presentationKeys.put(keys[i], c);
                }
            }   // for
        }   // else
    }

    /**
     * Get the set of keys currently used this question.  Includes all hidden and
     * read-only values as well.
     * @return The set of keys (internal non-i18n value)
     * @see #setChoices
     */
    public Enumeration getKeys() {
        if (value != null)
            return value.keys();
        else
            return null;
    }

    /**
     * Get the default response for this question.
     * @return the default response for this question.
     *
     * @see #setDefaultValue
     */
    public Properties getDefaultValue() {
        return defaultValue;
    }

    /**
     * Set the default response for this question,
     * used by the clear method.
     * @param v the default response for this question.
     *
     * @see #getDefaultValue
     */
    public void setDefaultValue(Properties props) {
        defaultValue = props;
    }

    /**
     * Get the current (default or latest) response to this question.
     * @return The current value - a cloned copy.
     * @see #setValue
     * @throws IllegalStateException if no choices have been set, defining
     *   the set of responses to this question
     */
    public Properties getValue() {
        if (value == null)
            return null;

        return (Properties)(value.clone());
    }

    /**
     * Verify this question is on the current path, and if it is,
     * return the current value.
     * @return the current value of this question
     * @throws Interview.NotOnPathFault if this question is not on the
     * current path
     * @see #getValue
     */
    public Properties getValueOnPath() throws Interview.NotOnPathFault {
        interview.verifyPathContains(this);
        return getValue();
    }

    public String getStringValue() {
        if (value == null)
            return "";

        // if upgraded, the values should probably be sorted
        // and you may consider removing the custom implemntation
        // of equals()

        StringWriter sw = new StringWriter();
        if (value != null) {
            value.list(new PrintWriter(sw));
        }

        return sw.toString();
    }

    public boolean equals(Object other) {

//        boolean basicEquality = super.equals(other);
        if ( !(other instanceof PropertiesQuestion))
            return false;

        PropertiesQuestion pq = (PropertiesQuestion)other;

        boolean basicEquality = this.tag.equals(pq.tag);
        if(!basicEquality) {
            return false;
        }

        if ( pq.value == null && value == null)
            return true;

        if ( (pq.value == null && value != null) ||
             (value == null && pq.value != null) )
            return false;

        if (pq.value.size() != value.size())
            return false;

        Enumeration names = pq.value.propertyNames();
        while (names.hasMoreElements()) {
            Object k = names.nextElement();
            Object val1 = pq.value.getProperty((String)(k));
            Object val2 = value.getProperty((String)(k));

            if (val1 == val2)
                continue;
            else if (val1 != null && val1.equals(val2))
                continue;
            else
                return false;
        }   // while

        return true;
    }

    /**
     * Set the current value.
     * @param newValue Value represented as a single string.  May not be null.  May be
     *    an empty string.
     * @see #getValue
     */
    public void setValue(String newValue) {
        if (value == null || value.size() == 0)
            return;

        // parse newValue and inject into properties object
        if (newValue == null)
            throw new NullPointerException();

        setValue(load(newValue));
    }

    /**
     * Set a specific property within this question.
     * The property must exist before it can be set, else a
     * Fault is thrown to prevent unauthorized additions of
     * properties to the question value.
     * @param key the key of the property to set, must not be null
     * @param value the new value for the property, must not be null
     * @throws Interview.Fault if the key does not already exist
     *         in the question's value.
     * @throws NullPointerException if either parameter is null
     * @throws Interview.Fault if the specified key does not exist in the question
     */
    public void setValue(String key, String v) throws Interview.Fault {
        if (key == null || v == null)
            throw new NullPointerException();

        String check = value.getProperty(key);
        if (check == null)
            throw new Fault(Interview.i18n, "props.badSubval");
        value.put(key, v);

        interview.updatePath(this);
        interview.setEdited(true);
    }

    public boolean isValueValid() {
        String[][] badVals = getInvalidKeys();

        if (badVals == null)
            return true;
        else
            return false;
    }

    public boolean isValueAlwaysValid() {
        return false;
    }

    /**
     * Clear any response to this question, resetting the value
     * back to its initial state.
     */
    public void clear() {
        setValue(defaultValue);
    }

    /**
     * Load the value for this question from a dictionary, using
     * the tag as the key.
     * @param data The map from which to load the value for this question.
     */
    protected void load(Map data) {
        Object o = data.get(tag);
        if (o != null && o instanceof String) {
            setValue(load((String)o));
        }
    }

    protected static Properties load(String s) {
        Properties2 p2 = new Properties2();

        try {
            p2.load(new StringReader(s));
        } catch (IOException e) {
            //e.printStackTrace();
            // what to do?!  source should be really stable since it is a
            // String
        }

        // repopulate a J2SE properties object
        Properties p = new Properties();

        Enumeration e = p2.propertyNames();
        while(e.hasMoreElements()) {
            Object next = e.nextElement();
            p.put( ((String)next), p2.get(next) );
        }   // while

        return p;
    }

    /**
     * Save the value for this question in a dictionary, using
     * the tag as the key.
     * @param data The map in which to save the value for this question.
     */
    protected void save(Map data) {
        if (value == null)
            return;

        Properties2 p2 = new Properties2();
        p2.load(value);
        StringWriter sw = new StringWriter();
        p2.save(sw, null);
        data.put(tag, sw.toString());
    }

    // extra special features

    // ------------- MISC METHODS -------------

    /**
     * Make the given value read-only.  It may be viewed, and used for
     * export, but not be modified by the user.
     * @throws IllegalArgumentException If the given key does not exist
     *      in the quuestion's property table.
     */
     /*
    public void setReadOnlyValue(String key) {
        if (value == null)
            throw new IllegalArgumentException("Question does not have a value yet.");

        if (readOnlyKeys == null)
            readOnlyKeys = new HashSet();

        if (value.getProperty(key) == null)
            throw new IllegalArgumentException("No such key: " + key);

        readOnlyKeys.add(key);
    }
     */

    /**
     * Determine if a value is read-only.
     */
    public boolean isReadOnlyValue(String key) {
        ValueConstraints c = getConstraints(key);
        if (c == null || !c.isReadOnly())
            return false;
        else
            return true;
    }

    /**
     * Determine if the given property is visible to the user.
     * If it is not, the value is not presented in the GUI for editing and
     * is hidden in any reports which show the state of the interview.  Nonetheless,
     * it is still present and can be altered manually on the command line or by
     * editing the configuration file.  So it is truely invisible, yet real.
     *
     * @return True if the entry is visible (default), false otherwise.
     */
    public boolean isEntryVisible(String key) {
        ValueConstraints c = getConstraints(key);
        if (c == null || c.isVisible())
            return true;
        else
            return false;
    }

    /**
     * Get the keys which are currently invalid and blocking the question
     * (getNext() returning null).  It is recommended but not required that
     * this method return null if the question is not blocked (getNext() !=
     * null).  This default implementation of this method is to check any
     * ValueConstraint objects for each key and return those results.  If you
     * override this method, it is highly recommended that you allow this to
     * take place, then add in any additional checking to the results provided
     * by this base implementation.
     * @return Invalid key in index zero, <em>localized</em> explanation
     *         in index one.  Null means there are no invalid keys.
     */
    public String[][] getInvalidKeys() {
        Enumeration names = value.propertyNames();
        ArrayList badKeys = new ArrayList();
        ArrayList reasons = new ArrayList();

        while (names.hasMoreElements()) {
            String curr = (String)(names.nextElement());
            ValueConstraints rules = getConstraints(curr);

            if (rules == null) {
                continue;   // no checks, next key
            }
            else {
                String reason = rules.isValid(value.getProperty(curr));
                if (reason != null) {
                    badKeys.add(curr);
                    reasons.add(reason);
                }
            }
        }

        // repack data for return
        if (badKeys.size() > 0) {
            String[][] ret = new String[badKeys.size()][2];
            for (int i = 0; i < badKeys.size(); i++) {
                ret[i][0] = (String)(badKeys.get(i));
                ret[i][1] = (String)(reasons.get(i));
            }   // for

            return ret;
        }
        else
            return null;
    }

    /**
     * Convienence method for finding out the status of a particular value.
     * This method is final because subclasses should implement getInvalidKeys().
     * @param key The key to query.  Must not be null.
     * @return The explanation for the value being invalid.  Null if the value
     *         is reported as valid.
     * @see #getInvalidKeys
     */
    public final String isValueValid(String key) {
        if (key == null)
            throw new IllegalArgumentException("Key parameter null!");

        String[][] badVals = getInvalidKeys();

        if (badVals == null)
            return null;

        for (int i = 0; i < badVals.length; i++)
            if (badVals[i][0].equals(key))
                return badVals[i][1];

        return null;
    }

    // ------------- UPDATE METHODS -------------
    /**
     * Private because we need to maintain internal consistency, especially with
     * the i18n info.
     */
    public void setValue(Properties props) {
        if (props == null) {
            value = null;
        }
        else {
            value = ((Properties)props.clone());
        }
        // what to do about read-only and other tagging values?  flush?
        // remove the extra ones?
        // should work ok for now if we just leave it, probably safer to leave
        // it

        interview.updatePath(this);
        interview.setEdited(true);
    }

    /**
     * Update the given properties.  New properties cannot be added this way.
     * @param props Properties to update, keys in first index, values in the second.
     * @throws IllegalArgumentException If a property in <code>props</code> does not
     *     exist.
     */
    public void updateProperties(String[][] props) {
        if (props == null || props.length == 0)
            throw new IllegalArgumentException("Argument is null or zero length.");

        for (int i = 0; i < props.length; i++) {
            updateProperty(props[i][0], props[i][1]);
        }
    }

    /**
     * Update the given property.  New properties cannot be added this way.
     * @param key Property to update.
     * @param value Value for the property.
     * @throws IllegalArgumentException If the property does not exist.
     */
    public void updateProperty(String key, String val) {
        if (!value.containsKey(key))
            throw new IllegalArgumentException("Key " + key + " does not exist");

        Object old = value.setProperty(key, val);
        if (!old.equals(val)) {
            interview.updatePath(this);
            interview.setEdited(true);
        }
    }

    // ------------- GROUP MANAGEMENT -------------

    /**
     * Create a new group.
     * @throws IllegalStateException If the group requested already exists.
     */
    public void createGroup(String name) {
        if (keyGroups == null)
            keyGroups = new HashMap();

        Object o = keyGroups.get(name);
        if (o != null)
            throw new IllegalStateException("Group " + name + " already exists.");

        ArrayList al = new ArrayList();
        keyGroups.put(name, al);
    }

    /**
     * Set the presentation group to which the key(s) should belong.
     * If the key is in another group, it will be removed from that one.
     * The
     * @param group internal name for the group.  Internationalized version
     *     must be available in the resource bundle as tag+group.
     * @param key Which keys to add to the group.
     * @throws IllegalArgumentException If an attempt is made to add to a group which
     *         does not exist.
     * @throws IllegalStateException If an attempt is made to group a key which is not
     *         present.
     * @see #createGroup
     */
    public void setGroup(String group, String key) {
        if (value == null)
            throw new IllegalStateException(
                    "Question has no values, cannot group non-existant key");
        if (!value.containsKey(key))
            throw new IllegalArgumentException("Key " + key + " does not exist");
        if (keyGroups == null)
            throw new IllegalArgumentException("No such group: " + group);

        // find existing group or create
        ArrayList l = ((ArrayList)(keyGroups.get(group)));
        if (l == null)
            throw new IllegalArgumentException("No such group: " + group);

        // remove key from all groups
        Iterator vals = keyGroups.values().iterator();
        while (vals.hasNext()) {
            ArrayList al = ((ArrayList)(vals.next()));
            for (int i = 0; i < al.size(); i++)
                if (al.get(i).equals(key))
                    al.remove(i);
        }

        // add to group
        for (int i = 0; i < l.size(); i++) {
            if (l.get(i).equals(key))
                return;     // already there
        }
        l.add(key);
    }

    public void setGroup(String group, String[] keys) {
        if (keys == null || keys.length == 0)
            return;

        for (int i = 0; i < keys.length; i++)
            setGroup(group, keys[i]);
    }

    /**
     * Get the names of the groups being used by the current set of values.
     * Groups which are empty are not listed; groups may become empty if client
     * code attempts to put a key in more than one group.
     * @return Group names, null if no grouping is in use.
     * @see #setKeyGroup
     */
    public String[] getGroups() {
        if (keyGroups == null)
            return null;

        ArrayList result = new ArrayList();
        Set keys = keyGroups.keySet();
        if (keys != null) {
            Iterator it = keys.iterator();
            while(it.hasNext()) {
                Object key = it.next();
                ArrayList al = ((ArrayList)keyGroups.get(key));
                if (al == null || al.size() == 0)
                    continue;       // empty group

                result.add(key);
            }   // while
        }

        if (result.size() == 0)
            return null;
        else {
            String[] ret = new String[result.size()];
            ret = (String[])(result.toArray(ret));
            return ret;
        }
    }

    /**
     * Get the keys which are registered with the given group.
     @ @param group Group name to query.  Null returns the groupless keys.
     * @return Null if the group is empty or does not exist, else the keys and
     *         values in this array.
     * @see #getGroups
     * @see #setGroup
     */
    public String[][] getGroup(String group) {
        if (value == null || value.size() == 0)
            return null;

        if (group == null)
            return getUngrouped();

        ArrayList al = ((ArrayList)keyGroups.get(group));

        if (al == null || al.size() == 0)
            return null;
        else {
            Iterator it = al.iterator();
            String[][] data = new String[al.size()][2];
            for(int i = 0; it.hasNext(); i++) {
                data[i][0] = (String)(it.next());
                data[i][1] = (String)(value.get(data[i][0]));
            }   // for

            return data;
        }
    }

    /**
     * @return The keys and values which are not allocated to any group.
     *         Null if there are no ungrouped values.
     * @see #getGroup
     * @see #setGroup
     */
    public String[][] getUngrouped() {
        if (value == null || value.size() == 0)
            return null;

        if (keyGroups != null) {
            Set keys = keyGroups.keySet();
            if (keys != null) {
                String[] gps = getGroups();
                Properties copy = (Properties)(value.clone());

                // remove all grouped entries from the copied
                // question value
                for (int i = 0; i < gps.length; i++) {
                    String[][] vals = getGroup(gps[i]);

                    for (int j = 0; j < vals.length; j++)
                        copy.remove(vals[j][0]);
                }

                if (copy.size() > 0) {
                    Enumeration en = copy.propertyNames();
                    String[][] ret = new String[copy.size()][2];
                    int i = 0;

                    while (en.hasMoreElements()) {
                        String key = (String)(en.nextElement());
                        ret[i][0] = key;
                        ret[i][1] = copy.getProperty(key);
                        i++;
                    }   // while

                    return ret;
                }
                else
                    return null;
            }
        }
        // no groups, return the entire value set
        String[][] ret = new String[value.size()][2];
        Enumeration en = value.propertyNames();
        int i = 0;

        while (en.hasMoreElements()) {
            String key = (String)(en.nextElement());
            ret[i][0] = key;
            ret[i][1] = value.getProperty(key);
            i++;
        }   // while

        return ret;        // no groups in use
    }

    // ------------ PRESENTATION ---------------

    /**
     * Get the display (localized) name of the group.
     * The resource necessary is the question tag, with the group name and ".group"
     * appended.  That is <code>jck.qTable.basic.group</code> and
     * <code>jck.qTable.advanced.group</code>.
     * @param group The internal group name, as is used in the rest of this API.
     * @return The localized group name, the generated bundle key if that
     *       cannot be found.
     */
    public String getGroupDisplayName(String group) {
        ResourceBundle b = interview.getResourceBundle();
        presentationKeys = new HashMap();
        String rn = tag + "." + group + ".group";

        try {
            return b.getString(rn);
        }
        catch (MissingResourceException e) {
            System.err.println("WARNING: missing resource " + rn);
            return rn;
        }
    }

    public String getKeyHeaderName() {
        // XXX upgrade to be customizable
        return Interview.i18n.getString("props.key.name");
    }

    public String getValueHeaderName() {
        // XXX upgrade to be customizable
        return Interview.i18n.getString("props.value.name");
    }


    // ------------ TYPING and CONSTRAINTS ---------------

    /**
     * Apply constraints to a value.
     * @throws IllegalArgumentException If the key supplied does not
     *        exist in the current data.
     * @see PropertiesQuestion.ValueConstraints
     * @see PropertiesQuestion.IntConstraints
     * @see PropertiesQuestion.FloatConstraints
     * @see PropertiesQuestion.BooleanConstraints
     */
    public void setConstraints(String key, ValueConstraints c) {
        if (constraints == null)
            constraints = new HashMap();

        if (value.getProperty(key) == null)
            throw new IllegalArgumentException("No such key: " + key);

        constraints.put(key, c);
    }

    /**
     * @param key The key for the value to get contraints for.
     * @return Constraints object for the specified key, null if there are no
     *      known constraints.
     */
    public ValueConstraints getConstraints(String key) {
        if (constraints == null)
            return null;
        else
            return ((ValueConstraints)(constraints.get(key)));
    }

    /**
     * The localized values to display, corresponding 1-1 to the
     * set of property keys.  Null indicates that no i18n presentation values
     * are provided.
     */
    private HashMap presentationKeys;

    /**
     * Indexed like everything else, by proeprty key, the value is a
     * ValueConstraint.
     */
    private HashMap constraints;

    /**
     * The current (default or latest) response to this question.
     */
    protected Properties value;

    /**
     * The default response for this question.
     */
    private Properties defaultValue;

    /**
     * Record of key groupings.  Key is the group name string, the value is
     * a ArrayList.
     */
    private HashMap keyGroups;

    // these are now represented in ValueConstraints
    //private HashSet readOnlyKeys;
    //private HashSet hiddenKeys;

    public static class ValueConstraints {
        public ValueConstraints() { this(false, true); }

        public ValueConstraints(boolean readonly, boolean visible) {
            this.readonly = readonly;
            this.visible = visible;
        }

        /**
         * Determine whether this value should be readable only, by the
         * interview user.  The default state is false.
         */
        public void setReadOnly(boolean state) { readonly = state; }

        /**
         * Make value outwardly visible or invisible.  This does not
         * mean it is not accessible, just that it is not shown when
         * possible in the user interfaces.  The default state is true.
         */
        public void setVisible(boolean state) { visible = state; }

        public boolean isReadOnly() { return readonly; }
        public boolean isVisible() { return visible; }

        /**
         * May the answer be set to an unanswered state.  If false, the
         * question will always be answered.  If true, the question may
         * be set to an affirmative, negative or unset response.  An unset
         * response is considered an incomplete answer by default.
         * @see #isUnsetAllowed
         */
        public void setUnsetAllowed(boolean state) { allowUnset = state; }

        /**
         * Is an unset response allowed.  The default is true, unless indicated
         * otherwise by a subclass.
         * @see #setUnsetAllowed
         */
        public boolean isUnsetAllowed() { return allowUnset; }

        /**
         * Is the given value valid for this field?  Since this constraint
         * class has no particular typing, the default only check that the
         * value is non-empty.  You may override this method to do custom
         * checking, or you may do your checking in getInvalidKeys() which
         * by default defers to the associated constraint object (if any).
         * @param v The value to check.
         * @return Null if the valid is valid, a localized reason string
         *         otherwise.
         * @see #getInvalidKeys
         */
        public String isValid(String v) {
            if (v == null || v.length() == 0)
                return "Value is not set";
            else
                return null;
        }

        private boolean visible = true;
        private boolean readonly = false;
        private boolean allowUnset = true;
    }

    public static class IntConstraints extends ValueConstraints {
        public IntConstraints() { super(); }

        public IntConstraints(int min, int max) {
            this();
            setBounds(min, max);
        }

        public IntConstraints(int[] suggesitons) {
            this();
            setSuggestions(suggesitons);
        }

        public IntConstraints(int min, int max, int[] suggestions) {
            this();
            setBounds(min, max);
            setSuggestions(suggestions);
        }

        /**
         * Set the max/min possible value that should be considered
         * valid.  The range in inclusive.  The defaults are the
         * MIN and MAX values for the integer type, except the minimum
         * value itself, which is reserved.
         */
        public void setBounds(int min, int max) {
            this.max = max;
            this.min = min;
        }

        public int getLowerBound() { return min; }

        public int getUpperBound() { return max; }

        /**
         * Get the suggested values.  Not a copy, do not alter the array.
         */
        public int[] getSuggestions() { return suggestions; }

        /**
         * Are user specified values allowed?  If not, there must be
         * suggestions present.
         * @throws IllegalStateException If no suggestions have been
         *      provided.
         * @see #setSuggestion
         */
        public void setCustomValuesAllowed(boolean state) {
            custom = state;
        }

        /**
         * Are custom user values allowed?
         * @see #setCustomValuesAllowed
         * @see #setSuggestions
         */
        public boolean isCustomValuesAllowed() {
            return custom;
        }

        /**
         * Supply some possible values that the user may want to
         * select from.
         */
        public void setSuggestions(int[] sugs) {
            suggestions = new int[sugs.length];
            System.arraycopy(sugs, 0, suggestions, 0, sugs.length);
        }

        /**
         * Is the given value valid for this field?  The basic check for
         * validity is to see if the given string can be parsed as an
         * integer value in the current locale.
         * @param v The value to check.
         * @return Null if the valid is valid, a localized reason string
         *         otherwise.
         */
        public String isValid(String v) {
            try {
                int number = Integer.parseInt(v);
                return isValid(number);
            }
            catch (NumberFormatException e) {
                return "Not an integer.";   // XXX i18n
            }
        }

        /**
         * Is the given value valid for this field?
         * @return Null if the valid is valid, a localized reason string
         *         otherwise.
         */
        public String isValid(int v) {
            if (v < min || v > max)
                return "Value out of range (" + v + "), must be between " +
                        min + " and " + max;
            else
                return null;
        }

        protected int[] suggestions;
        protected boolean custom = true;

        /**
         * The lower bound for responses to this value.
         */
        private int min = Integer.MIN_VALUE + 1;

        /**
         * The upper bound for responses to this value.
         */
        private int max = Integer.MAX_VALUE;
    }

    public static class FloatConstraints extends ValueConstraints {
        public FloatConstraints() {
            super();
        }

        public FloatConstraints(float min, float max) {
            this();
            setBounds(min, max);
        }

        public FloatConstraints(float[] suggestions) {
            this();
            setSuggestions(suggestions);
        }

        public FloatConstraints(float min, float max, float[] suggestions) {
            this();
            setBounds(min, max);
            setSuggestions(suggestions);
        }

        /**
         * Set the max/min possible value that should be considered
         * valid.  The range in inclusive.  The defaults are the
         * MIN and MAX values for the float type.
         */
        public void setBounds(float min, float max) {
            this.max = max;
            this.min = min;
        }

        public float getLowerBound() { return min; }

        public float getUpperBound() { return max; }

        /**
         * Get the suggested values.  Not a copy, do not alter the array.
         */
        public float[] getSuggestions() { return suggestions; }

        /**
         * Are user specified values allowed?  If not, there must be
         * suggestions present.
         * @throws IllegalStateException If no suggestions have been
         *      provided.
         * @see #setSuggestion
         */
        public void setCustomValuesAllowed(boolean state) {
            custom = state;
        }

        /**
         * Are custom user values allowed?
         * @see #setCustomValuesAllowed
         * @see #setSuggestions
         */
        public boolean isCustomValuesAllowed() {
            return custom;
        }

        /**
         * Supply some possible values that the user may want to
         * select from.
         */
        public void setSuggestions(float[] sugs) {
            suggestions = new float[sugs.length];
            System.arraycopy(sugs, 0, suggestions, 0, sugs.length);
        }

        /**
         * Set the resolution for responses to this question. Responses
         * may be rounded to the nearest multiple of the resolution.
         * @param resolution the resolution for responses to this question
         * @see #getResolution
         * @see #setValue
         */
        public void setResolution(float resolution) {
            this.resolution = resolution;
        }

        /**
         * Get the resolution for responses to this question. Responses
         * may be rounded to the nearest multiple of the resolution.
         * @return the resolution for responses to this question
         * @see #setResolution
         * @see #setValue
         */
        public float getResolution() {
            return resolution;
        }

        /**
         * Is the given value valid for this field?  The basic check for
         * validity is to see if the given string can be parsed as an
         * floating point value in the current locale.
         * @param v The value to check.
         * @return Null if the valid is valid, a localized reason string
         *         otherwise.
         */
        public String isValid(String v) {
            try {
                float number = Float.parseFloat(v);
                return isValid(number);
            }
            catch (NumberFormatException e) {
                return "Not an floating point number.";   // XXX i18n
            }
        }

        /**
         * Is the given value valid for this field?
         * @return Null if the valid is valid, a localized reason string
         *         otherwise.
         */
        public String isValid(float v) {
            if (v < min || v > max)
                return "Value out of range ( " + v + "), must be between " +
                        min + " and " + max;
            else
                return null;
        }

        protected float[] suggestions;
        protected boolean custom = true;

        /**
         * The lower bound for responses to this value.
         */
        private float min = Float.MIN_VALUE;

        /**
         * The upper bound for responses to this value.
         */
        private float max = Float.MAX_VALUE;

        /**
         * The resolution for responses to this question
         */
        private float resolution;
    }

    public static class StringConstraints extends ValueConstraints {
        public StringConstraints() {
            super();
        }

        public StringConstraints(String[] suggestions) {
            this();
            setSuggestions(suggestions);
        }

        public StringConstraints(int maxLen) {
            this();
            setNominalMaxLength(maxLen);
        }

        public StringConstraints(String[] suggestions, int maxLen) {
            this();
            setSuggestions(suggestions);
            setNominalMaxLength(maxLen);
        }

        /**
         * Supply some possible values that the user may want to
         * select from.
         */
        public void setSuggestions(String[] sugs) {
            suggestions = new String[sugs.length];
            System.arraycopy(sugs, 0, suggestions, 0, sugs.length);
        }

        public String[] getSuggestions() {
            return suggestions;
        }

        /**
         * Are user specified values allowed?  If not, there must be
         * suggestions present.
         * @throws IllegalStateException If no suggestions have been
         *      provided.
         * @see #setSuggestion
         */
        public void setCustomValuesAllowed(boolean state) {
            custom = state;
        }

        /**
         * Are custom user values allowed?
         * @see #setCustomValuesAllowed
         * @see #setSuggestions
         */
        public boolean isCustomValuesAllowed() {
            return custom;
        }

        /**
         * Get the nominal maximum length for the string.
         * @return the nominal maximum length for the string.
         *
         * @see #setNominalMaxLength
         */
        public int getNominalMaxLength() {
            return nominalMaxLength;
        }

        /**
         * Set the expected maximum length for the string.
         * @param nominalMaxLength  the nominal maximum length for the string.
         *
         * @see #getNominalMaxLength
         */
        public void setNominalMaxLength(int nominalMaxLength) {
            this.nominalMaxLength = nominalMaxLength;
        }

        protected String[] suggestions;
        protected boolean custom = true;

        /**
         * The nominal maximum length for the string.
         */
        protected int nominalMaxLength;
    }

    public static class BooleanConstraints extends ValueConstraints {
        public BooleanConstraints() {
            super();
        }

        /**
         * @see #setUseYesNo
         */
        public BooleanConstraints(boolean isYesNo) {
            this();
            setUseYesNo(isYesNo);
        }

        /**
         * @see #setUnsetAllowed
         * @see #setUseYesNo
         */
        public BooleanConstraints(boolean isYesNo, boolean unsetAllowed) {
            this();
            setUseYesNo(isYesNo);
            setUnsetAllowed(unsetAllowed);
        }

        /**
         * Indicate whether this should be a Yes/No question or
         * True/False.
         * @see #isYesNo
         */
        public void setUseYesNo(boolean state) {
            yesno = state;
        }

        /**
         * Is this a yes/no field, instead of the default true/false?
         * @return True if this is a Yes/No question, false if it is a
         *   True/False question.
         * @see #setUseYesNo
         */
        public boolean isYesNo() {
            return yesno;
        }

        private boolean yesno = false;
    }

    public static class FilenameConstraints extends ValueConstraints {
        public FilenameConstraints() {
            super();
        }

        public FilenameConstraints(File baseDir, boolean relativeOnly) {
            this();
            this.baseDir = baseDir;
            this.baseRelativeOnly = relativeOnly;
        }

        /**
         * Get the filters used to select valid files for a response
         * to this question.
         * @return An array of filters
         * @see #setFilter
         * @see #setFilters
         */
        public FileFilter[] getFilters() {
            return filters;
        }

        /**
         * Set a filter used to select valid files for a response
         * to this question.  Use this method, or setFilters(), not both.
         * @param filter a filter used to select valid files for a response
         * to this question
         * @see #getFilters
         * @see #setFilters
         */
        public void setFilter(FileFilter filter) {
            filters = new FileFilter[] { filter };
        }

        /**
         * Set the filters used to select valid files for a response
         * to this question.  The first element in the array is selected by
         * default.  Use this method, or setFilter(), not both.
         * @param filters An array of filters used to select valid files for a response
         * to this question
         * @see #getFilters
         * @see #setFilter
         */
        public void setFilters(FileFilter[] filters) {
            this.filters = filters;
        }

        /**
         * Get the default directory for files for a response to this question.
         * @return the default directory in which files should be found/placed
         * @see #setBaseDirectory
         * @see #isBaseRelativeOnly
         */
        public File getBaseDirectory() {
            return baseDir;
        }

        /**
         * Set the default directory for files for a response to this question.
         * @param dir the default directory in which files should be found/placed
         * @see #getBaseDirectory
         */
        public void setBaseDirectory(File dir) {
            baseDir = dir;
        }

        /**
         * Determine whether all valid responses to this question should be
         * relative to the base directory (in or under it).  False by
         * default.
         * @return true if all valid responses to this question should be
         * relative to the base directory
         * @see #setBaseRelativeOnly
         */
        public boolean isBaseRelativeOnly() {
            return baseRelativeOnly;
        }

        /**
         * Specify whether all valid responses to this question should be
         * relative to the base directory (i.e. in or under it.)
         * @param b this parameter should be true if all valid responses
         * to this question should be relative to the base directory
         * @see #setBaseRelativeOnly
         */
        public void setBaseRelativeOnly(boolean b) {
            baseRelativeOnly = b;
        }

        /**
         * Supply some possible values that the user may want to
         * select from.  The <code>getPath()</code> string will be used for
         * presentation and persistent storage of the value.
         */
        public void setSuggestions(File[] sugs) {
            // validate sugs
            suggestions = new File[sugs.length];
            System.arraycopy(sugs, 0, suggestions, 0, sugs.length);
        }

        public File[] getSuggestions() {
            return suggestions;
        }

        private File baseDir;
        private boolean baseRelativeOnly;
        private FileFilter[] filters;
        protected File[] suggestions;
    }
}
