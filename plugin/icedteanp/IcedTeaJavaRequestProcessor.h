/* IcedTeaJavaRequestProcessor.h

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

#ifndef ICEDTEAJAVAREQUEST_H_
#define ICEDTEAJAVAREQUEST_H_

#include <errno.h>
#include <stdlib.h>
#include <vector>

#include "IcedTeaNPPlugin.h"
#include "IcedTeaPluginUtils.h"
#include "IcedTeaScriptablePluginObject.h"

#define REQUESTTIMEOUT 120

/*
 * This struct holds data specific to a Java operation requested by the plugin
 */
typedef struct java_request
{
    // Instance id  (if applicable)
    int instance;

    // Context id (if applicable)
    int context;

    // request specific data
    std::vector<std::string>* data;

    // source of the request
    std::string* source;

} JavaRequest;

/*
 * This struct holds data specific to a Java operation requested by the plugin
 */
typedef struct java_result_data
{

	// Return identifier (if applicable)
    int return_identifier;

    // Return string (if applicable)
    std::string* return_string;

    // Return wide/mb string (if applicable)
    std::wstring* return_wstring;

    // Error message (if an error occurred)
    std::string* error_msg;

    // Boolean indicating if an error occurred
    bool error_occurred;

} JavaResultData;

class JavaRequestProcessor : BusSubscriber
{
    private:
    	// instance and references are constant throughout this objects
    	// lifecycle
    	int instance;
    	int reference;
    	bool result_ready;
    	JavaResultData* result;

    	/* Post message on bus and wait */
    	void postAndWaitForResponse(std::string message);

    	/* Creates a argument on java-side with appropriate type */
    	int createJavaObjectFromVariant(NPVariant variant);

    	// Call a method, static or otherwise, depending on supplied arg
        JavaResultData* call(std::string source, bool isStatic,
                             std::string objectID, std::string methodName,
                             const NPVariant* args, int numArgs);

        /* Resets the results */
        void resetResult();

    public:
    	JavaRequestProcessor();
    	~JavaRequestProcessor();
    	virtual bool newMessageOnBus(const char* message);

    	/* Increments reference count by 1 */
    	void addReference(std::string object_id);

    	/* Decrements reference count by 1 */
    	void deleteReference(std::string object_id);

    	/* Returns the toString() value, given an object identifier */
    	JavaResultData* getToStringValue(std::string object_id);

    	/* Returns the string, given the identifier */
    	JavaResultData* getString(std::string string_id);

    	/* Returns the field object */
        JavaResultData* getField(std::string source,
                                 std::string classID,
                                 std::string fieldName);

        /* Returns the static field object */
        JavaResultData* getStaticField(std::string source,
                                       std::string classID,
                                       std::string fieldName);

        /* Returns the field id */
        JavaResultData* getFieldID(std::string classID, std::string fieldName);

        /* Returns the static field id */
    	JavaResultData* getStaticFieldID(std::string classID, std::string fieldName);

    	/* Returns the method id */
    	JavaResultData* getMethodID(std::string classID, NPIdentifier methodName,
                                    std::vector<std::string> args);

        /* Returns the static method id */
        JavaResultData* getStaticMethodID(std::string classID, NPIdentifier methodName,
                                     std::vector<std::string> args);

        /* Calls a static method */
        JavaResultData* callStaticMethod(std::string source,
                                         std::string classID,
                                         std::string methodName,
                                         const NPVariant* args, int numArgs);

        /* Calls a method on an instance */
        JavaResultData* callMethod(std::string source,
                                   std::string objectID,
                                   std::string methodName,
								   const NPVariant* args, int numArgs);

        /* Returns the class of the given object */
        JavaResultData* getObjectClass(std::string objectID);

    	/* Creates a new object with choosable constructor */
    	JavaResultData* newObject(std::string source,
                                  std::string classID,
                                  const NPVariant* args, int numArgs);

    	/* Creates a new object when constructor is undetermined */
    	JavaResultData* newObjectWithConstructor(std::string source, std::string classID,
                                  std::string methodID,
                                  std::vector<std::string> args);

    	/* Returns the class ID */
    	JavaResultData* findClass(std::string name);

    	/* Returns the type class name */
    	JavaResultData* getClassName(std::string objectID);

    	/* Returns the type class id */
    	JavaResultData* getClassID(std::string objectID);

    	/* Creates a new string in the Java store */
    	JavaResultData* newString(std::string str);

    	/* Check if package exists */
    	JavaResultData* hasPackage(std::string package_name);

        /* Check if method exists */
        JavaResultData* hasMethod(std::string classID, std::string method_name);

        /* Check if field exists */
        JavaResultData* hasField(std::string classID, std::string method_name);

        /* Check if given object is instance of given class */
        JavaResultData* isInstanceOf(std::string objectID, std::string classID);

        /* Returns the instance ID of the java applet */
        JavaResultData* getAppletObjectInstance(std::string instanceID);
};

#endif /* ICEDTEAJAVAREQUESTPROCESSOR_H_ */
