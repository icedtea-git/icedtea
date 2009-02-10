/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
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

package sun.nio.fs;

import java.util.regex.PatternSyntaxException;

public class Globs {
    private Globs() { }

    /**
     * Creates a regex pattern from the given glob expression.
     *
     * @throws  PatternSyntaxException
     */
    public static String toRegexPattern(String globPattern) {
        String regexMetaChars = ".^$+{[]|()";
        String globMetaChars = "\\*?[]{}";

        boolean inNonCapturingGroup = false;

        StringBuilder regex = new StringBuilder("^");

        int i =0;
        while (i< globPattern.length()) {
            char c = globPattern.charAt(i++);
            switch (c) {
                case '\\' :
                    // escape special characters
                    if (i < globPattern.length()) {
                        char next = globPattern.charAt(i);
                        if (globMetaChars.indexOf(next) >= 0) {
                            regex.append('\\');
                            regex.append(next);
                            i++;
                        }
                    } else {
                        throw new PatternSyntaxException("No character to escape",
                            globPattern, i-1);
                    }
                    break;

                case '[' :
                    regex.append('[');
                    // negation
                    if (i < globPattern.length() && globPattern.charAt(i) == '!') {
                        regex.append('^');
                        i++;
                    }
                    // hyphen allowed at start
                    if (i < globPattern.length() && globPattern.charAt(i) == '-') {
                        regex.append('-');
                        i++;
                    }
                    boolean inRange = false;
                    boolean seenRangeStart = false;
                    while (i < globPattern.length()) {
                        c = globPattern.charAt(i++);
                        regex.append(c);
                        if (c == ']')
                            break;
                        if (c == '-') {
                            if (inRange || !seenRangeStart) {
                                throw new PatternSyntaxException("Invalid range",
                                    globPattern, i-1);
                            }
                            inRange = true;
                        } else {
                            if (inRange) {
                                seenRangeStart = false;
                                inRange = false;
                            } else {
                                seenRangeStart = true;
                            }
                        }
                    }
                    if (c != ']')
                        throw new PatternSyntaxException("Missing ']", globPattern, i-1);
                    break;

                case '{' :
                    if (inNonCapturingGroup) {
                        throw new PatternSyntaxException("Cannot nest groups",
                            globPattern, i-1);
                    }
                    regex.append("(?:(?:");
                    inNonCapturingGroup = true;
                    break;

                case '}' :
                    if (inNonCapturingGroup) {
                        regex.append("))");
                        inNonCapturingGroup = false;
                    } else {
                        regex.append('}');
                    }
                    break;

                case ',' :
                    if (inNonCapturingGroup) {
                        regex.append(")|(?:");
                        break;
                    } else {
                        regex.append(',');
                    }
                    break;

                case '*':
                    regex.append(".*");
                    break;

                case '?':
                    regex.append('.');
                    break;

                default:
                    // escape other meta characters
                    for (int j=0; j<regexMetaChars.length(); j++) {
                        if (c == regexMetaChars.charAt(j)) {
                            regex.append('\\');
                            break;
                        }
                    }
                    regex.append(c);
            }
        }

        if (inNonCapturingGroup)
            throw new PatternSyntaxException("Missing '}", globPattern, i-1);

        return regex.append('$').toString();
    }
}
