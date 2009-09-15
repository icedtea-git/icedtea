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
	IcedTeaPluginUtilities::storeInstanceID(this, instance);
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
	return false;
}

bool
IcedTeaScriptablePluginObject::invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args,
			uint32_t argCount,NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::invoke %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::invokeDefault(NPObject *npobj, const NPVariant *args,
			       uint32_t argCount, NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::invokeDefault %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::hasProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::hasProperty %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::getProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	// Package request?
	if (!strcmp(browser_functions.utf8fromidentifier(name), "java"))
	{
		//NPObject* obj = IcedTeaScriptablePluginObject::get_scriptable_java_package_object(getInstanceFromMemberPtr(npobj), name);
		//OBJECT_TO_NPVARIANT(obj, *result);

		//printf ("Filling variant %p with object %p\n", result);
	}

	return false;
}

bool
IcedTeaScriptablePluginObject::setProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::setProperty %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::removeProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::removeProperty %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::enumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::enumerate %p\n", npobj);
	return false;
}

bool
IcedTeaScriptablePluginObject::construct(NPObject *npobj, const NPVariant *args, uint32_t argCount,
	           NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptablePluginObject::construct %p\n", npobj);
	return false;
}

NPObject*
allocate_scriptable_jp_object(NPP npp, NPClass *aClass)
{
    PLUGIN_DEBUG_0ARG("Allocating new scriptable Java Package object\n");
    return new IcedTeaScriptableJavaPackageObject(npp);
}

NPObject*
IcedTeaScriptablePluginObject::get_scriptable_java_package_object(NPP instance, const NPUTF8* name)
{

	NPObject* scriptable_object;

	NPClass* np_class = new NPClass();
	np_class->structVersion = NP_CLASS_STRUCT_VERSION;
	np_class->allocate = allocate_scriptable_jp_object;
	np_class->deallocate = IcedTeaScriptableJavaPackageObject::deAllocate;
	np_class->invalidate = IcedTeaScriptableJavaPackageObject::invalidate;
	np_class->hasMethod = IcedTeaScriptableJavaPackageObject::hasMethod;
	np_class->invoke = IcedTeaScriptableJavaPackageObject::invoke;
	np_class->invokeDefault = IcedTeaScriptableJavaPackageObject::invokeDefault;
	np_class->hasProperty = IcedTeaScriptableJavaPackageObject::hasProperty;
	np_class->getProperty = IcedTeaScriptableJavaPackageObject::getProperty;
	np_class->setProperty = IcedTeaScriptableJavaPackageObject::setProperty;
	np_class->removeProperty = IcedTeaScriptableJavaPackageObject::removeProperty;
	np_class->enumerate = IcedTeaScriptableJavaPackageObject::enumerate;
	np_class->construct = IcedTeaScriptableJavaPackageObject::construct;

	scriptable_object = browser_functions.createobject(instance, np_class);
	PLUGIN_DEBUG_3ARG("Returning new scriptable package class: %p from instance %p with name %s\n", scriptable_object, instance, name);

    ((IcedTeaScriptableJavaPackageObject*) scriptable_object)->setPackageName(name);

    IcedTeaPluginUtilities::storeInstanceID(scriptable_object, instance);

	return scriptable_object;
}

std::map<std::string, NPObject*>* IcedTeaScriptableJavaPackageObject::object_map = new std::map<std::string, NPObject*>();

IcedTeaScriptableJavaPackageObject::IcedTeaScriptableJavaPackageObject(NPP instance)
{
    PLUGIN_DEBUG_0ARG("Constructing new scriptable java package object\n");
	this->instance = instance;
	this->package_name = new std::string();
}

IcedTeaScriptableJavaPackageObject::~IcedTeaScriptableJavaPackageObject()
{
    delete this->package_name;
}

void
IcedTeaScriptableJavaPackageObject::setPackageName(const NPUTF8* name)
{
    this->package_name->append(name);
}

std::string
IcedTeaScriptableJavaPackageObject::getPackageName()
{
    return this->package_name->c_str();
}

void
IcedTeaScriptableJavaPackageObject::deAllocate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::deAllocate %p\n", npobj);
}

void
IcedTeaScriptableJavaPackageObject::invalidate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::invalidate %p\n", npobj);
}

bool
IcedTeaScriptableJavaPackageObject::hasMethod(NPObject *npobj, NPIdentifier name)
{
    // Silly caller. Methods are for objects!
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args,
			uint32_t argCount,NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::invoke %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::invokeDefault(NPObject *npobj, const NPVariant *args,
			       uint32_t argCount, NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::invokeDefault %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::hasProperty(NPObject *npobj, NPIdentifier name)
{
	PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaPackageObject::hasProperty %s\n", browser_functions.utf8fromidentifier(name));

	bool hasProperty = false;
	JavaResultData* java_result;
	JavaRequestProcessor* java_request = new JavaRequestProcessor();
    NPP instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj);
    int plugin_instance_id = get_id_from_instance(instance);

	PLUGIN_DEBUG_1ARG("Object package name: \"%s\"\n", ((IcedTeaScriptableJavaPackageObject*) npobj)->getPackageName().c_str());

	// "^java" is always a package
	if (((IcedTeaScriptableJavaPackageObject*) npobj)->getPackageName().length() == 0 &&
	    (  !strcmp(browser_functions.utf8fromidentifier(name), "java") ||
	       !strcmp(browser_functions.utf8fromidentifier(name), "javax")))
	{
	    return true;
	}

	std::string property_name = ((IcedTeaScriptableJavaPackageObject*) npobj)->getPackageName();
	if (property_name.length() > 0)
	    property_name += ".";
	property_name += browser_functions.utf8fromidentifier(name);

	PLUGIN_DEBUG_1ARG("Looking for name \"%s\"\n", property_name.c_str());

	java_result = java_request->hasPackage(plugin_instance_id, property_name);

	if (!java_result->error_occurred && java_result->return_identifier != 0) hasProperty = true;

	// No such package. Do we have a class with that name?
	if (!hasProperty)
	{
		java_result = java_request->findClass(plugin_instance_id, property_name);
	}

	if (java_result->return_identifier != 0) hasProperty = true;

	delete java_request;

	return hasProperty;
}

bool
IcedTeaScriptableJavaPackageObject::getProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{

	PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaPackageObject::getProperty %s\n", browser_functions.utf8fromidentifier(name));

	if (!browser_functions.utf8fromidentifier(name))
	    return false;

	bool isPropertyClass = false;
	JavaResultData* java_result;
	JavaRequestProcessor java_request = JavaRequestProcessor();

	std::string property_name = ((IcedTeaScriptableJavaPackageObject*) npobj)->getPackageName();
	if (property_name.length() > 0)
	    property_name += ".";
	property_name += browser_functions.utf8fromidentifier(name);

	java_result = java_request.findClass(0, property_name);
	isPropertyClass = (java_result->return_identifier == 0);

	//NPIdentifier property = browser_functions.getstringidentifier(property_name.c_str());

	NPObject* obj;

	if (isPropertyClass)
	{
		PLUGIN_DEBUG_0ARG("Returning package object\n");
		obj = IcedTeaScriptablePluginObject::get_scriptable_java_package_object(
                                  IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj),
                                  property_name.c_str());
	}
	else
	{
		PLUGIN_DEBUG_0ARG("Returning Java object\n");
		obj = IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(
		                IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj),
		                *(java_result->return_string), "0", false);
	}

	OBJECT_TO_NPVARIANT(obj, *result);

	return true;
}

bool
IcedTeaScriptableJavaPackageObject::setProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	// Can't be going around setting properties on namespaces.. that's madness!
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::removeProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::removeProperty %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::enumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::enumerate %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaPackageObject::construct(NPObject *npobj, const NPVariant *args, uint32_t argCount,
	           NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaPackageObject::construct %p\n", npobj);
	return false;
}

NPObject*
allocate_scriptable_java_object(NPP npp, NPClass *aClass)
{
    PLUGIN_DEBUG_0ARG("Allocating new scriptable Java object\n");
    return new IcedTeaScriptableJavaObject(npp);
}

NPObject*
IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(NPP instance,
                                    std::string class_id,
                                    std::string instance_id,
                                    bool isArray)
{
    NPObject* scriptable_object;

    std::string obj_key = std::string();
    obj_key += class_id;
    obj_key += ":";
    obj_key += instance_id;

    std::map<std::string, NPObject*>::iterator iterator = object_map->find(obj_key);
    PLUGIN_DEBUG_1ARG("get_scriptable_java_object searching for %s...\n", obj_key.c_str());

    if (iterator != object_map->end())
    {
        scriptable_object = object_map->find(obj_key)->second;
        PLUGIN_DEBUG_2ARG("Found existing match for key %s. Returning %p\n", obj_key.c_str(), scriptable_object);
        browser_functions.retainobject(scriptable_object);
        return scriptable_object;
    }

	NPClass* np_class = new NPClass();
	np_class->structVersion = NP_CLASS_STRUCT_VERSION;
	np_class->allocate = allocate_scriptable_java_object;
	np_class->deallocate = IcedTeaScriptableJavaObject::deAllocate;
	np_class->invalidate = IcedTeaScriptableJavaObject::invalidate;
	np_class->hasMethod = IcedTeaScriptableJavaObject::hasMethod;
	np_class->invoke = IcedTeaScriptableJavaObject::invoke;
	np_class->invokeDefault = IcedTeaScriptableJavaObject::invokeDefault;
	np_class->hasProperty = IcedTeaScriptableJavaObject::hasProperty;
	np_class->getProperty = IcedTeaScriptableJavaObject::getProperty;
	np_class->setProperty = IcedTeaScriptableJavaObject::setProperty;
	np_class->removeProperty = IcedTeaScriptableJavaObject::removeProperty;
	np_class->enumerate = IcedTeaScriptableJavaObject::enumerate;
	np_class->construct = IcedTeaScriptableJavaObject::construct;

	scriptable_object = browser_functions.createobject(instance, np_class);
	browser_functions.retainobject(scriptable_object);
	PLUGIN_DEBUG_3ARG("Constructing new Java Object with classid=%s, instanceid=%s and isArray=%d\n", class_id.c_str(), instance_id.c_str(), isArray);

	((IcedTeaScriptableJavaObject*) scriptable_object)->setClassIdentifier(class_id);
    ((IcedTeaScriptableJavaObject*) scriptable_object)->setIsArray(isArray);

	if (instance_id != "0")
	    ((IcedTeaScriptableJavaObject*) scriptable_object)->setInstanceIdentifier(instance_id);

	IcedTeaPluginUtilities::storeInstanceID(scriptable_object, instance);

	object_map->insert(std::make_pair(obj_key, scriptable_object));

	PLUGIN_DEBUG_2ARG("Inserting into object_map key %s->%p\n", obj_key.c_str(), scriptable_object);
	return scriptable_object;
}

IcedTeaScriptableJavaObject::IcedTeaScriptableJavaObject(NPP instance)
{
	this->instance = instance;
	this->class_id = new std::string();
	this->instance_id = new std::string();
}

IcedTeaScriptableJavaObject::~IcedTeaScriptableJavaObject()
{
	delete this->class_id;
	delete this->instance_id;
}

void
IcedTeaScriptableJavaObject::setClassIdentifier(std::string class_id)
{
	this->class_id->append(class_id);
}

void
IcedTeaScriptableJavaObject::setInstanceIdentifier(std::string instance_id)
{
	this->instance_id->append(instance_id);
}

void
IcedTeaScriptableJavaObject::setIsArray(bool isArray)
{
    this->isObjectArray = isArray;
}

void
IcedTeaScriptableJavaObject::deAllocate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaObject::deAllocate %p\n", npobj);
}

void
IcedTeaScriptableJavaObject::invalidate(NPObject *npobj)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaObject::invalidate %p\n", npobj);
}

bool
IcedTeaScriptableJavaObject::hasMethod(NPObject *npobj, NPIdentifier name)
{
    PLUGIN_DEBUG_2ARG("IcedTeaScriptableJavaObject::hasMethod %s (ival=%d)\n", browser_functions.utf8fromidentifier(name), browser_functions.intfromidentifier(name));
    bool hasMethod = false;

    // If object is an array and requested "method" may be a number, check for it first
    if ( !((IcedTeaScriptableJavaObject*) npobj)->isArray()  ||
         (browser_functions.intfromidentifier(name) < 0))
    {

        if (!browser_functions.utf8fromidentifier(name))
            return false;

        JavaResultData* java_result;
        JavaRequestProcessor java_request = JavaRequestProcessor();

        std::string classId = std::string(((IcedTeaScriptableJavaObject*) npobj)->getClassID());
        std::string methodName = browser_functions.utf8fromidentifier(name);

        java_result = java_request.hasMethod(classId, methodName);
        hasMethod = java_result->return_identifier != 0;
    }

    PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaObject::hasMethod returning %d\n", hasMethod);
    return hasMethod;
}

bool
IcedTeaScriptableJavaObject::javaResultToNPVariant(NPObject *npobj,
                                                   JavaResultData* java_result,
                                                   NPVariant* variant)
{
    JavaRequestProcessor java_request = JavaRequestProcessor();

    if (java_result->return_identifier == 0)
    {
        // VOID/BOOLEAN/NUMBER

        if (*(java_result->return_string) == "void")
        {
            PLUGIN_DEBUG_0ARG("Method call returned void\n");
            VOID_TO_NPVARIANT(*variant);
        } else if (*(java_result->return_string) == "null")
        {
            PLUGIN_DEBUG_0ARG("Method call returned null\n");
            NULL_TO_NPVARIANT(*variant);
        }else if (*(java_result->return_string) == "true")
        {
            PLUGIN_DEBUG_0ARG("Method call returned a boolean (true)\n");
            BOOLEAN_TO_NPVARIANT(true, *variant);
        } else if (*(java_result->return_string) == "false")
        {
            PLUGIN_DEBUG_0ARG("Method call returned a boolean (false)\n");
            BOOLEAN_TO_NPVARIANT(false, *variant);
        } else
        {
            double d = strtod(java_result->return_string->c_str(), NULL);

            // See if it is convertible to int
            if (java_result->return_string->find(".") != std::string::npos ||
                d < -(0x7fffffffL - 1L) ||
                d > 0x7fffffffL)
            {
                PLUGIN_DEBUG_1ARG("Method call returned a double %f\n", d);
                DOUBLE_TO_NPVARIANT(d, *variant);
            } else
            {
                int32_t i = (int32_t) d;
                PLUGIN_DEBUG_1ARG("Method call returned an int %d\n", i);
                INT32_TO_NPVARIANT(i, *variant);
            }
        }
    } else {
        // Else this is a complex java object
        std::string return_obj_instance_id = std::string();
        std::string return_obj_class_id = std::string();
        std::string return_obj_class_name = std::string();
        return_obj_instance_id.append(*(java_result->return_string));

        // Find out the class name first, because string is a special case
        java_result = java_request.getClassName(return_obj_instance_id);

        if (java_result->error_occurred)
        {
            return false;
        }

        return_obj_class_name.append(*(java_result->return_string));

        if (return_obj_class_name == "java.lang.String")
        {
            // String is a special case as NPVariant can handle it directly
            java_result = java_request.getString(return_obj_instance_id);

            if (java_result->error_occurred)
            {
                return false;
            }

            // needs to be on the heap
            NPUTF8* return_str = (NPUTF8*) malloc(sizeof(NPUTF8)*java_result->return_string->size() + 1);
            strcpy(return_str, java_result->return_string->c_str());

            PLUGIN_DEBUG_1ARG("Method call returned a string: \"%s\"\n", return_str);
            STRINGZ_TO_NPVARIANT(return_str, *variant);

            // delete string from java side, as it is no longer needed
            java_request.deleteReference(return_obj_instance_id);
        } else {

            // Else this is a regular class. Reference the class object so
            // we can construct an NPObject with it and the instance
            java_result = java_request.getClassID(return_obj_instance_id);

            if (java_result->error_occurred)
            {
                return false;
            }

            return_obj_class_id.append(*(java_result->return_string));

            NPObject* obj;

            if (return_obj_class_name.find('[') == 0) // array
                obj = IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(
                                IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj),
                                return_obj_class_id, return_obj_instance_id, true);
            else
                obj = IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(
                                                IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj),
                                                return_obj_class_id, return_obj_instance_id, false);

            OBJECT_TO_NPVARIANT(obj, *variant);
        }
    }

    return true;
}

bool
IcedTeaScriptableJavaObject::invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args,
			uint32_t argCount, NPVariant *result)
{
    NPUTF8* method_name = browser_functions.utf8fromidentifier(name);

    // Extract arg type array
    PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaObject::invoke %s. Args follow.\n", method_name);
    for (int i=0; i < argCount; i++)
    {
        IcedTeaPluginUtilities::printNPVariant(args[i]);
    }

    JavaResultData* java_result;
    JavaRequestProcessor java_request = JavaRequestProcessor();

    NPObject* obj;
    std::string instance_id = ((IcedTeaScriptableJavaObject*) npobj)->getInstanceID();
    std::string class_id = ((IcedTeaScriptableJavaObject*) npobj)->getClassID();
    std::string callee;
    std::string source;

    NPP instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj);

    if (instance_id.length() == 0) // Static
    {
        PLUGIN_DEBUG_0ARG("Calling static method\n");
        callee = ((IcedTeaScriptableJavaObject*) npobj)->getClassID();
        java_result = java_request.callStaticMethod(
                        IcedTeaPluginUtilities::getSourceFromInstance(instance),
                        callee, browser_functions.utf8fromidentifier(name), args, argCount);
    } else
    {
        PLUGIN_DEBUG_0ARG("Calling method normally\n");
        callee = ((IcedTeaScriptableJavaObject*) npobj)->getInstanceID();
        java_result = java_request.callMethod(
                        IcedTeaPluginUtilities::getSourceFromInstance(instance),
                        callee, browser_functions.utf8fromidentifier(name), args, argCount);
    }

    if (java_result->error_occurred)
    {
        // error message must be allocated on heap
        char* error_msg = (char*) malloc(java_result->error_msg->length()*sizeof(char));
        browser_functions.setexception(npobj, error_msg);
        return false;
    }

    PLUGIN_DEBUG_0ARG("IcedTeaScriptableJavaObject::invoke converting and returning.\n");
    return IcedTeaScriptableJavaObject::javaResultToNPVariant(npobj, java_result, result);
}

bool
IcedTeaScriptableJavaObject::invokeDefault(NPObject *npobj, const NPVariant *args,
			       uint32_t argCount, NPVariant *result)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaObject::invokeDefault %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaObject::hasProperty(NPObject *npobj, NPIdentifier name)
{
    PLUGIN_DEBUG_2ARG("IcedTeaScriptableJavaObject::hasProperty %s (ival=%d)\n", browser_functions.utf8fromidentifier(name), browser_functions.intfromidentifier(name));
    bool hasProperty = false;

    // If it is an array, only length and indexes are valid
    if (((IcedTeaScriptableJavaObject*) npobj)->isArray())
    {
        if (browser_functions.intfromidentifier(name) >= 0 ||
            !strcmp(browser_functions.utf8fromidentifier(name), "length"))
            hasProperty = true;

    } else
    {

        if (!browser_functions.utf8fromidentifier(name))
            return false;

        if (!strcmp(browser_functions.utf8fromidentifier(name), "Packages"))
        {
            hasProperty = true;
        } else {

            JavaResultData* java_result;
            JavaRequestProcessor java_request = JavaRequestProcessor();

            std::string class_id = std::string(((IcedTeaScriptableJavaObject*) npobj)->getClassID());
            std::string fieldName = browser_functions.utf8fromidentifier(name);

            java_result = java_request.hasField(class_id, fieldName);

            hasProperty = java_result->return_identifier != 0;
        }
    }

	PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaObject::hasProperty returning %d\n", hasProperty);
	return hasProperty;
}

bool
IcedTeaScriptableJavaObject::getProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
    PLUGIN_DEBUG_2ARG("IcedTeaScriptableJavaObject::getProperty %s (ival=%d)\n", browser_functions.utf8fromidentifier(name), browser_functions.intfromidentifier(name));

    bool isPropertyClass = false;
    JavaResultData* java_result;
    JavaRequestProcessor java_request = JavaRequestProcessor();

    NPObject* obj;
    std::string instance_id = ((IcedTeaScriptableJavaObject*) npobj)->getInstanceID();
    std::string class_id = ((IcedTeaScriptableJavaObject*) npobj)->getClassID();
    NPP instance = ((IcedTeaScriptableJavaObject*) npobj)->getInstance();

    if (instance_id.length() > 0) // Could be an array or a simple object
    {
        // If array and requesting length
        if ( ((IcedTeaScriptableJavaObject*) npobj)->isArray() &&
             browser_functions.utf8fromidentifier(name) &&
             !strcmp(browser_functions.utf8fromidentifier(name), "length"))
        {
            java_result = java_request.getArrayLength(instance_id);
        } else if ( ((IcedTeaScriptableJavaObject*) npobj)->isArray() &&
                    browser_functions.intfromidentifier(name) >= 0) // else if array and requesting index
        {

            java_result = java_request.getArrayLength(instance_id);
            if (java_result->error_occurred)
            {
                printf("ERROR: Couldn't fetch array length\n");
                return false;
            }

            int length = atoi(java_result->return_string->c_str());

            // Access beyond size?
            if (browser_functions.intfromidentifier(name) >= length)
            {
                NULL_TO_NPVARIANT(*result);
                return true;
            }

            std::string index = std::string();
            IcedTeaPluginUtilities::itoa(browser_functions.intfromidentifier(name), &index);
            java_result = java_request.getSlot(instance_id, index);

        } else // Everything else
        {
            if (!browser_functions.utf8fromidentifier(name))
                return false;

            if (!strcmp(browser_functions.utf8fromidentifier(name), "Packages"))
            {
                NPObject* pkgObject = IcedTeaScriptablePluginObject::get_scriptable_java_package_object(instance, "");
                OBJECT_TO_NPVARIANT(pkgObject, *result);
                return true;
            }

            java_result = java_request.getField(
                        IcedTeaPluginUtilities::getSourceFromInstance(instance),
                        class_id, instance_id, browser_functions.utf8fromidentifier(name));
        }
    }
    else
    {
        if (!browser_functions.utf8fromidentifier(name))
            return true;

        java_result = java_request.getStaticField(
                                IcedTeaPluginUtilities::getSourceFromInstance(instance),
                                class_id, browser_functions.utf8fromidentifier(name));
    }

    if (java_result->error_occurred)
    {
        return false;
    }

    PLUGIN_DEBUG_0ARG("IcedTeaScriptableJavaObject::getProperty converting and returning.\n");
    return IcedTeaScriptableJavaObject::javaResultToNPVariant(npobj, java_result, result);

}

bool
IcedTeaScriptableJavaObject::setProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
    PLUGIN_DEBUG_2ARG("IcedTeaScriptableJavaObject::setProperty %s (ival=%d) to:\n", browser_functions.utf8fromidentifier(name), browser_functions.intfromidentifier(name));
    IcedTeaPluginUtilities::printNPVariant(*value);

    bool isPropertyClass = false;
    JavaResultData* java_result;
    JavaRequestProcessor java_request = JavaRequestProcessor();

    NPObject* obj;
    std::string instance_id = ((IcedTeaScriptableJavaObject*) npobj)->getInstanceID();
    std::string class_id = ((IcedTeaScriptableJavaObject*) npobj)->getClassID();

    NPP instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj);

    if (instance_id.length() > 0) // Could be an array or a simple object
    {
        // If array
        if ( ((IcedTeaScriptableJavaObject*) npobj)->isArray() &&
             browser_functions.utf8fromidentifier(name) &&
             !strcmp(browser_functions.utf8fromidentifier(name), "length"))
        {
            printf("ERROR: Array length is not a modifiable property\n");
            return false;
        } else if ( ((IcedTeaScriptableJavaObject*) npobj)->isArray() &&
                    browser_functions.intfromidentifier(name) >= 0) // else if array and requesting index
        {

            java_result = java_request.getArrayLength(instance_id);
            if (java_result->error_occurred)
            {
                printf("ERROR: Couldn't fetch array length\n");
                return false;
            }

            int length = atoi(java_result->return_string->c_str());

            // Access beyond size?
            if (browser_functions.intfromidentifier(name) >= length)
            {
                return true;
            }

            std::string index = std::string();
            IcedTeaPluginUtilities::itoa(browser_functions.intfromidentifier(name), &index);
            java_result = java_request.setSlot(instance_id, index, *value);

        } else // Everything else
        {

            java_result = java_request.setField(
                        IcedTeaPluginUtilities::getSourceFromInstance(instance),
                        class_id, instance_id, browser_functions.utf8fromidentifier(name), *value);
        }
    }
    else
    {
        java_result = java_request.setStaticField(
                                IcedTeaPluginUtilities::getSourceFromInstance(instance),
                                class_id, browser_functions.utf8fromidentifier(name), *value);
    }

    if (java_result->error_occurred)
    {
        return false;
    }

    PLUGIN_DEBUG_0ARG("IcedTeaScriptableJavaObject::setProperty returning.\n");
    return true;
}

bool
IcedTeaScriptableJavaObject::removeProperty(NPObject *npobj, NPIdentifier name)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaObject::removeProperty %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaObject::enumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	printf ("** Unimplemented: IcedTeaScriptableJavaObject::enumerate %p\n", npobj);
	return false;
}

bool
IcedTeaScriptableJavaObject::construct(NPObject *npobj, const NPVariant *args, uint32_t argCount,
	           NPVariant *result)
{
    NPUTF8* method_name = "";

    // Extract arg type array
    PLUGIN_DEBUG_1ARG("IcedTeaScriptableJavaObject::construct %s. Args follow.\n", ((IcedTeaScriptableJavaObject*) npobj)->getClassID().c_str());
    for (int i=0; i < argCount; i++)
    {
        IcedTeaPluginUtilities::printNPVariant(args[i]);
    }

    JavaResultData* java_result;
    JavaRequestProcessor java_request = JavaRequestProcessor();

    NPObject* obj;
    std::string class_id = ((IcedTeaScriptableJavaObject*) npobj)->getClassID();
    NPP instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj);

    java_result = java_request.newObject(
                            IcedTeaPluginUtilities::getSourceFromInstance(instance),
                            class_id,
                            args,
                            argCount);

    if (java_result->error_occurred)
    {
        // error message must be allocated on heap
        int length = java_result->error_msg->length();
        char* error_msg = (char*) malloc((length+1)*sizeof(char));
        strcpy(error_msg, java_result->error_msg->c_str());

        browser_functions.setexception(npobj, error_msg);
        return false;
    }

    std::string return_obj_instance_id = std::string();
    std::string return_obj_class_id = class_id;
    return_obj_instance_id.append(*(java_result->return_string));

    obj = IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(
                                IcedTeaPluginUtilities::getInstanceFromMemberPtr(npobj),
                                return_obj_class_id, return_obj_instance_id, false);

    OBJECT_TO_NPVARIANT(obj, *result);

    PLUGIN_DEBUG_0ARG("IcedTeaScriptableJavaObject::construct returning.\n");
    return true;
}
