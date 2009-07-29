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

/**
 * This class contains a few file-related utility functions. 
 * 
 * @author Omair Majid
 */

public class FileUtils {

    
    /**
     * Given an input, return a sanitized form of the input suitable for use as
     * a file/directory name
     * 
     * @param input
     * @return a sanitized version of the input
     */
    public static String sanitizeFileName(String input) {

        /*
         * FIXME
         * 
         * Assuming safe characters are 'a-z','A-Z','0-9', '_', '.'
         */

        String sanitizedName = input.replaceAll("[^a-zA-Z0-9.]", "_");
        return sanitizedName;
    }
    
}
