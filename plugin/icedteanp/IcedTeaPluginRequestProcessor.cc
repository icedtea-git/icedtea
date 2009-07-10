/* IcedTeaPluginRequestProcessor.cc

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

#include "IcedTeaNPPlugin.h"
#include "IcedTeaPluginRequestProcessor.h"

/*
 * This class processes requests made by Java. The requests include pointer
 * information, script execution and variable get/set
 */

/**
 * Given the window pointer, returns the instance associated with it
 *
 * @param member_ptr The pointer key
 * @return The associated instance
 */

NPP
getInstanceFromMemberPtr(void* member_ptr)
{

	NPP instance = NULL;
	PLUGIN_DEBUG_1ARG("getInstanceFromMemberPtr looking for %p\n", member_ptr);

	std::map<void*, NPP>::iterator iterator = instance_map->find(member_ptr);

	if (iterator != instance_map->end())
	{
		instance = instance_map->find(member_ptr)->second;
		PLUGIN_DEBUG_2ARG("getInstanceFromMemberPtr found %p. Instance = %p\n", member_ptr, instance);
	}

	return instance;
}

/**
 * Stores a window pointer <-> instance mapping
 *
 * @param member_ptr The pointer key
 * @param instance The instance to associate with this pointer
 */

void
storeInstanceID(void* member_ptr, NPP instance)
{
	PLUGIN_DEBUG_2ARG("Storing instance %p with key %p\n", instance, member_ptr);
	instance_map->insert(std::make_pair(member_ptr, instance));
}

/**
 * PluginRequestProcessor constructor.
 *
 * Initializes various complex data structures used by the class.
 */

PluginRequestProcessor::PluginRequestProcessor()
{
	this->pendingRequests = new std::map<pthread_t, uintmax_t>();
	instance_map = new std::map<void*, NPP>();
}

/**
 * PluginRequestProcessor destructor.
 *
 * Frees memory used by complex objects.
 */

PluginRequestProcessor::~PluginRequestProcessor()
{
	if (pendingRequests)
		delete pendingRequests;

	if (instance_map)
		delete instance_map;

}

/**
 * Processes plugin (C++ side) requests from the Java side, and internally.
 *
 * @param message The message request to process
 * @return boolean indicating whether the message is serviceable by this object
 */

bool
PluginRequestProcessor::newMessageOnBus(const char* message)
{
	PLUGIN_DEBUG_1ARG("PluginRequestProcessor processing %s\n", message);

	std::string type;
	std::string command;
	int counter = 0;

	std::vector<std::string>* message_parts = IcedTeaPluginUtilities::strSplit(message, " ");

	std::vector<std::string>::iterator the_iterator;
	the_iterator = message_parts->begin();

	IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::newMessageOnBus:", message_parts);

	// Prioritize internal requests
	if (message_parts->at(0) == "internal")
	{
		if (message_parts->at(1) == "SendMember")
		{
			// first item is length, and it is radix 10
			int length = strtol(message_parts->at(3).c_str(), NULL, 10);

			std::vector<std::string*>* send_string_req = new std::vector<std::string*>();
			std::string* member_name = new std::string();

			// pack parent id and member name
			send_string_req->push_back(&(message_parts->at(0)));
			IcedTeaPluginUtilities::getUTF8String(length, 4 /* start at */, message_parts, member_name);
			send_string_req->push_back(member_name);

			// make the internal request
			this->_sendMember(send_string_req);

			// free the memory
			delete send_string_req;
			delete message_parts;

			return true; // request processed
		}
	}

	type = message_parts->at(0);
	command = message_parts->at(2);

	if (type == "instance")
	{
		if (command == "GetWindow")
		{
			// Window can be queried from the main thread only. And this call
			// returns immediately, so we do it in the same thread.
			this->sendWindow(message_parts);
			return true;
		} else if (command == "GetMember")
		{
			this->dispatch(&sendMember, message_parts, NULL);
			return true;
		}
	}

	// If we got here, it means we couldn't process the message. Let the caller know.
	return false;
}

/**
 * Delegates further processing to a new thread.
 *
 * @param func_ptr Pointer to the function to delegate this request to
 * @param message_parts The full message, as a vector of strings
 * @param src The source URL of the request (may be NULL)
 *
 * The functions called by dispatch() are started in a new thread, and provided
 * a "ThreadData" struct. Since timings are not guaranteed, the struct is
 * allocated on the heap and it is upto the called functions to free the
 * associated memory when it is safe to do so.
 */

void PluginRequestProcessor::dispatch(void* func_ptr (void*), std::vector<std::string>* message_parts, std::string* src)
{
	pthread_t thread;
	ThreadData* tdata = new ThreadData();

	IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::dispatch:", message_parts);

	tdata->source = src;
	tdata->message_parts = message_parts;

	pthread_create (&thread, NULL, func_ptr, (void*) tdata);
	uintmax_t start_time = (uintmax_t) time(NULL);
}

/**
 * Sends the window pointer to the Java side.
 *
 * @param message_parts The request message.
 */

void
PluginRequestProcessor::sendWindow(std::vector<std::string>* message_parts)
{
	std::string type;
	std::string command;
	std::string* response;
	std::string* window_ptr_str;
	int id;

	type = message_parts->at(0);
	id = atoi(message_parts->at(1).c_str());
	command = message_parts->at(2);

	NPP instance;
	get_instance_from_id(id, instance);

	static NPObject *window_ptr;
	browser_functions.getvalue(instance, NPNVWindowNPObject, &window_ptr);
	PLUGIN_DEBUG_3ARG("ID=%d, Instance=%p, WindowPTR = %p\n", id, instance, window_ptr);

	window_ptr_str = IcedTeaPluginUtilities::JSIDToString(window_ptr);

	// We need the context 0 for backwards compatibility with the Java side
	response = IcedTeaPluginUtilities::constructMessagePrefix(0);
	*response += " JavaScriptGetWindow ";
	*response += *window_ptr_str;

	plugin_to_java_bus->post(response->c_str());

	delete response;
	delete window_ptr_str;
	delete message_parts;

	// store the instance pointer for future reference
	storeInstanceID(window_ptr, instance);
}

/**
 * Sends request member pointer to the Java side.
 *
 * This is a static function, called in another thread. Since certain data
 * can only be requested from the main thread in Mozilla, this function
 * does whatever it can seperately, and then makes an internal request that
 * causes _sendMember to do the rest of the work.
 *
 * @param tdata A ThreadData structure holding information needed by this function.
 */

void*
sendMember(void* tdata)
{
	// member initialization
	std::vector<std::string>* message_parts;
	std::vector<std::string>* compound_data;
	JavaRequestProcessor* java_request;
	JavaRequest* java_request_data;
	std::string* member_id = new std::string();
	std::string* parent_id = new std::string();
	std::string* member_name_utf = new std::string();
	std::string* internal_request = new std::string();
	int id;

	/** Data extraction **/

	// extract data passed from parent thread
	ThreadData *data;
	data = (ThreadData*) tdata;
	message_parts = data->message_parts;

	// debug printout of parent thread data
	IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::getMember:", message_parts);

	// store info in local variables for easy access
	id = atoi(message_parts->at(1).c_str());
	*parent_id += message_parts->at(3);
	*member_id += message_parts->at(4);

	/** Request data from Java **/

	// create a compound request data structure to pass to getString
	compound_data = new std::vector<std::string>();
	compound_data->push_back(*member_id);

	// make a new request for getString, to get the name of the identifier
	java_request = new JavaRequestProcessor();
	java_request_data = new JavaRequest();
	java_request_data->instance = id;
	java_request_data->data = compound_data;
	java_request_data->source = data->source != NULL ? data->source : new std::string("file://");
	JavaResultData* result = java_request->getString(java_request_data);

	// the result we want is in result_string (assuming there was no error)
	if (result->error_occured)
	{
		printf("Unable to process getMember request. Error occurred: %s\n", result->error_msg);
		goto cleanup;
	}

	/** Make an internal request for the main thread to handle**/
    IcedTeaPluginUtilities::convertStringToUTF8(result->return_string, member_name_utf);

    // Ask main thread to do the send
    *internal_request = "internal SendMember ";
    *internal_request += *parent_id;
    *internal_request += " ";
    *internal_request += *member_name_utf;

    java_to_plugin_bus->post(internal_request->c_str());

	// Now be a good citizen and help keep the heap free of garbage
	cleanup:
	delete data; // thread data that caller allocated for us
	delete java_request; // request object
	delete java_request_data; // request data
	delete member_id; // member id string
	delete compound_data; // compound data object
	delete message_parts; // message_parts vector that was allocated by the caller
	delete internal_request; // delete the string that held the internal request data
}

/**
 * Given the parent id and the member name, sends the member pointer to Java.
 *
 * @param message_parts Vector containing the parent pointer and member name
 */

void
PluginRequestProcessor::_sendMember(std::vector<std::string*>* message_parts)
{
	std::string* member_ptr_str;
	std::string* member;
	std::string* parent;

	NPObject *window_ptr;
	NPVariant member_ptr;
	NPP instance;
	NPIdentifier identifier;
	std::string* response;

	IcedTeaPluginUtilities::printStringPtrVector(" - ", message_parts);

	parent = message_parts->at(0);
	member = message_parts->at(1);

	std::cout << "MEMBER=" << *member << std::endl;

	// Get the corresponding windowId
	identifier = browser_functions.getstringidentifier(member->c_str());

	// Get the window pointer
	window_ptr = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(parent->c_str()));

	// Get the associated instance
	instance = getInstanceFromMemberPtr(window_ptr);

	printf("[2] Instance=%p, window=%p, identifier=%p -- %s::%s\n", instance, window_ptr, identifier, parent->c_str(), member->c_str());

	// Get the NPVariant corresponding to this member
    browser_functions.getproperty(instance, window_ptr, identifier, &member_ptr);

    PLUGIN_DEBUG_4ARG("[3] Instance=%p, window=%p, identifier=%p, variant=%p\n", instance, window_ptr, identifier, &member_ptr);

    if (NPVARIANT_IS_VOID(member_ptr))
    {
        printf("VOID %d\n", member_ptr);
    }
    else if (NPVARIANT_IS_NULL(member_ptr))
    {
        printf("NULL\n", member_ptr);
    }
    else if (NPVARIANT_IS_BOOLEAN(member_ptr))
    {
        printf("BOOL: %d\n", NPVARIANT_TO_BOOLEAN(member_ptr));
    }
    else if (NPVARIANT_IS_INT32(member_ptr))
    {
        printf("INT32: %d\n", NPVARIANT_TO_INT32(member_ptr));
    }
    else if (NPVARIANT_IS_DOUBLE(member_ptr))
    {
        printf("DOUBLE: %f\n", NPVARIANT_TO_DOUBLE(member_ptr));
    }
    else if (NPVARIANT_IS_STRING(member_ptr))
    {
        printf("STRING: %s\n", NPVARIANT_TO_STRING(member_ptr).utf8characters);
    }
    else
    {
        printf("OBJ: %p\n", NPVARIANT_TO_OBJECT(member_ptr));
    }

    member_ptr_str = IcedTeaPluginUtilities::JSIDToString(&member_ptr);

    response = IcedTeaPluginUtilities::constructMessagePrefix(0);
    *response += " JavaScriptGetMember ";
    *response += *member_ptr_str;

	plugin_to_java_bus->post(response->c_str());

	delete member_ptr_str;
	delete response;
}
