/* RH1195203 -- Check correct recognition of mime types.
   Copyright (C) 2015 Red Hat, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import javax.activation.MimetypesFileTypeMap;

public class RH1195203
{
    public static void main(String[] args)
    {
        if (args.length == 0)
        {
	    System.err.println("No file specified.");
	    System.exit(-1);
	}

        System.out.println(MimetypesFileTypeMap.getDefaultFileTypeMap().getContentType(args[0]));
    }
}
