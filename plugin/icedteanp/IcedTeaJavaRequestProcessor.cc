/* IcedTeaJavaRequestProcessor.cc

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

#include "IcedTeaJavaRequestProcessor.h"

/*
 * This class processes LiveConnect requests from JavaScript to Java.
 *
 * It sends the requests to Java, gets the return information, and sends it
 * back to the browser/JavaScript
 */

/**
 * Processes return information from JavaSide (return messages of requests)
 *
 * @param message The message request to process
 * @return boolean indicating whether the message is serviceable by this object
 */

bool
JavaRequestProcessor::newMessageOnBus(const char* message)
{

	// Anything we are waiting for _MUST_ have and instance id and reference #
	std::vector<std::string>* message_parts = IcedTeaPluginUtilities::strSplit(message, " ");

	IcedTeaPluginUtilities::printStringVector("JavaRequest::newMessageOnBus:", message_parts);

	if (message_parts->at(0) == "context" && message_parts->at(2) == "reference")
		if (atoi(message_parts->at(1).c_str()) == this->instance && atoi(message_parts->at(3).c_str()) == this->reference)
		{
			// Gather the results

			// Let's get errors out of the way first
			if (message_parts->at(4) == "Error")
			{
				for (int i=5; i < message_parts->size(); i++)
				{
					result->error_msg->append(message_parts->at(i));
					result->error_msg->append(" ");
				}

				printf("Error on Java side: %s\n", result->error_msg->c_str());

				result->error_occurred = true;
				result_ready = true;
			}
			else if (message_parts->at(4) == "GetStringUTFChars" ||
			         message_parts->at(4) == "GetToStringValue")
			{
				// first item is length, and it is radix 10
				int length = strtol(message_parts->at(5).c_str(), NULL, 10);

				IcedTeaPluginUtilities::getUTF8String(length, 6 /* start at */, message_parts, result->return_string);
				result_ready = true;
			}
			else if (message_parts->at(4) == "GetStringChars") // GetStringChars (UTF-16LE/UCS-2)
			{
				// first item is length, and it is radix 10
				int length = strtol(message_parts->at(5).c_str(), NULL, 10);

				IcedTeaPluginUtilities::getUTF16LEString(length, 6 /* start at */, message_parts, result->return_wstring);
				result_ready = true;
			} else if ((message_parts->at(4) == "FindClass") ||
			        (message_parts->at(4) == "GetClassName") ||
			        (message_parts->at(4) == "GetClassID") ||
			        (message_parts->at(4) == "GetMethodID") ||
			        (message_parts->at(4) == "GetStaticMethodID") ||
			        (message_parts->at(4) == "GetObjectClass") ||
                    (message_parts->at(4) == "NewObject") ||
                    (message_parts->at(4) == "NewStringUTF") ||
                    (message_parts->at(4) == "HasPackage") ||
                    (message_parts->at(4) == "HasMethod") ||
                    (message_parts->at(4) == "HasField") ||
                    (message_parts->at(4) == "GetStaticFieldID") ||
                    (message_parts->at(4) == "GetFieldID") ||
                    (message_parts->at(4) == "GetField") ||
                    (message_parts->at(4) == "GetStaticField") ||
                    (message_parts->at(4) == "GetJavaObject") ||
                    (message_parts->at(4) == "IsInstanceOf"))
			{
				result->return_identifier = atoi(message_parts->at(5).c_str());
				result->return_string->append(message_parts->at(5)); // store it as a string as well, for easy access
				result_ready = true;
			}  else if ((message_parts->at(4) == "DeleteLocalRef") ||
		                (message_parts->at(4) == "NewGlobalRef"))
			{
			    result_ready = true; // nothing else to do
			} else if ((message_parts->at(4) == "CallMethod") ||

					   (message_parts->at(4) == "CallStaticMethod"))
			{

			    if (message_parts->at(5) == "literalreturn")
                {
			        // literal returns don't have a corresponding jni id
			        result->return_identifier = 0;
			        result->return_string->append(message_parts->at(6));

                } else
			    {
                    // Else it is a complex object

			        result->return_identifier = atoi(message_parts->at(5).c_str());
			        result->return_string->append(message_parts->at(5)); // store it as a string as well, for easy access
			    }

				result_ready = true;
			}

			delete message_parts;
			return true;
		}

	delete message_parts;
	return false;

}

/**
 * Constructor.
 *
 * Initializes the result data structure (heap)
 */

JavaRequestProcessor::JavaRequestProcessor()
{
    PLUGIN_DEBUG_0ARG("JavaRequestProcessor constructor\n");

	// caller frees this
	result = new JavaResultData();
	result->error_msg = new std::string();
	result->return_identifier = 0;
	result->return_string = new std::string();
	result->return_wstring = new std::wstring();
	result->error_occurred = false;

	result_ready = false;
}

/**
 * Destructor
 *
 * Frees memory used by the result struct
 */

JavaRequestProcessor::~JavaRequestProcessor()
{
    PLUGIN_DEBUG_0ARG("JavaRequestProcessor::~JavaRequestProcessor\n");

	if (result)
	{
		if (result->error_msg)
			delete result->error_msg;

		if (result->return_string)
			delete result->return_string;

		if (result->return_wstring)
			delete result->return_wstring;

		delete result;
	}
}

/**
 * Resets the results
 */
void
JavaRequestProcessor::resetResult()
{
	// caller frees this
	result->error_msg->clear();
	result->return_identifier = 0;
	result->return_string->clear();
	result->return_wstring->clear();
	result->error_occurred = false;

	result_ready = false;
}

void
JavaRequestProcessor::postAndWaitForResponse(std::string message)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    t.tv_sec += REQUESTTIMEOUT; // 1 minute timeout

    // Clear the result
    resetResult();

    java_to_plugin_bus->subscribe(this);
    plugin_to_java_bus->post(message.c_str());

    // Wait for result to be filled in.
	struct timespec curr_t;

    do
    {
    	clock_gettime(CLOCK_REALTIME, &curr_t);

		if (!result_ready && (curr_t.tv_sec < t.tv_sec))
		{
			if (g_main_context_pending(NULL))
				g_main_context_iteration(NULL, false);
			else
				usleep(2000);
		}
		else
			break;

    } while (1);

    if (curr_t.tv_sec >= t.tv_sec)
    {
    	result->error_occurred = true;
    	result->error_msg->append("Error: Timed out when waiting for response");

    	// Report error
    	PLUGIN_DEBUG_1ARG("Error: Timed out when waiting for response to %s\n", message.c_str());
    }

    java_to_plugin_bus->unSubscribe(this);
}

/**
 * Given an object id, fetches the toString() value from Java
 *
 * @param object_id The ID of the object
 * @return A JavaResultData struct containing the result of the request
 */

JavaResultData*
JavaRequestProcessor::getToStringValue(std::string object_id)
{
	std::string message = std::string();

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" GetToStringValue "); // get it in UTF8
    message.append(object_id);

    postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	return result;
}

/**
 * Given a string id, fetches the actual string from Java side
 *
 * @param string_id The ID of the string
 * @return A JavaResultData struct containing the result of the request
 */

JavaResultData*
JavaRequestProcessor::getString(std::string string_id)
{
    std::string message = std::string();

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" GetStringUTFChars "); // get it in UTF8
    message.append(string_id);

    postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	return result;
}

/**
 * Decrements reference count by 1
 *
 * @param object_id The ID of the object
 */

void
JavaRequestProcessor::deleteReference(std::string object_id)
{
    std::string message = std::string();

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" DeleteLocalRef ");
    message.append(object_id);

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();
}

/**
 * Increments reference count by 1
 *
 * @param object_id The ID of the object
 */

void
JavaRequestProcessor::addReference(std::string object_id)
{
    std::string message = std::string();

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" NewGlobalRef ");
    message.append(object_id);

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

}

JavaResultData*
JavaRequestProcessor::findClass(std::string name)
{
    std::string message = std::string();

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" FindClass ");
    message.append(name);

    postAndWaitForResponse(message);

	return result;
}

JavaResultData*
JavaRequestProcessor::getClassName(std::string objectID)
{
    std::string message = std::string();

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" GetClassName ");
    message.append(objectID);

    postAndWaitForResponse(message);

	return result;
}

JavaResultData*
JavaRequestProcessor::getClassID(std::string objectID)
{
    std::string message = std::string();

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);

    message.append(" GetClassID ");
    message.append(objectID);

    postAndWaitForResponse(message);

    return result;
}

JavaResultData*
JavaRequestProcessor::getFieldID(std::string classID, std::string fieldName)
{
	JavaResultData* java_result;
	JavaRequestProcessor* java_request = new JavaRequestProcessor();
	std::string message = std::string();

	java_result = java_request->newString(fieldName);

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
	message.append(" GetFieldID ");
	message.append(classID);
	message.append(" ");
	message.append(java_result->return_string->c_str());

	postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	delete java_request;

	return result;
}

JavaResultData*
JavaRequestProcessor::getStaticFieldID(std::string classID, std::string fieldName)
{
    JavaResultData* java_result;
    JavaRequestProcessor* java_request = new JavaRequestProcessor();
    std::string message = std::string();

    java_result = java_request->newString(fieldName);

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message.append(" GetStaticFieldID ");
    message.append(classID);
    message.append(" ");
    message.append(java_result->return_string->c_str());

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    delete java_request;

    return result;
}

JavaResultData*
JavaRequestProcessor::getField(std::string source,
                               std::string classID,
                               std::string fieldName)
{
    JavaResultData* java_result;
    JavaRequestProcessor* java_request = new JavaRequestProcessor();
    std::string message = std::string();

    java_result = java_request->getFieldID(classID, fieldName);

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, source, &message);
    message.append(" GetField ");
    message.append(classID);
    message.append(" ");
    message.append(java_result->return_string->c_str());

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    delete java_request;

    return result;
}

JavaResultData*
JavaRequestProcessor::getStaticField(std::string classID, std::string source,
                                     std::string fieldName)
{
    JavaResultData* java_result;
    JavaRequestProcessor* java_request = new JavaRequestProcessor();
    std::string message = std::string();

    java_result = java_request->getStaticFieldID(classID, fieldName);

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, source, &message);
    message.append(" GetStaticField ");
    message.append(classID);
    message.append(" ");
    message.append(java_result->return_string->c_str());

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    delete java_request;

    return result;
}

JavaResultData*
JavaRequestProcessor::getMethodID(std::string classID, NPIdentifier methodName,
                                  std::vector<std::string> args)
{
	JavaRequestProcessor* java_request;
	std::string message = std::string();
    std::string* signature;

    signature = new std::string();
    *signature += "(";

    // FIXME: Need to determine how to extract array types and complex java objects
    for (int i=0; i < args.size(); i++)
    {
    	*signature += args[i];
    }

    *signature += ")";

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
	message += " GetMethodID ";
	message += classID;
	message += " ";
	message += browser_functions.utf8fromidentifier(methodName);
	message += " ";
	message += *signature;

	postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();
	delete signature;

	return result;
}

JavaResultData*
JavaRequestProcessor::getStaticMethodID(std::string classID, NPIdentifier methodName,
                                  std::vector<std::string> args)
{
    JavaRequestProcessor* java_request;
    std::string message = std::string();
    std::string* signature;

    signature = new std::string();
    *signature += "(";

    // FIXME: Need to determine how to extract array types and complex java objects
    for (int i=0; i < args.size(); i++)
    {
        *signature += args[i];
    }

    *signature += ")";

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message += " GetStaticMethodID ";
    message += classID;
    message += " ";
    message += browser_functions.utf8fromidentifier(methodName);
    message += " ";
    message += *signature;

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();
    delete signature;

    return result;
}

int
JavaRequestProcessor::createJavaObjectFromVariant(NPVariant variant)
{
	JavaResultData* java_result;

	std::string className;
	std::string jsObjectClassID = std::string();
	std::string jsObjectConstructorID = std::string();

	std::string stringArg = std::string();
	std::vector<std::string> args = std::vector<std::string>();

	JavaRequestProcessor java_request = JavaRequestProcessor();
	bool alreadyCreated = false;

    if (NPVARIANT_IS_VOID(variant))
    {
    	PLUGIN_DEBUG_1ARG("VOID %d\n", variant);
    }
    else if (NPVARIANT_IS_NULL(variant))
    {
    	PLUGIN_DEBUG_1ARG("NULL\n", variant);
    }
    else if (NPVARIANT_IS_BOOLEAN(variant))
    {
    	className = "java.lang.Boolean";

    	if (NPVARIANT_TO_BOOLEAN(variant))
    		stringArg = "true";
    	else
    		stringArg = "false";

    }
    else if (NPVARIANT_IS_INT32(variant))
    {
    	className = "java.lang.Integer";

    	char* valueStr = (char*) malloc(sizeof(char)*32);
    	sprintf(valueStr, "%d", NPVARIANT_TO_INT32(variant));
    	stringArg += valueStr;
    	free(valueStr);
    }
    else if (NPVARIANT_IS_DOUBLE(variant))
    {
    	className = "java.lang.Double";

    	char* valueStr = (char*) malloc(sizeof(char)*1024);
    	sprintf(valueStr, "%d", NPVARIANT_TO_DOUBLE(variant));
    	stringArg += valueStr;
    	free(valueStr);
    }
    else if (NPVARIANT_IS_STRING(variant))
    {
    	className = "java.lang.String";

    	stringArg += NPVARIANT_TO_STRING(variant).utf8characters;
    } else {
    	alreadyCreated = true;
    }

    if (!alreadyCreated) {
		java_result = java_request.findClass(className.c_str());

		// the result we want is in result_string (assuming there was no error)
		if (java_result->error_occurred) {
			printf("Unable to find classid for %s\n", className.c_str());
			return 0;
		}

		jsObjectClassID.append(*(java_result->return_string));
		java_request.resetResult();

		std::string stringClassName = "Ljava/lang/String;";
		args.push_back(stringClassName);

		java_result = java_request.getMethodID(jsObjectClassID,
				      browser_functions.getstringidentifier("<init>"), args);

		// the result we want is in result_string (assuming there was no error)
		if (java_result->error_occurred) {
			printf("Unable to find string constructor for %s\n", className.c_str());
			return 0;
		}

		jsObjectConstructorID.append(*(java_result->return_string));
		java_request.resetResult();

		// We have class id and constructor ID. So we know we can create the
		// object.. now create the string that will be provided as the arg
		java_result = java_request.newString(stringArg);

		if (java_result->error_occurred) {
			printf("Unable to create requested object\n");
			return 0;
		}

		// Create the object
		args.clear();
		std::string arg = std::string();
		arg.append(*(java_result->return_string));
		args.push_back(arg);
		java_result = java_request.newObject("[System]", jsObjectClassID, jsObjectConstructorID, args);

        if (java_result->error_occurred) {
            printf("Unable to create requested object\n");
            return 0;
        }


		return java_result->return_identifier;

	}

    std::string classId = std::string(((IcedTeaScriptableJavaObject*) NPVARIANT_TO_OBJECT(variant))->getClassID());
    std::string instanceId = std::string(((IcedTeaScriptableJavaObject*) NPVARIANT_TO_OBJECT(variant))->getInstanceID());

    if (instanceId.length() == 0)
    	return atoi(classId.c_str());
    else
    	return atoi(instanceId.c_str());
}

JavaResultData*
JavaRequestProcessor::callStaticMethod(std::string source, std::string classID,
                                       std::string methodName,
                                       const NPVariant* args,
                                       int numArgs)
{
    return call(source, true, classID, methodName, args, numArgs);
}

JavaResultData*
JavaRequestProcessor::callMethod(std::string source,
                                 std::string objectID, std::string methodName,
                                 const NPVariant* args, int numArgs)
{
    return call(source, false, objectID, methodName, args, numArgs);
}

JavaResultData*
JavaRequestProcessor::call(std::string source,
                           bool isStatic, std::string objectID,
                           std::string methodName,
                           const NPVariant* args, int numArgs)
{
    std::string message = std::string();
    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, source, &message);

    if (isStatic)
        message += " CallStaticMethod ";
    else
        message += " CallMethod ";

    message += objectID;
    message += " ";
    message += methodName;
    message += " ";

	// First, we need to load the arguments into the java-side table
	for (int i=0; i < numArgs; i++) {
		int objectID = createJavaObjectFromVariant(args[i]);
		if (objectID == 0)
		{
			result->error_occurred = true;
			result->error_msg->append("Unable to create arguments");
			return result;
		}

		char* id = (char*) malloc(sizeof(char)*32);
		sprintf(id, "%d", objectID);
		message += id;
		message += " ";
		free(id);
	}

	postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    return result;
}

JavaResultData*
JavaRequestProcessor::getObjectClass(std::string objectID)
{
    JavaRequestProcessor* java_request;
    std::string message = std::string();

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message += " GetObjectClass ";
    message += objectID;

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    return result;
}

JavaResultData*
JavaRequestProcessor::newObject(std::string source, std::string objectID,
                                std::string methodID,
                                std::vector<std::string> args)
{
	JavaRequestProcessor* java_request;
	std::string message = std::string();

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, source, &message);
	message += " NewObject ";
	message += objectID;
	message += " ";
	message += methodID;
	message += " ";

	for (int i=0; i < args.size(); i++)
	{
		message += args[i];
		message += " ";
	}

	postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	return result;
}

JavaResultData*
JavaRequestProcessor::newString(std::string str)
{
	std::string utf_string = std::string();
	std::string message = std::string();

	IcedTeaPluginUtilities::convertStringToUTF8(&str, &utf_string);

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
	message.append(" NewStringUTF ");
	message.append(utf_string);

	postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	return result;
}

JavaResultData*
JavaRequestProcessor::hasPackage(std::string package_name)
{
	JavaResultData* java_result;
	JavaRequestProcessor* java_request = new JavaRequestProcessor();
	std::string message = std::string();

	java_result = java_request->newString(package_name);

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
	message.append(" HasPackage ");
	message.append(java_result->return_string->c_str());

	postAndWaitForResponse(message);

	IcedTeaPluginUtilities::releaseReference();

	delete java_request;

	return result;
}

JavaResultData*
JavaRequestProcessor::hasMethod(std::string classID, std::string method_name)
{
    JavaResultData* java_result;
    JavaRequestProcessor* java_request = new JavaRequestProcessor();
    std::string message = std::string();

    java_result = java_request->newString(method_name);

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message.append(" HasMethod ");
    message.append(classID);
    message.append(" ");
    message.append(java_result->return_string->c_str());

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    delete java_request;

    return result;
}

JavaResultData*
JavaRequestProcessor::hasField(std::string classID, std::string method_name)
{
    JavaResultData* java_result;
    JavaRequestProcessor java_request = JavaRequestProcessor();
    std::string message = std::string();

    java_result = java_request.newString(method_name);

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message.append(" HasField ");
    message.append(classID);
    message.append(" ");
    message.append(java_result->return_string->c_str());

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    return result;
}

JavaResultData*
JavaRequestProcessor::isInstanceOf(std::string objectID, std::string classID)
{
    std::string message = std::string();

    this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
    this->reference = IcedTeaPluginUtilities::getReference();

    IcedTeaPluginUtilities::constructMessagePrefix(0, reference, &message);
    message.append(" IsInstanceOf ");
    message.append(objectID);
    message.append(" ");
    message.append(classID);

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    return result;
}

JavaResultData*
JavaRequestProcessor::getAppletObjectInstance(std::string instanceID)
{
    std::string message = std::string();
    std::string ref_str = std::string();

    this->instance = 0;
    this->reference = IcedTeaPluginUtilities::getReference();
    IcedTeaPluginUtilities::itoa(reference, &ref_str);

    message = "instance ";
    message += instanceID;
    message += " reference ";
    message += ref_str;
    message += " GetJavaObject";

    postAndWaitForResponse(message);

    IcedTeaPluginUtilities::releaseReference();

    return result;
}
