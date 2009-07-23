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

// Initialize static members used by the queue processing framework
pthread_mutex_t message_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t syn_write_mutex = PTHREAD_MUTEX_INITIALIZER;
std::vector< std::vector<std::string>* >* message_queue = new std::vector< std::vector<std::string>* >();

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

    internal_req_ref_counter = 0;
}

/**
 * PluginRequestProcessor destructor.
 *
 * Frees memory used by complex objects.
 */

PluginRequestProcessor::~PluginRequestProcessor()
{
    PLUGIN_DEBUG_0ARG("PluginRequestProcessor::~PluginRequestProcessor\n");

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
        } else if (command == "GetMember" ||
                   command == "SetMember" ||
                   command == "ToString")
        {

        	// Update queue synchronously
        	pthread_mutex_lock(&message_queue_mutex);
            message_queue->push_back(message_parts);
            pthread_mutex_unlock(&message_queue_mutex);

            return true;
        }

    }

    delete message_parts;

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
    int tries = 0;
    ThreadData* tdata = new ThreadData();

    IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::dispatch:", message_parts);

    tdata->source = src;
    tdata->message_parts = message_parts;

    printf("Threads MAX=%ld, Thread data=%p\n", sysconf(_SC_THREAD_THREADS_MAX), tdata);
    while (pthread_create (&thread, NULL, func_ptr, (void*) tdata) == 11 && tries++ < 100)
    {
        printf("Couldn't create thread. Sleeping and then retrying. TC=%d\n", thread_count);
        usleep(1000000);
    }
    uintmax_t start_time = (uintmax_t) time(NULL);
    pthread_mutex_lock(&tc_mutex);
    thread_count++;
    pthread_mutex_unlock(&tc_mutex);

    PLUGIN_DEBUG_2ARG("pthread %p created. Thread count=%d\n", thread, thread_count);
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
    static NPObject* window_ptr;
    int id;

    type = message_parts->at(0);
    id = atoi(message_parts->at(1).c_str());
    command = message_parts->at(2);

    NPP instance;
    get_instance_from_id(id, instance);

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
 * Sends the string value of the requested variable
 *
 * @param message_parts The request message.
 */
void
PluginRequestProcessor::sendString(std::vector<std::string>* message_parts)
{
    std::string variant_ptr;
    NPVariant* variant;
    std::string* variant_string;
    std::string* variant_string_id;
    JavaRequestProcessor* java_request;
    JavaResultData* java_result;
    std::string* response;
    int instance;

    instance = atoi(message_parts->at(1).c_str());
    variant_ptr = message_parts->at(3);

    variant = (NPVariant*) IcedTeaPluginUtilities::stringToJSID(variant_ptr);
    variant_string = IcedTeaPluginUtilities::NPVariantToString(*variant);

    java_request = new JavaRequestProcessor();
    java_result = java_request->newString(*variant_string);

    if (java_result->error_occured)
    {
        printf("Unable to process NewString request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    variant_string_id = java_result->return_string;

    // We need the context 0 for backwards compatibility with the Java side
    response = IcedTeaPluginUtilities::constructMessagePrefix(instance);
    *response += " JavaScriptToString ";
    *response += *variant_string_id;

    plugin_to_java_bus->post(response->c_str());

    cleanup:
    delete java_request;
    delete response;
    delete variant_string;
    delete message_parts;

    pthread_mutex_lock(&tc_mutex);
    thread_count--;
    pthread_mutex_unlock(&tc_mutex);
}

/**
 * Sets variable to given value
 *
 * @param message_parts The request message.
 */

void
PluginRequestProcessor::setMember(std::vector<std::string>* message_parts)
{
    std::string valueID;
    std::string propertyNameID;
    std::string* property_name = new std::string();
    std::string* value = new std::string();
    std::string* type = new std::string();
    std::string* value_variant_ptr_str;
    std::vector<std::string*>* internal_request_params = new std::vector<std::string*>();

    NPObject* member;
    nsCOMPtr<nsIRunnable> event;
    ResultData* rdata = new ResultData();

    JavaRequestProcessor* java_request;
    JavaResultData* java_result;

    IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::_setMember - ", message_parts);

    member = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(message_parts->at(3)));
    propertyNameID = message_parts->at(4);
    valueID = message_parts->at(5);

    java_request = new JavaRequestProcessor();
    java_result = java_request->getString(propertyNameID);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to get member name for setMember. Error occurred: %s\n", java_result->error_msg);
        goto cleanup;
    }

    // Copy into local variable before disposing the object
    property_name->append(*(java_result->return_string));
    delete java_request;

    // Based on the value ID, find the type and string value
    // FIXME: Complex java objects not yet peered

    java_request = new JavaRequestProcessor();
    java_result = java_request->getClassName(valueID);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to get class name for setMember. Error occurred: %s\n", java_result->error_msg);
        goto cleanup;
    }

    // Copy into local variable before disposing the object
    type->append(*(java_result->return_string));
    delete java_request;

    java_request = new JavaRequestProcessor();
    java_result = java_request->getToStringValue(valueID);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to get value for setMember. Error occurred: %s\n", java_result->error_msg);
        goto cleanup;
    }

    value->append(*(java_result->return_string));

    internal_request_params->push_back(IcedTeaPluginUtilities::JSIDToString(member));
    internal_request_params->push_back(property_name);
    internal_request_params->push_back(type);
    internal_request_params->push_back(value);

    rdata->result_ready = false;
    event = new IcedTeaRunnableMethod(&_setMember, (void*) internal_request_params, rdata);
    NS_DispatchToMainThread(event, 0);

    cleanup:
    delete message_parts;
    delete java_request;

    // property_name, type and value are deleted by _setMember
    pthread_mutex_lock(&tc_mutex);
    thread_count--;
    pthread_mutex_unlock(&tc_mutex);
}

void
convertToNPVariant(std::string value, std::string type, NPVariant* result_variant)
{
    if (type == "java.lang.Byte" ||
        type == "java.lang.Char" ||
        type == "java.lang.Short" ||
        type == "java.lang.Integer") {
        int i = atoi(value.c_str());
        INT32_TO_NPVARIANT(i, *result_variant);
    } else if (type == "java.lang.Long" ||
               type == "java.lang.Double" ||
               type == "java.lang.Float")
    {
        double d = atof(value.c_str());
        DOUBLE_TO_NPVARIANT(d, *result_variant);
    } else if (type == "java.lang.Boolean")
    {
        bool b = (value == "true");
        BOOLEAN_TO_NPVARIANT(b, *result_variant);
    } else if (type == "java.lang.String")
    {
        STRINGZ_TO_NPVARIANT(value.c_str(), *result_variant);
    } else if (type.substr(0,1) == "[")
    {
        // FIXME: Set up object peering
    }
}

/**
 * Sends request member pointer to the Java side.
 *
 * This is a static function, called in another thread. Since certain data
 * can only be requested from the main thread in Mozilla, this function
 * does whatever it can seperately, and then makes an internal request that
 * causes _sendMember to do the rest of the work.
 *
 * @param message_parts The request message
 */

void
PluginRequestProcessor::sendMember(std::vector<std::string>* message_parts)
{
    // member initialization
    std::vector<std::string>* args;
    JavaRequestProcessor* java_request;
    JavaResultData* java_result;
    ResultData* member_data;
    std::string* member_id = new std::string();
    std::string* parent_id = new std::string();
    std::string* jsObjectClassID = new std::string();
    std::string* jsObjectConstructorID = new std::string();
    std::string* response = new std::string();
    nsCOMPtr<nsIRunnable> event;

    std::vector<std::string*>* internal_request_params = new std::vector<std::string*>();
    int method_id;
    int instance;
    long reference;

    // debug printout of parent thread data
    IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::getMember:", message_parts);

    // store info in local variables for easy access
    instance = atoi(message_parts->at(1).c_str());
    *parent_id += message_parts->at(3);
    *member_id += message_parts->at(4);

    /** Request data from Java **/

    // make a new request for getString, to get the name of the identifier
    java_request = new JavaRequestProcessor();
    java_result = java_request->getString(*member_id);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    /** Make an internal request for the main thread to handle, to get the member pointer **/

    reference = internal_req_ref_counter++;

    internal_request_params->push_back(parent_id);
    internal_request_params->push_back(java_result->return_string);

    member_data = new ResultData();
    member_data->result_ready = false;

    event = new IcedTeaRunnableMethod(&_getMember, (void*) internal_request_params, member_data);
    NS_DispatchToMainThread(event, 0);

    while (!member_data->result_ready) // wait till ready
    {
        usleep(2000);
    }

    PLUGIN_DEBUG_1ARG("Member PTR after internal request: %s\n", member_data->return_string->c_str());

    internal_req_ref_counter--;

    delete java_request;
    java_request = new JavaRequestProcessor();
    java_result = java_request->findClass("netscape.javascript.JSObject");

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    *jsObjectClassID += *(java_result->return_string);

    // We have the result. Free the request memory
    delete java_request;

    java_request = new JavaRequestProcessor();

    args = new std::vector<std::string>();
    std::string longArg = "J";
    args->push_back(longArg);

    java_result = java_request->getMethodID(
            *(jsObjectClassID),
            browser_functions.getstringidentifier("<init>"),
            *args);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    *jsObjectConstructorID += *(java_result->return_string);

    delete args;
    delete java_request;

    // We have the method id. Now create a new object.

    java_request = new JavaRequestProcessor();
    args = new std::vector<std::string>();
    args->push_back(*(member_data->return_string));
    java_result = java_request->newObject(*jsObjectClassID,
                                          *jsObjectConstructorID,
                                          *args);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occured)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }


    response = IcedTeaPluginUtilities::constructMessagePrefix(0);
    response->append(" JavaScriptGetMember ");
    response->append(java_result->return_string->c_str());
    plugin_to_java_bus->post(response->c_str());


    // Now be a good citizen and help keep the heap free of garbage
    cleanup:
    delete args;
    delete java_request; // request object
    delete member_id; // member id string
    delete message_parts; // message_parts vector that was allocated by the caller
    delete internal_request_params; // delete the internal requests params vector
    delete jsObjectClassID; // delete object that holds the jsobject
    delete member_data->return_string;
    delete member_data;

    pthread_mutex_lock(&tc_mutex);
    thread_count--;
    pthread_mutex_unlock(&tc_mutex);
}

void*
queue_processor(void* data)
{

    PluginRequestProcessor* processor = (PluginRequestProcessor*) data;
    std::vector<std::string>* message_parts = NULL;
    std::string command;

    PLUGIN_DEBUG_1ARG("Queue processor initialized. Queue = %p\n", message_queue);

    while (true)
    {
        pthread_mutex_lock(&message_queue_mutex);
        if (message_queue->size() > 0)
        {
            message_parts = message_queue->front();
            message_queue->erase(message_queue->begin());
        }
        pthread_mutex_unlock(&message_queue_mutex);

        if (message_parts)
        {
            PLUGIN_DEBUG_0ARG("Processing engaged\n");

            command = message_parts->at(2);

            if (command == "GetMember")
            {
                processor->sendMember(message_parts);
            } else if (command == "ToString")
            {
                processor->sendString(message_parts);
            } else if (command == "SetMember")
            {
                processor->setMember(message_parts);
            } else
            {
                // Nothing matched
                IcedTeaPluginUtilities::printStringVector("Error: Unable to process message: ", message_parts);

            }

            PLUGIN_DEBUG_0ARG("Processing dis-engaged\n");
        } else
        {
            usleep(20000);
            pthread_testcancel();
        }

        message_parts = NULL;
    }

    PLUGIN_DEBUG_0ARG("Queue processing stopped.\n");
}

/******************************************
 * Functions delegated to the main thread *
 ******************************************/

void*
_setMember(void* data, ResultData* result)
{
    std::string* property_name;
    std::string* value;
    std::string* type;
    std::string* response;
    std::vector<std::string*>* message_parts = (std::vector<std::string*>*) data;

    NPP instance;
    NPVariant* value_variant = new NPVariant();
    NPObject* member;
    NPIdentifier property;

    IcedTeaPluginUtilities::printStringPtrVector("PluginRequestProcessor::_setMember - ", message_parts);

    member = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(*(message_parts->at(0))));
    property_name = message_parts->at(1);
    type = message_parts->at(2);
    value = message_parts->at(3);

    instance = getInstanceFromMemberPtr(member);
    convertToNPVariant(*value, *type, value_variant);

    PLUGIN_DEBUG_4ARG("Setting %s on instance %p, object %p to value %s\n", property_name->c_str(), instance, member, value_variant);

    property = browser_functions.getstringidentifier(property_name->c_str());
    browser_functions.setproperty(instance, member, property, value_variant);

    response = IcedTeaPluginUtilities::constructMessagePrefix(0);
    response->append(" JavaScriptSetMember ");
    plugin_to_java_bus->post(response->c_str());

    // free memory
    IcedTeaPluginUtilities::freeStringPtrVector(message_parts);
    delete value_variant;
    delete response;

    result->result_ready = true;

}

void*
_getMember(void* data, ResultData* result)
{
    std::string* parent_ptr_str;
    std::string* member_name;

    NPObject* parent_ptr;
    NPVariant member_ptr;
    std::string* member_ptr_str;
    NPP instance;
    NPIdentifier member_identifier;

    std::vector<std::string*>* message_parts = (std::vector<std::string*>*) data;

    IcedTeaPluginUtilities::printStringPtrVector("PluginRequestProcessor::_getMember - ", message_parts);

    parent_ptr_str = message_parts->at(0);
    member_name = message_parts->at(1);

    // Get the corresponding windowId
    member_identifier = browser_functions.getstringidentifier(member_name->c_str());

    // Get the window pointer
    parent_ptr = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(parent_ptr_str->c_str()));

    // Get the associated instance
    instance = getInstanceFromMemberPtr(parent_ptr);

    // Get the NPVariant corresponding to this member
    PLUGIN_DEBUG_4ARG("Looking for %p %p %p (%s)\n", instance, parent_ptr, member_identifier,member_name->c_str());

    if (!browser_functions.hasproperty(instance, parent_ptr, member_identifier))
    {
        printf("%s not found!\n", member_name->c_str());
    }
    browser_functions.getproperty(instance, parent_ptr, member_identifier, &member_ptr);

    IcedTeaPluginUtilities::printNPVariant(member_ptr);
    member_ptr_str = IcedTeaPluginUtilities::JSIDToString(NPVARIANT_TO_OBJECT(member_ptr));
    PLUGIN_DEBUG_2ARG("Got variant %p (integer value = %s)\n", NPVARIANT_TO_OBJECT(member_ptr), member_ptr_str->c_str());

    result->return_string = member_ptr_str;
    result->result_ready = true;

    // store member -> instance link
    storeInstanceID(NPVARIANT_TO_OBJECT(member_ptr), instance);

    PLUGIN_DEBUG_0ARG("_getMember returning.\n");

    return result;
}
