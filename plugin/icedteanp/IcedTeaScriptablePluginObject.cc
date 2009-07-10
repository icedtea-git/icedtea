/* IcedTeaScriptablePluginObject.cc

   Copyright (C) 2009  Red Hat

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

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

#include "IcedTeaScriptablePluginObject.h"

IcedTeaScriptablePluginObject::IcedTeaScriptablePluginObject(NPP instance)
{
	this->instance = instance;
}

void
IcedTeaScriptablePluginObject::deAllocate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::deAllocate %p\n", npobj);
}

void
IcedTeaScriptablePluginObject::invalidate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::invalidate %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::hasMethod(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::hasMethod %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args,
			uint32_t argCount,NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::invoke %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::invokeDefault(NPObject *npobj, const NPVariant *args,
			       uint32_t argCount, NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::invokeDefault %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::hasProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::hasProperty %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::getProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::getProperty %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::setProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::setProperty %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::removeProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::removeProperty %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::enumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::enumerate %p\n", npobj);
}

bool
IcedTeaScriptablePluginObject::construct(NPObject *npobj, const NPVariant *args, uint32_t argCount,
	           NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::construct %p\n", npobj);
}
