// Copyright (C) 2009 Red Hat, Inc.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

package net.sourceforge.jnlp.util;

import java.io.File;

/**
 * This class contains a few file-related utility functions.
 *
 * @author Omair Majid
 */

public final class FileUtils {

    /**
     * list of characters not allowed in filenames
     */
    private static final char INVALID_CHARS[] = { '\\', '/', ':', '*', '?', '"', '<', '>', '|' };

    private static final char SANITIZED_CHAR = '_';

    /**
     * Clean up a string by removing characters that can't appear in a local
     * file name.
     *
     * @param path
     *        the path to sanitize
     * @return a sanitized version of the input which is suitable for using as a
     *         file path
     */
    public static String sanitizePath(String path) {

        for (int i = 0; i < INVALID_CHARS.length; i++)
            if (INVALID_CHARS[i] != File.separatorChar)
                if (-1 != path.indexOf(INVALID_CHARS[i]))
                    path = path.replace(INVALID_CHARS[i], SANITIZED_CHAR);

        return path;
    }

    /**
     * Given an input, return a sanitized form of the input suitable for use as
     * a file/directory name
     *
     * @param input
     * @return a sanitized version of the input
     */
    public static String sanitizeFileName(String filename) {

        for (int i = 0; i < INVALID_CHARS.length; i++)
            if (-1 != filename.indexOf(INVALID_CHARS[i]))
                filename = filename.replace(INVALID_CHARS[i], SANITIZED_CHAR);

        return filename;
    }

}
