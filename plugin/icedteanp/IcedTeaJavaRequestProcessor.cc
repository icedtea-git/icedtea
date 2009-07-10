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

			// GetStringUTFChars
			if (message_parts->at(4) == "GetStringUTFChars")
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
	// caller frees this
	result = new JavaResultData();
	result->error_msg = new std::string();
	result->return_string = new std::string();
	result->return_wstring = new std::wstring();
	result->error_occured = false;

	result_ready = false;
}

/**
 * Destructor
 *
 * Frees memory used by the result struct
 */

JavaRequestProcessor::~JavaRequestProcessor()
{
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
 * Given a string id, fetches the actual string from Java side
 *
 * @param request_data The JavaRequest struct containing request relevant information
 * @return A JavaResultData struct containing the result of the request
 */

JavaResultData*
JavaRequestProcessor::getString(JavaRequest* request_data)
{
	std::string string_id;
	std::string* message;

	this->instance = 0; // context is always 0 (needed for java-side backwards compat.)
	this->reference = IcedTeaPluginUtilities::getReference();

	string_id = request_data->data->at(0);

	message = IcedTeaPluginUtilities::constructMessagePrefix(0, reference);

    message->append(" GetStringUTFChars "); // get it in UTF8
    message->append(string_id);

    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    t.tv_sec += 60; // 1 minute timeout

    result_ready = false;
    java_to_plugin_bus->subscribe(this);
    plugin_to_java_bus->post(message->c_str());

    // Wait for result to be filled in.
	struct timespec curr_t;

    do
    {
    	clock_gettime(CLOCK_REALTIME, &curr_t);
        bool timedout = false;

		if (!result_ready && (curr_t.tv_sec < t.tv_sec))
			sleep(1);
		else
			break;

    } while (1);

    if (curr_t.tv_sec >= t.tv_sec)
    {
    	// Report error
    	PLUGIN_DEBUG_1ARG("Error: Timed out when waiting for response to %s\n", message->c_str());
    }

    java_to_plugin_bus->unSubscribe(this);
	IcedTeaPluginUtilities::releaseReference();

    delete message;

	return result;
}
