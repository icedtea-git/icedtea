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

#include <typeinfo>

#include "IcedTeaScriptablePluginObject.h"
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
 * PluginRequestProcessor constructor.
 *
 * Initializes various complex data structures used by the class.
 */

PluginRequestProcessor::PluginRequestProcessor()
{
    this->pendingRequests = new std::map<pthread_t, uintmax_t>();

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
                   command == "ToString"  ||
                   command == "Call"      ||
                   command == "GetSlot"   ||
                   command == "SetSlot"   ||
                   command == "Eval")
        {

        	// Update queue synchronously
        	pthread_mutex_lock(&message_queue_mutex);
            message_queue->push_back(message_parts);
            pthread_mutex_unlock(&message_queue_mutex);

            // Broadcast that a message is now available
            pthread_cond_broadcast(&cond_message_available);

            return true;
        }

    }

    delete message_parts;

    // If we got here, it means we couldn't process the message. Let the caller know.
    return false;
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
    std::string response = std::string();
    std::string window_ptr_str = std::string();
    static NPObject* window_ptr;
    int id;

    type = message_parts->at(0);
    id = atoi(message_parts->at(1).c_str());
    command = message_parts->at(2);

    NPP instance;
    get_instance_from_id(id, instance);

    browser_functions.getvalue(instance, NPNVWindowNPObject, &window_ptr);
    PLUGIN_DEBUG_3ARG("ID=%d, Instance=%p, WindowPTR = %p\n", id, instance, window_ptr);

    IcedTeaPluginUtilities::JSIDToString(window_ptr, &window_ptr_str);

    // We need the context 0 for backwards compatibility with the Java side
    IcedTeaPluginUtilities::constructMessagePrefix(0, &response);
    response += " JavaScriptGetWindow ";
    response += window_ptr_str;

    plugin_to_java_bus->post(response.c_str());

    delete message_parts;

    // store the instance pointer for future reference
    IcedTeaPluginUtilities::storeInstanceID(window_ptr, instance);
}

/**
 * Evaluates the given script
 *
 * @param message_parts The request message.
 */

void
PluginRequestProcessor::eval(std::vector<std::string>* message_parts)
{
    JavaRequestProcessor request_processor = JavaRequestProcessor();
    JavaResultData* java_result;

    NPObject* window_ptr;
    NPP instance;
    std::string script;
    NPVariant result;
    std::string response = std::string();
    std::string return_type = std::string();
    int id;

    window_ptr = (NPObject*) IcedTeaPluginUtilities::stringToJSID(message_parts->at(3));
    instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(window_ptr);

    java_result = request_processor.getString(message_parts->at(4));
    CHECK_JAVA_RESULT(java_result);
    script.append(*(java_result->return_string));

    AyncCallThreadData thread_data = AyncCallThreadData();
    thread_data.result_ready = false;
    thread_data.parameters = std::vector<void*>();
    thread_data.result = std::string();

    thread_data.parameters.push_back(instance);
    thread_data.parameters.push_back(window_ptr);
    thread_data.parameters.push_back(&script);

    browser_functions.pluginthreadasynccall(instance, &_eval, &thread_data);

    while (!thread_data.result_ready) usleep(2000); // Wait till result is ready

    NPVariant* result_variant = (NPVariant*) IcedTeaPluginUtilities::stringToJSID(thread_data.result);
    std::string result_variant_jniid = std::string();
    storeVariantInJava(*result_variant, &result_variant_jniid);

    IcedTeaPluginUtilities::constructMessagePrefix(0, &response);
    response += " JavaScriptEval ";
    response += result_variant_jniid;

    plugin_to_java_bus->post(response.c_str());

    delete message_parts;

}

/**
 * Calls the given javascript script
 *
 * @param message_parts The request message.
 */

void
PluginRequestProcessor::call(std::vector<std::string>* message_parts)
{
    NPP instance;
    std::string window_ptr_str;
    void* window_ptr;
    std::string window_function_name;
    std::vector<NPVariant> args = std::vector<NPVariant>();
    std::vector<std::string> arg_ids = std::vector<std::string>();
    int arg_count;
    std::string response = std::string();
    JavaRequestProcessor java_request = JavaRequestProcessor();
    JavaResultData* java_result;

    // window
    window_ptr_str = message_parts->at(3);
    window_ptr = IcedTeaPluginUtilities::stringToJSID(window_ptr_str);

    // instance
    instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(window_ptr);

    // function name
    java_result = java_request.getString(message_parts->at(4));
    CHECK_JAVA_RESULT(java_result);
    window_function_name.append(*(java_result->return_string));

    // arguments
    for (int i=5; i < message_parts->size(); i++)
    {
        arg_ids.push_back(message_parts->at(i));
    }

    std::string number_class_id = std::string();
    std::string boolean_class_id = std::string();
    std::string byte_class_id = std::string();
    std::string char_class_id = std::string();
    std::string string_class_id = std::string();

    java_result = java_request.findClass(0, "java.lang.Number");
    CHECK_JAVA_RESULT(java_result);
    number_class_id.append(*(java_result->return_string));

    java_result = java_request.findClass(0, "java.lang.Boolean");
    CHECK_JAVA_RESULT(java_result);
    boolean_class_id.append(*(java_result->return_string));

    java_result = java_request.findClass(0, "java.lang.Byte");
    CHECK_JAVA_RESULT(java_result);
    byte_class_id.append(*(java_result->return_string));

    java_result = java_request.findClass(0, "java.lang.Character");
    CHECK_JAVA_RESULT(java_result);
    char_class_id.append(*(java_result->return_string));

    java_result = java_request.findClass(0, "java.lang.String");
    CHECK_JAVA_RESULT(java_result);
    string_class_id.append(*(java_result->return_string));

    // determine arguments
    for (int i=0; i < arg_ids.size(); i++)
    {
        bool isNum = false;
        bool isBool = false;
        bool isByte = false;
        bool isChar = false;
        bool isString = false;
        std::string id = arg_ids[i];
        std::string arg_class_name = std::string();
        std::string arg_value = std::string();
        NPVariant* variant = new NPVariant();

        // instanceOf calls are very fast, so just do them all even
        // if one of them is true

        // Number
        java_result = java_request.isInstanceOf(id, number_class_id);
        CHECK_JAVA_RESULT(java_result);
        if (*(java_result->return_string) == "1")
        {
            isNum = true;
        }

        // Boolean
        java_result = java_request.isInstanceOf(id, boolean_class_id);
        CHECK_JAVA_RESULT(java_result);
        if (*(java_result->return_string) == "1")
        {
            isBool = true;
        }

        void storeVariantInJava(NPVariant variant, std::string* result);
        // Byte
        java_result = java_request.isInstanceOf(id, byte_class_id);
        CHECK_JAVA_RESULT(java_result);
        if (*(java_result->return_string) == "1")
        {
            isByte = true;
        }

        // Character
        java_result = java_request.isInstanceOf(id, char_class_id);
        CHECK_JAVA_RESULT(java_result);
        if (*(java_result->return_string) == "1")
        {
            isChar = true;
        }

        // String
        java_result = java_request.isInstanceOf(id, string_class_id);
        CHECK_JAVA_RESULT(java_result);
        if (*(java_result->return_string) == "1")
        {
            isString = true;
        }

        if (isNum)
        {
            int i;
            double d;

            java_result = java_request.getString(id);
            CHECK_JAVA_RESULT(java_result);

            if (java_result->return_string->find("."))
            {
                d = strtod(java_result->return_string->c_str(), NULL);
                DOUBLE_TO_NPVARIANT(d, *variant);
            } else
            {
                i = atoi(java_result->return_string->c_str());
                INT32_TO_NPVARIANT(d, *variant);
            }


        } else if (isChar || isByte)
        {
            int i;
            std::string str = std::string();

            java_result = java_request.getString(id);
            CHECK_JAVA_RESULT(java_result);
            str.append(*(java_result->return_string));

            // FIXME: No multi-byte char support yet
            i = str[i];

            INT32_TO_NPVARIANT(i, *variant);

        } else if (isBool)
        {
            bool b;

            java_result = java_request.getString(id);
            CHECK_JAVA_RESULT(java_result);

            if (java_result->return_string->find("true") == 0)
                b = true;
            else
                b = false;

            BOOLEAN_TO_NPVARIANT(b, *variant);

        } else if (isString)
        {
            java_result = java_request.getString(id);
            CHECK_JAVA_RESULT(java_result);

            // string is non-primitive and needs to be allocated on the heap
            NPUTF8* arg_str = (NPUTF8*) malloc(sizeof(NPUTF8)*java_result->return_string->size() + 1);
            strcpy(arg_str, java_result->return_string->c_str());

            STRINGZ_TO_NPVARIANT(arg_str, *variant);
        } else
        {
            // Else complex java object

            std::string class_id; // class id of object

            NPObject* obj;

            java_result = java_request.getClassID(id);
            CHECK_JAVA_RESULT(java_result);
            class_id.append(*(java_result->return_string));

            obj = IcedTeaScriptableJavaPackageObject::get_scriptable_java_object(instance, class_id, id, false);

            OBJECT_TO_NPVARIANT(obj, *variant);

        }

        args.push_back(*variant);
    }

    arg_count = args.size();
    NPVariant *args_array = (NPVariant*) malloc(sizeof(NPVariant)*args.size());
    for (int i=0; i < args.size(); i++)
        args_array[i] = args[i];

    AyncCallThreadData thread_data = AyncCallThreadData();
    thread_data.result_ready = false;
    thread_data.parameters = std::vector<void*>();
    thread_data.result = std::string();

    thread_data.parameters.push_back(instance);
    thread_data.parameters.push_back(window_ptr);
    thread_data.parameters.push_back(&window_function_name);
    thread_data.parameters.push_back(&arg_count);
    thread_data.parameters.push_back(args_array);

    printf("Packing %p [%p] %p %s@%p %p %p\n", instance, thread_data.parameters.at(0), window_ptr, window_function_name.c_str(), &window_function_name, &arg_count, args_array);
    browser_functions.pluginthreadasynccall(instance, &_call, &thread_data);

    while (!thread_data.result_ready) usleep(2000); // wait till ready

    NPVariant* result_variant = (NPVariant*) IcedTeaPluginUtilities::stringToJSID(thread_data.result);
    std::string result_variant_jniid = std::string();
    storeVariantInJava(*result_variant, &result_variant_jniid);

    IcedTeaPluginUtilities::constructMessagePrefix(0, &response);
    response += " JavaScriptCall ";
    response += result_variant_jniid;

    plugin_to_java_bus->post(response.c_str());

    cleanup:
    free(args_array);
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
    JavaRequestProcessor java_request = JavaRequestProcessor();
    JavaResultData* java_result;
    std::string response = std::string();
    int instance;

    instance = atoi(message_parts->at(1).c_str());
    variant_ptr = message_parts->at(3);

    variant = (NPVariant*) IcedTeaPluginUtilities::stringToJSID(variant_ptr);
    variant_string = IcedTeaPluginUtilities::NPVariantToString(*variant);

    java_result = java_request.newString(*variant_string);

    if (java_result->error_occurred)
    {
        printf("Unable to process NewString request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    variant_string_id = java_result->return_string;

    // We need the context 0 for backwards compatibility with the Java side
    IcedTeaPluginUtilities::constructMessagePrefix(instance, &response);
    response += " JavaScriptToString ";
    response += *variant_string_id;

    plugin_to_java_bus->post(response.c_str());

    cleanup:
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
    std::string value = std::string();
    std::string type = std::string();
    std::string* value_variant_ptr_str;

    NPP instance;
    NPObject* member;

    JavaRequestProcessor* java_request = new JavaRequestProcessor();
    JavaResultData* java_result;

    IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::_setMember - ", message_parts);

    member = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(message_parts->at(3)));
    propertyNameID = message_parts->at(4);
    valueID = message_parts->at(5);
    instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(member);

    java_request = new JavaRequestProcessor();
    java_result = java_request->getString(propertyNameID);

        // the result we want is in result_string (assuming there was no error)
        if (java_result->error_occurred)
        {
            printf("Unable to get member name for setMember. Error occurred: %s\n", java_result->error_msg);
            //goto cleanup;
        }

        property_identifier = browser_functions.getstringidentifier(java_result->return_string->c_str());
    }

    // Copy into local variable before disposing the object
    property_name.append(*(java_result->return_string));
    delete java_request;

    // Based on the value ID, find the type and string value
    // FIXME: Complex java objects not yet peered

    java_request = new JavaRequestProcessor();
    java_result = java_request->getClassName(valueID);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occurred)
    {
        printf("Unable to get class name for setMember. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    // Copy into local variable before disposing the object
    type.append(*(java_result->return_string));
    delete java_request;

    java_request = new JavaRequestProcessor();
    java_result = java_request->getToStringValue(valueID);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occurred)
    {
        printf("Unable to get value for setMember. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    value.append(*(java_result->return_string));

    AyncCallThreadData thread_data = AyncCallThreadData();
    thread_data.result_ready = false;
    thread_data.parameters = std::vector<void*>();
    thread_data.result = std::string();

    thread_data.parameters.push_back(instance);
    thread_data.parameters.push_back(member);
    thread_data.parameters.push_back(&property_name);
    thread_data.parameters.push_back(&type);
    thread_data.parameters.push_back(&value);

    browser_functions.pluginthreadasynccall(instance, &_setMember, &thread_data);

    while (!thread_data.result_ready) usleep(2000); // wait till ready

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
    std::vector<std::string> args;
    JavaRequestProcessor java_request = JavaRequestProcessor();
    JavaResultData* java_result;
    NPObject* parent_ptr;

    std::string member_id = std::string();
    std::string jsObjectClassID = std::string();
    std::string jsObjectConstructorID = std::string();
    std::string response = std::string();

    NPIdentifier member_identifier;

    int method_id;
    int instance_id;
    long reference;

    // debug printout of parent thread data
    IcedTeaPluginUtilities::printStringVector("PluginRequestProcessor::getMember:", message_parts);

    // store info in local variables for easy access
    instance_id = atoi(message_parts->at(1).c_str());
    parent_ptr = reinterpret_cast <NPObject*> (IcedTeaPluginUtilities::stringToJSID(message_parts->at(3)));
    member_id += message_parts->at(4);

    /** Request data from Java if necessary **/
    if (message_parts->at(2) == "GetSlot")
    {
        member_identifier = browser_functions.getintidentifier(atoi(member_id.c_str()));
    } else
    {
        // make a new request for getString, to get the name of the identifier
        java_result = java_request.getString(member_id);

        // the result we want is in result_string (assuming there was no error)
        if (java_result->error_occurred)
        {
            printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
            //goto cleanup;
        }

        member_identifier = browser_functions.getstringidentifier(java_result->return_string->c_str());
    }

    /** Make an internal request for the main thread to handle, to get the member pointer **/

    reference = internal_req_ref_counter++;

    AyncCallThreadData thread_data = AyncCallThreadData();
    thread_data.result_ready = false;
    thread_data.parameters = std::vector<void*>();
    thread_data.result = std::string();

    NPP instance = IcedTeaPluginUtilities::getInstanceFromMemberPtr(parent_ptr);
    thread_data.parameters.push_back(instance);
    thread_data.parameters.push_back(parent_ptr);
    thread_data.parameters.push_back(java_result->return_string);

    browser_functions.pluginthreadasynccall(instance, &_getMember, &thread_data);

    while (!thread_data.result_ready) usleep(2000); // wait till ready

    PLUGIN_DEBUG_1ARG("Member PTR after internal request: %s\n", thread_data.result.c_str());

    internal_req_ref_counter--;

    java_result = java_request.findClass(0, "netscape.javascript.JSObject");

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occurred)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    jsObjectClassID.append(*(java_result->return_string));

    args = std::vector<std::string>();
    std::string longArg = "J";
    args.push_back(longArg);

    java_result = java_request.getMethodID(jsObjectClassID,
                                           browser_functions.getstringidentifier("<init>"),
                                           args);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occurred)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }

    jsObjectConstructorID.append(*(java_result->return_string));

    // We have the method id. Now create a new object.

    args.clear();
    args.push_back(thread_data.result);
    java_result = java_request.newObjectWithConstructor("",
                                         jsObjectClassID,
                                         jsObjectConstructorID,
                                         args);

    // the result we want is in result_string (assuming there was no error)
    if (java_result->error_occurred)
    {
        printf("Unable to process getMember request. Error occurred: %s\n", java_result->error_msg);
        //goto cleanup;
    }


    IcedTeaPluginUtilities::constructMessagePrefix(0, &response);
    if (message_parts->at(2) == "GetSlot")
    {
        response.append(" JavaScriptGetMember ");
    } else {
        response.append(" JavaScriptGetSlot ");
    }
    response.append(java_result->return_string->c_str());
    plugin_to_java_bus->post(response.c_str());


    // Now be a good citizen and help keep the heap free of garbage
    cleanup:
    delete message_parts; // message_parts vector that was allocated by the caller

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
    pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER; // This is needed for API compat. and is unused

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
            command = message_parts->at(2);

            if (command == "GetMember")
            {
                processor->sendMember(message_parts);
            } else if (command == "ToString")
            {
                processor->sendString(message_parts);
            } else if (command == "SetMember")
            {
            	// write methods are synchronized
            	pthread_mutex_lock(&syn_write_mutex);
                processor->setMember(message_parts);
                pthread_mutex_unlock(&syn_write_mutex);
            } else if (command == "Call")
            {
                // write methods are synchronized
                pthread_mutex_lock(&syn_write_mutex);
                processor->call(message_parts);
                pthread_mutex_unlock(&syn_write_mutex);
            } else if (command == "Eval")
            {
                // write methods are synchronized
                pthread_mutex_lock(&syn_write_mutex);
                processor->eval(message_parts);
                pthread_mutex_unlock(&syn_write_mutex);
            } else
            {
                // Nothing matched
                IcedTeaPluginUtilities::printStringVector("Error: Unable to process message: ", message_parts);

            }

        } else
        {
        	pthread_cond_wait(&cond_message_available, &wait_mutex);
            pthread_testcancel();
        }

        message_parts = NULL;
    }

    PLUGIN_DEBUG_0ARG("Queue processing stopped.\n");
}

/* Stores the variant on java side
 *
 * @param variant The variant to store
 * @param result The result
 */
void
PluginRequestProcessor::storeVariantInJava(NPVariant variant, std::string* result)
{

    // FIXME: Should null and void have some different representation?
    // the spec doesn't say..
    JavaRequestProcessor java_request = JavaRequestProcessor();
    JavaResultData* java_result;

    if (NPVARIANT_IS_VOID(variant) || NPVARIANT_IS_NULL(variant))
    {
        NPString str = NPVARIANT_TO_STRING(variant);
        java_result = java_request.newString("");
        CHECK_JAVA_RESULT(java_result);
        result->append(*(java_result->return_string));
    }
    else if (NPVARIANT_IS_BOOLEAN(variant))
    {
        std::string boolean_classid = std::string();
        std::string boolean_constructor_id = std::string();
        std::vector<std::string> boolean_args = std::vector<std::string>();
        std::string value_str;
        if (NPVARIANT_TO_BOOLEAN(variant))
            value_str = "true";
        else
            value_str = "false";

        java_result = java_request.findClass(0, "java.lang.Boolean");
        CHECK_JAVA_RESULT(java_result);
        boolean_classid.append(*(java_result->return_string));

        boolean_args.push_back("Z");
        java_result = java_request.getMethodID(boolean_classid,
                                               browser_functions.getstringidentifier("<init>"),
                                               boolean_args);
        CHECK_JAVA_RESULT(java_result);
        boolean_constructor_id.append(*(java_result->return_string));

        boolean_args.clear();
        boolean_args.push_back(value_str);
        java_result = java_request.newObjectWithConstructor("",
                                             boolean_classid,
                                             boolean_constructor_id,
                                             boolean_args);

        CHECK_JAVA_RESULT(java_result);
        result->append(*(java_result->return_string));
    }
    else if (NPVARIANT_IS_INT32(variant))
    {
        std::string integer_classid = std::string();
        std::string integer_constructor_id = std::string();
        std::vector<std::string> integer_args = std::vector<std::string>();
        std::string value_str = std::string();
        IcedTeaPluginUtilities::itoa(NPVARIANT_TO_INT32(variant), &value_str);

        java_result = java_request.findClass(0, "java.lang.Integer");
        CHECK_JAVA_RESULT(java_result);
        integer_classid.append(*(java_result->return_string));

        integer_args.push_back("I");
        java_result = java_request.getMethodID(integer_classid,
                                               browser_functions.getstringidentifier("<init>"),
                                               integer_args);
        CHECK_JAVA_RESULT(java_result);
        integer_constructor_id.append(*(java_result->return_string));

        integer_args.clear();
        integer_args.push_back(value_str);
        java_result = java_request.newObjectWithConstructor("",
                                             integer_classid,
                                             integer_constructor_id,
                                             integer_args);

        CHECK_JAVA_RESULT(java_result);
        result->append(*(java_result->return_string));
    }
    else if (NPVARIANT_IS_DOUBLE(variant))
    {
        std::string double_classid = std::string();
        std::string double_constructor_id = std::string();
        std::vector<std::string> double_args = std::vector<std::string>();
        std::string value_str = std::string();
        IcedTeaPluginUtilities::itoa(NPVARIANT_TO_DOUBLE(variant), &value_str);

        java_result = java_request.findClass(0, "java.lang.Double");
        CHECK_JAVA_RESULT(java_result);
        double_classid.append(*(java_result->return_string));

        double_args.push_back("J");
        java_result = java_request.getMethodID(double_classid,
                                               browser_functions.getstringidentifier("<init>"),
                                               double_args);
        CHECK_JAVA_RESULT(java_result);
        double_constructor_id.append(*(java_result->return_string));

        double_args.clear();
        double_args.push_back(value_str);
        java_result = java_request.newObjectWithConstructor("",
                                             double_classid,
                                             double_constructor_id,
                                             double_args);

        CHECK_JAVA_RESULT(java_result);
        result->append(*(java_result->return_string));
    }
    else if (NPVARIANT_IS_STRING(variant))
    {
        NPString str = NPVARIANT_TO_STRING(variant);
#if MOZILLA_VERSION_COLLAPSED < 1090200
        java_result = java_request.newString(str.utf8characters);
#else
        java_result = java_request.newString(str.UTF8Characters);
#endif
        CHECK_JAVA_RESULT(java_result);
        result->append(*(java_result->return_string));
    } else {

        // Else it is a complex object
        if (typeid(IcedTeaScriptableJavaObject*) == typeid(NPVARIANT_TO_OBJECT(variant)))
        {
            IcedTeaScriptableJavaObject* obj;
            obj = (IcedTeaScriptableJavaObject*) NPVARIANT_TO_OBJECT(variant);
            result->append(obj->getInstanceID());
        } else {
            // Else store it as a JSObject
            std::string jsobject_classid = std::string();
            std::string jsobject_constructor_id = std::string();
            std::vector<std::string> jsobject_args = std::vector<std::string>();
            std::string value_str = std::string();
            printf("Got NPObject %p\n", NPVARIANT_TO_OBJECT(variant));
            IcedTeaPluginUtilities::JSIDToString(NPVARIANT_TO_OBJECT(variant), &value_str);

            java_result = java_request.findClass(0, "netscape.javascript.JSObject");
            CHECK_JAVA_RESULT(java_result);
            jsobject_classid.append(*(java_result->return_string));

            jsobject_args.push_back("J");
            java_result = java_request.getMethodID(jsobject_classid,
                                                   browser_functions.getstringidentifier("<init>"),
                                                   jsobject_args);
            CHECK_JAVA_RESULT(java_result);
            jsobject_constructor_id.append(*(java_result->return_string));

            jsobject_args.clear();
            jsobject_args.push_back(value_str);
            java_result = java_request.newObjectWithConstructor("",
                                                 jsobject_classid,
                                                 jsobject_constructor_id,
                                                 jsobject_args);

            CHECK_JAVA_RESULT(java_result);
            result->append(*(java_result->return_string));
        }
    }

}


/******************************************
 * Functions delegated to the main thread *
 ******************************************/

void
_setMember(void* data)
{
    std::string* value;
    std::string* type;
    std::string response = std::string();

    NPP instance;
    NPVariant* value_variant = new NPVariant();
    NPObject* member;
    NPIdentifier* property;

    std::vector<void*> parameters = ((AyncCallThreadData*) data)->parameters;
    instance = (NPP) parameters.at(0);
    member = (NPObject*) parameters.at(1);
    property_name = (std::string*) parameters.at(2);
    type = (std::string*) parameters.at(3);
    value = (std::string*) parameters.at(4);

    convertToNPVariant(*value, *type, value_variant);

    PLUGIN_DEBUG_4ARG("Setting %s on instance %p, object %p to value %s\n", property_name->c_str(), instance, member, value_variant);

    property = browser_functions.getstringidentifier(property_name->c_str());
    browser_functions.setproperty(instance, member, property, value_variant);

    IcedTeaPluginUtilities::constructMessagePrefix(0, &response);
    response.append(" JavaScriptSetMember ");
    plugin_to_java_bus->post(response.c_str());

    ((AyncCallThreadData*) data)->result_ready = true;

    // free memory
    delete value_variant;
}

void
_getMember(void* data)
{
    NPObject* parent_ptr;
    NPVariant member_ptr;
    std::string member_ptr_str = std::string();
    NPP instance;

    std::vector<void*> parameters = ((AyncCallThreadData*) data)->parameters;

    instance = (NPP) parameters.at(0);
    parent_ptr = (NPObject*) parameters.at(1);
    NPIdentifier* member_identifier = (NPIdentifier*) parameters.at(2);

    // Get the NPVariant corresponding to this member
    PLUGIN_DEBUG_4ARG("Looking for %p %p %p (%s)\n", instance, parent_ptr, member_identifier, browser_functions.utf8fromidentifier(*member_identifier));

    if (!browser_functions.hasproperty(instance, parent_ptr, *member_identifier))
    {
        printf("%s not found!\n", browser_functions.utf8fromidentifier(*member_identifier));
    }
    browser_functions.getproperty(instance, parent_ptr, member_identifier, &member_ptr);

    IcedTeaPluginUtilities::printNPVariant(member_ptr);
    IcedTeaPluginUtilities::JSIDToString(NPVARIANT_TO_OBJECT(member_ptr), &member_ptr_str);
    PLUGIN_DEBUG_2ARG("Got variant %p (integer value = %s)\n", NPVARIANT_TO_OBJECT(member_ptr), member_ptr_str.c_str());

    ((AyncCallThreadData*) data)->result.append(member_ptr_str);
    ((AyncCallThreadData*) data)->result_ready = true;

    // store member -> instance link
    IcedTeaPluginUtilities::storeInstanceID(NPVARIANT_TO_OBJECT(member_ptr), instance);

    PLUGIN_DEBUG_0ARG("_getMember returning.\n");
}

void
_eval(void* data)
{
    NPP instance;
    NPObject* window_ptr;
    std::string* script_str;
    NPIdentifier script_identifier;
    NPString script = NPString();
    NPVariant* eval_result = new NPVariant();
    std::string eval_result_ptr_str = std::string();

    PLUGIN_DEBUG_0ARG("_eval called\n");

    std::vector<void*>* call_data = (std::vector<void*>*) data;

    instance = (NPP) call_data->at(0);
    window_ptr = (NPObject*) call_data->at(1);
    script_str = (std::string*) call_data->at(2);

#if MOZILLA_VERSION_COLLAPSED < 1090200
    script.utf8characters = script_str->c_str();
    script.utf8length = script_str->size();

    PLUGIN_DEBUG_1ARG("Evaluating: %s\n", script.utf8characters);
#else
    script.UTF8Characters = script_str->c_str();
    script.UTF8Length = script_str->size();

    PLUGIN_DEBUG_1ARG("Evaluating: %s\n", script.UTF8Characters);
#endif

    browser_functions.evaluate(instance, window_ptr, &script, eval_result);
    IcedTeaPluginUtilities::printNPVariant(*eval_result);

    IcedTeaPluginUtilities::JSIDToString(eval_result, &eval_result_ptr_str);
    ((AyncCallThreadData*) data)->result.append(eval_result_ptr_str);
    ((AyncCallThreadData*) data)->result_ready = true;

    PLUGIN_DEBUG_0ARG("_eval returning\n");
}


void
_call(void* data)
{
    NPP instance;
    NPObject* window_ptr;
    std::string* function_name;
    NPIdentifier function;
    int* arg_count;
    NPVariant* args;
    NPVariant* call_result = new NPVariant();
    std::string call_result_ptr_str = std::string();

    PLUGIN_DEBUG_0ARG("_call called\n");

    std::vector<void*>* call_data = (std::vector<void*>*) data;

    instance = (NPP) call_data->at(0);
    window_ptr = (NPObject*) call_data->at(1);
    function_name = (std::string*) call_data->at(2);

    function = browser_functions.getstringidentifier(function_name->c_str());
    arg_count = (int*) call_data->at(3);
    args = (NPVariant*) call_data->at(4);

    printf("Extracted %p -- %p -- %s@%p -- %d -- %p\n", instance, window_ptr, function_name->c_str(), function_name, *arg_count, args);

    browser_functions.invoke(instance, window_ptr, function, args, *arg_count, call_result);

    IcedTeaPluginUtilities::JSIDToString(&call_result, &call_result_ptr_str);
    ((AyncCallThreadData*) data)->result.append(call_result_ptr_str);
    ((AyncCallThreadData*) data)->result_ready = true;

    PLUGIN_DEBUG_0ARG("_call returning\n");
}
