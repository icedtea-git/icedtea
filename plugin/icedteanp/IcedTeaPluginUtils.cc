/* IcedTeaPluginUtils.cc

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
#include "IcedTeaPluginUtils.h"

/**
 * Misc. utility functions used by the plugin
 */

/***********************************************
 * Begin IcedTeaPluginUtilities implementation *
************************************************/

// Initialize static variables
int IcedTeaPluginUtilities::reference = 0;
pthread_mutex_t IcedTeaPluginUtilities::reference_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Given a context number, constructs a message prefix to send to Java
 *
 * @param context The context of the request
 * @return The string prefix (allocated on heap)
 */

std::string*
IcedTeaPluginUtilities::constructMessagePrefix(int context)
{
	std::string* result = new std::string();
	char* context_str = itoa(context);

	*result += "context ";
	result->append(context_str);
	*result += " reference -1";

	free(context_str);

	return result;
}

/**
 * Given a context number, and reference number, constructs a message prefix to
 * send to Java
 *
 * @param context The context of the request
 * @param rerefence The reference number of the request
 * @return The string prefix (allocated on heap)
 */

std::string*
IcedTeaPluginUtilities::constructMessagePrefix(int context, int reference)
{
    // Until security is implemented, use file:// source for _everything_

	/*std::string* result = new std::string();
	char* context_str = itoa(context);
	char* reference_str = itoa(reference);

	*result += "context ";
	result->append(context_str);
	*result += " reference ";
	result->append(reference_str);

	free(context_str);
	free(reference_str);

	return result;*/

    return IcedTeaPluginUtilities::constructMessagePrefix(context, reference, "file://");
}

/**
 * Given a context number, reference number, and source location, constructs
 * a message prefix to send to Java
 *
 * @param context The context of the request
 * @param rerefence The reference number of the request
 * @param address The address for the script that made the request
 * @return The string prefix (allocated on heap)
 */

std::string*
IcedTeaPluginUtilities::constructMessagePrefix(int context, int reference,
		                                       const char* address)
{
	std::string* result = new std::string();
	char* context_str = itoa(context);
	char* reference_str = itoa(reference);

	*result += "context ";
	result->append(context_str);
	*result += " reference ";
	result->append(reference_str);
	*result += " src ";
	result->append(address);

	free(context_str);
	free(reference_str);

	return result;
}

/**
 * Returns a string representation of a void pointer
 *
 * @param id The pointer
 * @return The string representation (Allocated on heap)
 */

std::string*
IcedTeaPluginUtilities::JSIDToString(void* id)
{

	std::string* result = new std::string();
	char* id_str = (char*) malloc(sizeof(char)*20); // max = long long = 8446744073709551615 == 19 chars

	if (sizeof(void*) == sizeof(long long))
	{
		sprintf(id_str, "%llu", id);
	}
	else
	{
		sprintf(id_str, "%lu", id); // else use long
	}

	*result += id_str;

	PLUGIN_DEBUG_2ARG("Converting pointer %p to %s\n", id, id_str);
	free(id_str);

	return result;
}

/**
 * Returns a void pointer from a string representation
 *
 * @param id_str The string representation
 * @return The pointer
 */

void*
IcedTeaPluginUtilities::stringToJSID(std::string id_str)
{
	void* ptr;
	if (sizeof(void*) == sizeof(long long))
	{
		PLUGIN_DEBUG_2ARG("Casting (long long) \"%s\" -- %llu\n", id_str.c_str(), strtoull(id_str.c_str(), NULL, 0));
		ptr = reinterpret_cast <void*> ((unsigned long long) strtoull(id_str.c_str(), NULL, 0));
	} else
	{
		PLUGIN_DEBUG_2ARG("Casting (long) \"%s\" -- %lu\n", id_str.c_str(), strtoul(id_str.c_str(), NULL, 0));
		ptr = reinterpret_cast <void*> ((unsigned long)  strtoul(id_str.c_str(), NULL, 0));
	}

	PLUGIN_DEBUG_1ARG("Casted: %p\n", ptr);

	return ptr;
}

/**
 * Increments the global reference number and returns it.
 *
 * This function is thread-safe.
 */
int
IcedTeaPluginUtilities::getReference()
{
	pthread_mutex_lock(&reference_mutex);
	reference++;
	pthread_mutex_unlock(&reference_mutex);

	return reference;
}

/**
 * Decrements the global reference number.
 *
 * This function is thread-safe.
 */
void
IcedTeaPluginUtilities::releaseReference()
{
	pthread_mutex_lock(&reference_mutex);
	reference--;
	pthread_mutex_unlock(&reference_mutex);
}

/**
 * Converts integer to char*
 *
 * @param i The integer to convert to ascii
 * @return The converted string (allocated on heap)
 */
char*
IcedTeaPluginUtilities::itoa(int i)
{
	// largest possible integer is 10 digits long
	char* result = (char*) malloc(sizeof(char)*11);
	sprintf(result, "%d", i);

	return result;
}

/**
 * Frees memory from a string* vector
 *
 * The vector deconstructor will only delete string pointers upon being
 * called. This function frees the associated string memory as well.
 *
 * @param v The vector whose strings are to be freed
 */
void
IcedTeaPluginUtilities::freeStringPtrVector(std::vector<std::string*>* v)
{
	if (v)
	{
		for (int i=0; i < v->size(); i++)
			delete v->at(i);

		delete v;
	}

}

/**
 * Given a string, splits it on the given delimiters.
 *
 * @param str The string to split
 * @param The delimiters to split on
 * @return A string vector containing the aplit components
 */

std::vector<std::string>*
IcedTeaPluginUtilities::strSplit(const char* str, const char* delim)
{
	std::vector<std::string>* v = new std::vector<std::string>();
	char* copy;

	// Tokening is done on a copy
	copy = (char*) malloc (sizeof(char)*strlen(str) + 1);
	strcpy(copy, str);

	char* tok_ptr;
	tok_ptr = strtok (copy, delim);

	while (tok_ptr != NULL)
	{
		std::string* s = new std::string();
		s->append(tok_ptr);
		v->push_back(*s);
		tok_ptr = strtok (NULL, " ");
	}

	return v;
}

/**
 * Given a unicode byte array, converts it to a UTF8 string
 *
 * The actual contents in the array may be surrounded by other data.
 *
 * e.g. with length 5, begin = 3,
 * unicode_byte_array = "37 28 5 48 45 4c 4c 4f 9e 47":
 *
 * We'd start at 3 i.e. "48" and go on for 5 i.e. upto and including "4f".
 * So we convert "48 45 4c 4c 4f" which is "hello"
 *
 * @param length The length of the string
 * @param begin Where in the array to begin conversion
 * @param result_unicode_str The return variable in which the
 *        converted string is placed
 */

void
IcedTeaPluginUtilities::getUTF8String(int length, int begin, std::vector<std::string>* unicode_byte_array, std::string* result_unicode_str)
{
	result_unicode_str->clear();
	for (int i = begin; i < begin+length; i++)
	    result_unicode_str->push_back((char) strtol(unicode_byte_array->at(i).c_str(), NULL, 16));

	PLUGIN_DEBUG_2ARG("Converted UTF-8 string: %s. Length=%d\n", result_unicode_str->c_str(), result_unicode_str->length());
}

/**
 * Given a UTF8 string, converts it to a space delimited string of hex characters
 *
 * The first element in the return array is the length of the string
 *
 * e.g. "hello" would convert to: "5 48 45 4c 4c 4f"
 *
 * @param str The string to convert
 * @param urt_str The result
 */

void
IcedTeaPluginUtilities::convertStringToUTF8(std::string* str, std::string* utf_str)
{
	std::ostringstream ostream;

	char* length = itoa(str->length());
	ostream << length;
	free(length);

	// UTF-8 characters are 4-bytes max + space + '\0'
	char* hex_value = (char*) malloc(sizeof(char)*10);

	for (int i = 0; i < str->length(); i++)
	{
		sprintf(hex_value, " %x", (*str)[i]);
		ostream << hex_value;
	}

	utf_str->clear();
	*utf_str = ostream.str();

	free(hex_value);
	PLUGIN_DEBUG_2ARG("Converted %s to UTF-8 string %s\n", str->c_str(), utf_str->c_str());
}

/**
 * Given a unicode byte array, converts it to a UTF16LE/UCS-2 string
 *
 * This works in a manner similar to getUTF8String, except that it reads 2
 * slots for each byte.
 *
 * @param length The length of the string
 * @param begin Where in the array to begin conversion
 * @param result_unicode_str The return variable in which the
 *        converted string is placed
 */
void
IcedTeaPluginUtilities::getUTF16LEString(int length, int begin, std::vector<std::string>* unicode_byte_array, std::wstring* result_unicode_str)
{

	wchar_t c;

	if (plugin_debug) printf("Converted UTF-16LE string: ");

	result_unicode_str->clear();
	for (int i = begin; i < begin+length; i+=2)
	{
		int low = strtol(unicode_byte_array->at(i).c_str(), NULL, 16);
		int high = strtol(unicode_byte_array->at(i+1).c_str(), NULL, 16);

        c = ((high << 8) | low);

        if ((c >= 'a' && c <= 'z') ||
        	(c >= 'A' && c <= 'Z') ||
        	(c >= '0' && c <= '9'))
        {
        	if (plugin_debug) printf("%c", c);
        }

        result_unicode_str->push_back(c);
	}

	// not routing via debug print macros due to wide-string issues
	if (plugin_debug) printf(". Length=%d\n", result_unicode_str->length());
}

/*
 * Prints the given string vector (if debug is true)
 *
 * @param prefix The prefix to print before printing the vector contents
 * @param cv The string vector whose contents are to be printed
 */
void
IcedTeaPluginUtilities::printStringVector(const char* prefix, std::vector<std::string>* str_vector)
{
	std::string* str = new std::string();
	*str += "{ ";
	for (int i=0; i < str_vector->size(); i++)
	{
		*str += str_vector->at(i);

		if (i != str_vector->size() - 1)
			*str += ", ";
	}

	*str += " }";

	PLUGIN_DEBUG_2ARG("%s %s\n", prefix, str->c_str());

	delete str;
}

/*
 * Similar to printStringVector, but takes a vector of string pointers instead
 *
 * @param prefix The prefix to print before printing the vector contents
 * @param cv The string* vector whose contents are to be printed
 */

void
IcedTeaPluginUtilities::printStringPtrVector(const char* prefix, std::vector<std::string*>* str_ptr_vector)
{
	std::string* str = new std::string();
	*str += "{ ";
	for (int i=0; i < str_ptr_vector->size(); i++)
	{
		*str += *(str_ptr_vector->at(i));

		if (i != str_ptr_vector->size() - 1)
			*str += ", ";
	}

	*str += " }";

	PLUGIN_DEBUG_2ARG("%s %s\n", prefix, str->c_str());

	delete str;
}

std::string*
IcedTeaPluginUtilities::variantToClassName(NPVariant variant)
{

	std::string* java_type = new std::string();

	if (NPVARIANT_IS_VOID(variant))
	{
		*java_type += "V";
	} else if (NPVARIANT_IS_BOOLEAN(variant))
	{
		*java_type += "Z";
	} else if (NPVARIANT_IS_INT32(variant))
	{
		*java_type += "I";
	} else if (NPVARIANT_IS_DOUBLE(variant))
	{
		*java_type += "D";
	} else if (NPVARIANT_IS_STRING(variant))
	{
		*java_type += "Ljava/lang/String;";
	} else if (NPVARIANT_IS_OBJECT(variant))
	{
		printf("** Unimplemented: IcedTeaPluginUtilities::variantToClassName(variant type=obj)\n");
	} else if (NPVARIANT_IS_NULL(variant))
	{
		printf("** Unimplemented: IcedTeaPluginUtilities::variantToClassName(variant type=null)\n");
	} else
	{
		printf("** Unimplemented: IcedTeaPluginUtilities::variantToClassName(variant type=unknown)\n");
	}

	return java_type;
}

void
IcedTeaPluginUtilities::printNPVariant(NPVariant variant)
{
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
    	PLUGIN_DEBUG_1ARG("BOOL: %d\n", NPVARIANT_TO_BOOLEAN(variant));
    }
    else if (NPVARIANT_IS_INT32(variant))
    {
    	PLUGIN_DEBUG_1ARG("INT32: %d\n", NPVARIANT_TO_INT32(variant));
    }
    else if (NPVARIANT_IS_DOUBLE(variant))
    {
    	PLUGIN_DEBUG_1ARG("DOUBLE: %f\n", NPVARIANT_TO_DOUBLE(variant));
    }
    else if (NPVARIANT_IS_STRING(variant))
    {
    	PLUGIN_DEBUG_1ARG("STRING: %s\n", NPVARIANT_TO_STRING(variant).utf8characters);
    }
    else
    {
    	PLUGIN_DEBUG_1ARG("OBJ: %p\n", NPVARIANT_TO_OBJECT(variant));
    }
}

std::string*
IcedTeaPluginUtilities::NPVariantToString(NPVariant variant)
{
	char* str = (char*) malloc(sizeof(char)*32); // enough for everything except string

    if (NPVARIANT_IS_VOID(variant))
    {
        sprintf(str, "%p", variant);
    }
    else if (NPVARIANT_IS_NULL(variant))
    {
    	sprintf(str, "NULL");
    }
    else if (NPVARIANT_IS_BOOLEAN(variant))
    {
    	if (NPVARIANT_TO_BOOLEAN(variant))
    		sprintf(str, "true");
    	else
    		sprintf(str, "false");
    }
    else if (NPVARIANT_IS_INT32(variant))
    {
    	sprintf(str, "%d", NPVARIANT_TO_INT32(variant));
    }
    else if (NPVARIANT_IS_DOUBLE(variant))
    {
    	sprintf(str, "%f", NPVARIANT_TO_DOUBLE(variant));;
    }
    else if (NPVARIANT_IS_STRING(variant))
    {
    	free(str);
    	str = (char*) malloc(sizeof(char)*NPVARIANT_TO_STRING(variant).utf8length);
    	sprintf(str, "%s", NPVARIANT_TO_STRING(variant).utf8characters);
    }
    else
    {
        sprintf(str, "[Object %p]", variant);
    }

    std::string* ret = new std::string(str);
    free(str);

    return ret;
}

/******************************************
 * Begin JavaMessageSender implementation *
 ******************************************
 *
 * This implementation is very simple and is therefore folded into this file
 * rather than a new one.
 */

/**
 * Sends to the Java side
 *
 * @param message The message to send.
 * @param returns whether the message was consumable (always true)
 */

bool
JavaMessageSender::newMessageOnBus(const char* message)
{
	char* msg = (char*) malloc(sizeof(char)*strlen(message) + 1);
	strcpy(msg, message);
	plugin_send_message_to_appletviewer(msg);

	free(msg);
	msg = NULL;

	// Always successful
	return true;
}

/***********************************
 * Begin MessageBus implementation *
 ***********************************/

/**
 * Constructor.
 *
 * Initializes the mutexes needed by the other functions.
 */
MessageBus::MessageBus()
{
	int ret;

	ret = pthread_mutex_init(&subscriber_mutex, NULL);

	if(ret)
		PLUGIN_DEBUG_1ARG("Error: Unable to initialize subscriber mutex: %d\n", ret);

	ret = pthread_mutex_init(&msg_queue_mutex, NULL);
	if(ret)
		PLUGIN_DEBUG_1ARG("Error: Unable to initialize message queue mutex: %d\n", ret);

	PLUGIN_DEBUG_2ARG("Mutexs %p and %p initialized\n", &subscriber_mutex, &msg_queue_mutex);
}

/**
 * Destructor.
 *
 * Destroy the mutexes initialized by the constructor.
 */

MessageBus::~MessageBus()
{
    PLUGIN_DEBUG_0ARG("MessageBus::~MessageBus\n");

	int ret;

	ret = pthread_mutex_destroy(&subscriber_mutex);
	if(ret)
		PLUGIN_DEBUG_1ARG("Error: Unable to destroy subscriber mutex: %d\n", ret);

	ret = pthread_mutex_destroy(&msg_queue_mutex);
	if(ret)
			PLUGIN_DEBUG_1ARG("Error: Unable to destroy message queue mutex: %d\n", ret);
}

/**
 * Adds the given BusSubscriber as a subscriber to self
 *
 * @param b The BusSubscriber to subscribe
 */
void
MessageBus::subscribe(BusSubscriber* b)
{
    // Applets may initialize in parallel. So lock before pushing.

	PLUGIN_DEBUG_2ARG("Subscribing %p to bus %p\n", b, this);
    pthread_mutex_lock(&subscriber_mutex);
    subscribers.push_back(b);
    pthread_mutex_unlock(&subscriber_mutex);
}

/**
 * Removes the given BusSubscriber from the subscriber list
 *
 * @param b The BusSubscriber to ubsubscribe
 */
void
MessageBus::unSubscribe(BusSubscriber* b)
{
    // Applets may initialize in parallel. So lock before pushing.

	PLUGIN_DEBUG_2ARG("Un-subscribing %p from bus %p\n", b, this);
    pthread_mutex_lock(&subscriber_mutex);
    subscribers.remove(b);
    pthread_mutex_unlock(&subscriber_mutex);
}

/**
 * Notifies all subscribers with the given message
 *
 * @param message The message to send to the subscribers
 */
void
MessageBus::post(const char* message)
{
	char* msg = (char*) malloc(sizeof(char)*strlen(message) + 1);
	bool message_consumed = false;

	// consumer frees this memory
	strcpy(msg, message);

	PLUGIN_DEBUG_1ARG("Trying to lock %p...\n", &msg_queue_mutex);
    pthread_mutex_lock(&msg_queue_mutex);

    PLUGIN_DEBUG_1ARG("Message %s received on bus. Notifying subscribers.\n", msg);

    std::list<BusSubscriber*>::const_iterator i;
    for( i = subscribers.begin(); i != subscribers.end() && !message_consumed; ++i ) {
    	PLUGIN_DEBUG_2ARG("Notifying subscriber %p of %s\n", *i, msg);
    	message_consumed = ((BusSubscriber*) *i)->newMessageOnBus(msg);
    }

    pthread_mutex_unlock(&msg_queue_mutex);

    if (!message_consumed)
    	PLUGIN_DEBUG_1ARG("Warning: No consumer found for message %s\n", msg);

    PLUGIN_DEBUG_1ARG("%p unlocked...\n", &msg_queue_mutex);
}
