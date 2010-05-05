/* jni_create_stap.c -- Parses jni_desc into hotspot_jni.stp.in
   Copyright (C) 2009  Red Hat, Inc.

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

IcedTea is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.
*/

// g++ -o jni_create_stap jni_create_stap
// ./jni_create_stap < jni_desc > hotspot_jni.stp.in
// (Don't forget to add header plus documentation)

// data file:
// JNIProbeName<space>Description
// arg1_name<space><type><space>Description
// arg2_name...
// [ret<space><type><space>Description]
// blank line
//
// Notes:
// JNIProbeName doesn't include __entry or __return.
// ret is the the return argument (if there is a __return probe)
// <type>/representation is one of:
// v - void, s - string, x - hex number, d - number
// void is only used for return ret arguments.

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

struct probe
{
  string name;
  string desc;
  vector<string> args;
  vector<char> types;
  vector<string> descs;
};

int
main()
{
  vector<probe> probes;

  string line;
  size_t index;
  while (getline (cin, line))
    {
      if (line.size () == 0 || line[0] == '#')
	continue;
      probe p;
      // probe name and description
      index = line.find (' ');
      p.name = line.substr (0, index);
      p.desc = line.substr (index + 1);
      
      // arguments
      while (getline (cin, line) && line.size () != 0)
	{
	  if (line[0] == '#')
	    continue;
	  
	  index = line.find (' ');
	  p.args.push_back (line.substr (0, index));
	  p.types.push_back (line[index + 1]);
	  p.descs.push_back (line.substr (index + 3));
	}

      probes.push_back (p);
    }

  vector<probe>::iterator it = probes.begin();
  while (it != probes.end())
    {
      // Output probe entry
      probe p = *it;
      cout << "/* hotspot.jni." << p.name << endl;
      cout << " * " << p.desc << endl;
      cout << " *" << endl;
      for (index = 0; index < p.args.size (); index++)
	{
	  if (p.args[index] == "ret")
	    continue;
	  cout << " * " << p.args[index] << " - " << p.descs[index] << endl;
	}
      cout << " */" << endl;
      cout << "probe hotspot.jni." << p.name << " =" << endl;
      cout << "  process(\"@ABS_CLIENT_LIBJVM_SO@\").mark(\""
	   << p.name << "__entry" << "\")," << endl;
      cout << "  process(\"@ABS_SERVER_LIBJVM_SO@\").mark(\""
	   << p.name << "__entry" << "\")" << endl;
      cout << "{" << endl;
      cout << "  name = \"" << p.name << '"' << endl;
      for (index = 0; index < p.args.size (); index++)
	{
	  if (p.args[index] == "ret")
	    continue;
	  cout << "  " << p.args[index] << " = ";
	  if (p.types[index] == 's')
	    cout << "user_string(" << "$arg" << (index + 1) << ")" << endl;
	  else
	    cout << "$arg" << (index + 1) << endl;
	}
      cout << "  probestr = sprintf(\"%s(";
      for (index = 0; index < p.args.size (); index++)
	{
	  if (p.args[index] == "ret")
            continue;
	  cout << p.args[index] << '=';
	  if (p.types[index] == 's')
	    cout << "'%s'";
	  else if (p.types[index] == 'x')
	    cout << "0x%x";
	  else
	    cout << "%" << p.types[index];
	  if (index != p.args.size () - 1 && p.args[index + 1] != "ret")
	    cout << ',';
	}
      cout << ")\", name";
      for (index = 0; index < p.args.size (); index++)
        {
          if (p.args[index] == "ret")
            continue;
	  cout << ", " << p.args[index];
	}
      cout << ")" << endl;
      cout << "}" << endl;

      // Output return probe if it exists
      if (p.args.size () > 0 && p.args[p.args.size () - 1] == "ret")
	{
	  char type = p.types[p.args.size () - 1];
	  cout << endl;
	  cout << "/* hotspot.jni." << p.name << ".return" << endl;
	  cout << " * " << p.desc << " Return." << endl;
	  if (type != 'v')
	    {
	      cout << " *" << endl;
	      cout << " * ret - " << p.descs[p.args.size() - 1] << endl;
	    }
	  cout << " */" << endl;

	  cout << "probe hotspot.jni." << p.name << ".return =" << endl;
	  cout << "  process(\"@ABS_CLIENT_LIBJVM_SO@\").mark(\""
	       << p.name << "__return" << "\")," << endl;
	  cout << "  process(\"@ABS_SERVER_LIBJVM_SO@\").mark(\""
	       << p.name << "__return" << "\")" << endl;
	  cout << "{" << endl;
	  cout << "  name = \"" << p.name << '"' << endl;
	  if (type == 's')
	    {
	      cout << "  ret = user_string($arg1)" << endl;
	      cout << "  retstr = ret" << endl;
	    }
	  else if (type == 'x')
	    {
	      cout << "  ret = $arg1" << endl;
	      cout << "  retstr = sprintf(\"0x%x\", ret)" << endl;
	    }
	  else if (type != 'v')
	    {
	      cout << "  ret = $arg1" << endl;
	      cout << "  retstr = sprint(ret)" << endl;
	    }
	  else
	    {
	      cout << "  retstr = \"\"" << endl;
	    }
	  cout << "}" << endl;
	}

      cout << endl;
      ++it;
    }
}
