/* IcedTeaPlugin -- implement OJI
   Copyright (C) 2008  Red Hat

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

#include <nsStringAPI.h>

PRThread* current_thread ();

#if PR_BYTES_PER_LONG == 8
#define PLUGIN_JAVASCRIPT_TYPE jlong
#define PLUGIN_INITIALIZE_JAVASCRIPT_ARGUMENT(args, obj) args[0].j = obj
#define PLUGIN_JAVASCRIPT_SIGNATURE "(J)V"
#else
#define PLUGIN_JAVASCRIPT_TYPE jint
#define PLUGIN_INITIALIZE_JAVASCRIPT_ARGUMENT(args, obj) args[0].i = obj
#define PLUGIN_JAVASCRIPT_SIGNATURE "(I)V"
#endif

// System includes.
#include <dlfcn.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// GLib includes.
#include <glib.h>
#include <glib/gstdio.h>

// GTK includes.
#include <gtk/gtk.h>

// FIXME: Look into this:
// #0  nsACString_internal (this=0xbff3016c) at ../../../dist/include/string/nsTSubstring.h:522
// #1  0x007117c9 in nsDependentCSubstring (this=0xbff3016c, str=@0xab20d00, startPos=0, length=0) at ../../dist/include/string/nsTDependentSubstring.h:68
// #2  0x0076a9d9 in Substring (str=@0xab20d00, startPos=0, length=0) at ../../dist/include/string/nsTDependentSubstring.h:103
// #3  0x008333a7 in nsStandardURL::Hostport (this=0xab20ce8) at nsStandardURL.h:338
// #4  0x008299b8 in nsStandardURL::GetHostPort (this=0xab20ce8, result=@0xbff30210) at nsStandardURL.cpp:1003
// #5  0x0095b9dc in nsPrincipal::GetOrigin (this=0xab114e0, aOrigin=0xbff30320) at nsPrincipal.cpp:195
// #6  0x0154232c in nsCSecurityContext::GetOrigin (this=0xab8f410, buf=0xbff30390 "\004", buflen=256) at nsCSecurityContext.cpp:126
// #7  0x04db377e in CNSAdapter_SecurityContextPeer::GetOrigin () from /opt/jdk1.6.0_03/jre/plugin/i386/ns7/libjavaplugin_oji.so
// #8  0x05acd59f in getAndPackSecurityInfo () from /opt/jdk1.6.0_03/jre/lib/i386/libjavaplugin_nscp.so
// #9  0x05acc77f in jni_SecureCallMethod () from /opt/jdk1.6.0_03/jre/lib/i386/libjavaplugin_nscp.so
// #10 0x05aba88d in CSecureJNIEnv::CallMethod () from /opt/jdk1.6.0_03/jre/lib/i386/libjavaplugin_nscp.so
// #11 0x04db1be7 in CNSAdapter_SecureJNIEnv::CallMethod () from /opt/jdk1.6.0_03/jre/plugin/i386/ns7/libjavaplugin_oji.so
// #12 0x0153e62f in ProxyJNIEnv::InvokeMethod (env=0xa8b8040, obj=0x9dad690, method=0xa0ed070, args=0x0) at ProxyJNI.cpp:571
// #13 0x0153f91c in ProxyJNIEnv::InvokeMethod (env=0xa8b8040, obj=0x9dad690, method=0xa0ed070, args=0xbff3065c "\235\225$") at ProxyJNI.cpp:580
// #14 0x0153fdbf in ProxyJNIEnv::CallObjectMethod (env=0xa8b8040, obj=0x9dad690, methodID=0xa0ed070) at ProxyJNI.cpp:641

// timeout (in seconds) for various calls to java side
#define TIMEOUT 180

#define NOT_IMPLEMENTED() \
  PLUGIN_DEBUG_1ARG ("NOT IMPLEMENTED: %s\n", __PRETTY_FUNCTION__)

#define ID(object) \
  (object == NULL ? (PRUint32) 0 : reinterpret_cast<JNIReference*> (object)->identifier)

static int plugin_debug = 0;

#if 1
// Debugging macros.

#define PLUGIN_DEBUG_0ARG(str) \
  do                                        \
  {                                         \
    if (plugin_debug)                       \
    {                                       \
      fprintf (stderr, str);                \
    }                                       \
  } while (0)

#define PLUGIN_DEBUG_1ARG(str, arg1) \
  do                                        \
  {                                         \
    if (plugin_debug)                       \
    {                                       \
      fprintf (stderr, str, arg1);          \
    }                                       \
  } while (0)

#define PLUGIN_DEBUG_2ARG(str, arg1, arg2)  \
  do                                        \
  {                                         \
    if (plugin_debug)                       \
    {                                       \
      fprintf (stderr, str, arg1, arg2);    \
    }                                       \
  } while (0)

#define PLUGIN_DEBUG_3ARG(str, arg1, arg2, arg3) \
  do                                           \
  {                                            \
    if (plugin_debug)                          \
    {                                          \
      fprintf (stderr, str, arg1, arg2, arg3); \
    }                                          \
  } while (0)

#define PLUGIN_DEBUG_4ARG(str, arg1, arg2, arg3, arg4) \
  do                                                 \
  {                                                  \
    if (plugin_debug)                                \
    {                                                \
      fprintf (stderr, str, arg1, arg2, arg3, arg4); \
    }                                                \
  } while (0)

#define PLUGIN_DEBUG(message)                                           \
  PLUGIN_DEBUG_1ARG ("ICEDTEA PLUGIN: %s\n", message)

#define PLUGIN_DEBUG_TWO(first, second)                                 \
  PLUGIN_DEBUG_2ARG ("ICEDTEA PLUGIN: %s %s\n",      \
           first, second)

// Tracing.
class Trace
{
public:
  Trace (char const* name, char const* function)
  {
    Trace::name = name;
    Trace::function = function;
    PLUGIN_DEBUG_2ARG ("ICEDTEA PLUGIN: %s%s\n",
             name, function);
  }

  ~Trace ()
  {
    PLUGIN_DEBUG_3ARG ("ICEDTEA PLUGIN: %s%s %s\n",
             name, function, "return");
  }
private:
  char const* name;
  char const* function;
};

// Testing macro.
#define PLUGIN_TEST(expression, message)  \
  do                                            \
    {                                           \
      if (!(expression))                        \
        printf ("FAIL: %d: %s\n", __LINE__,     \
                message);                       \
    }                                           \
  while (0);

#include <sys/time.h>
#include <unistd.h>

inline suseconds_t get_time_in_ms()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	return tv.tv_usec;
}


inline long get_time_in_s()
{
	time_t t;
	return time(&t);
}

// __func__ is a variable, not a string literal, so it cannot be
// concatenated by the preprocessor.
#define PLUGIN_TRACE_JNIENV() Trace _trace ("JNIEnv::", __func__)
#define PLUGIN_TRACE_FACTORY() Trace _trace ("Factory::", __func__)
#define PLUGIN_TRACE_INSTANCE() Trace _trace ("Instance::", __func__)
#define PLUGIN_TRACE_EVENTSINK() Trace _trace ("EventSink::", __func__)
#define PLUGIN_TRACE_LISTENER() Trace _trace ("Listener::", __func__)
//#define PLUGIN_TRACE_RC() Trace _trace ("ResultContainer::", __func__)
#define PLUGIN_TRACE_RC()

// Error reporting macros.
#define PLUGIN_ERROR(message)                                       \
  fprintf (stderr, "%s:%d: Error: %s\n", __FILE__, __LINE__,  \
           message)

#define PLUGIN_ERROR_TWO(first, second)                                 \
  fprintf (stderr, "%s:%d: Error: %s: %s\n", __FILE__, __LINE__,  \
           first, second)

#define PLUGIN_ERROR_THREE(first, second, third)                        \
  fprintf (stderr, "%s:%d: Error: %s: %s: %s\n", __FILE__,        \
           __LINE__, first, second, third)

#define PLUGIN_CHECK_RETURN(message, result)           \
  if (NS_SUCCEEDED (result))                    \
  {                                             \
    PLUGIN_DEBUG (message);                     \
  }                                             \
  else                                          \
    {                                           \
      PLUGIN_ERROR (message);                   \
      return result;                            \
    }

#define PLUGIN_CHECK(message, result)           \
  if (NS_SUCCEEDED (result))                    \
  {                                             \
    PLUGIN_DEBUG (message);                     \
  } else                                        \
  {                                             \
    PLUGIN_ERROR (message);                     \
  }

#else

// Debugging macros.
#define PLUGIN_DEBUG(message)
#define PLUGIN_DEBUG_TWO(first, second)

// Testing macros.
#define PLUGIN_TEST(expression, message)
#define PLUGIN_TRACE_JNIENV()
#define PLUGIN_TRACE_FACTORY() Trace _trace ("Factory::", __func__)
//#define PLUGIN_TRACE_FACTORY()
#define PLUGIN_TRACE_INSTANCE()
#define PLUGIN_TRACE_EVENTSINK()
#define PLUGIN_TRACE_LISTENER()

// Error reporting macros.
#define PLUGIN_ERROR(message)                                       \
  fprintf (stderr, "%s:%d: Error: %s\n", __FILE__, __LINE__,  \
           message)

#define PLUGIN_ERROR_TWO(first, second)                                 \
  fprintf (stderr, "%s:%d: Error: %s: %s\n", __FILE__, __LINE__,  \
           first, second)

#define PLUGIN_ERROR_THREE(first, second, third)                        \
  fprintf (stderr, "%s:%d: Error: %s: %s: %s\n", __FILE__,        \
           __LINE__, first, second, third)
#define PLUGIN_CHECK_RETURN(message, result)
#define PLUGIN_CHECK(message, result)
#endif

#define PLUGIN_NAME "IcedTea Java Web Browser Plugin (using " PLUGIN_VERSION ")"
#define PLUGIN_DESCRIPTION "The " PLUGIN_NAME " executes Java applets."
#define PLUGIN_MIME_DESC                                               \
  "application/x-java-vm:class,jar:IcedTea;"                           \
  "application/x-java-applet:class,jar:IcedTea;"                       \
  "application/x-java-applet;version=1.1:class,jar:IcedTea;"           \
  "application/x-java-applet;version=1.1.1:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.1.2:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.1.3:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.2:class,jar:IcedTea;"           \
  "application/x-java-applet;version=1.2.1:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.2.2:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.3:class,jar:IcedTea;"           \
  "application/x-java-applet;version=1.3.1:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.4:class,jar:IcedTea;"           \
  "application/x-java-applet;version=1.4.1:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.4.2:class,jar:IcedTea;"         \
  "application/x-java-applet;version=1.5:class,jar:IcedTea;"           \
  "application/x-java-applet;version=1.6:class,jar:IcedTea;"           \
  "application/x-java-applet;jpi-version=1.6.0_00:class,jar:IcedTea;"  \
  "application/x-java-bean:class,jar:IcedTea;"                         \
  "application/x-java-bean;version=1.1:class,jar:IcedTea;"             \
  "application/x-java-bean;version=1.1.1:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.1.2:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.1.3:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.2:class,jar:IcedTea;"             \
  "application/x-java-bean;version=1.2.1:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.2.2:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.3:class,jar:IcedTea;"             \
  "application/x-java-bean;version=1.3.1:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.4:class,jar:IcedTea;"             \
  "application/x-java-bean;version=1.4.1:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.4.2:class,jar:IcedTea;"           \
  "application/x-java-bean;version=1.5:class,jar:IcedTea;"             \
  "application/x-java-bean;version=1.6:class,jar:IcedTea;"             \
  "application/x-java-bean;jpi-version=1.6.0_00:class,jar:IcedTea;"

#define FAILURE_MESSAGE "IcedTeaPluginFactory error: Failed to run %s." \
  "  For more detail rerun \"firefox -g\" in a terminal window."

// Global instance counter.
// A global variable for reporting GLib errors.  This must be free'd
// and set to NULL after each use.
static GError* channel_error = NULL;
// Fully-qualified appletviewer executable.
gchar* data_directory = NULL;
static char* appletviewer_executable = NULL;
static char* libjvm_so = NULL;

class IcedTeaPluginFactory;

static PRBool factory_created = PR_FALSE;
static IcedTeaPluginFactory* factory = NULL;

static PRBool jvm_attached = PR_FALSE;

// Applet viewer input channel (needs to be static because it is used in plugin_in_pipe_callback)
GIOChannel* in_from_appletviewer = NULL;

// Callback used to monitor input pipe status.
static gboolean plugin_in_pipe_callback (GIOChannel* source,
                                         GIOCondition condition,
                                         gpointer plugin_data);

#include <prmon.h>
#include <queue>
#include <nsCOMPtr.h>
#include <nsIThread.h>
#include <nspr.h>

PRMonitor *jvmMsgQueuePRMonitor;
std::queue<nsCString> jvmMsgQueue;
nsCOMPtr<nsIThread> processThread;

// IcedTeaJNIEnv helpers.
class JNIReference
{
public:
  JNIReference (PRUint32 identifier);
  ~JNIReference ();
  PRUint32 identifier;
  PRUint32 count;
};

JNIReference::JNIReference (PRUint32 identifier)
  : identifier (identifier),
    count (0)
{
  PLUGIN_DEBUG_2ARG ("JNIReference CONSTRUCT: %d %p\n", identifier, this);
}

JNIReference::~JNIReference ()
{
  PLUGIN_DEBUG_2ARG ("JNIReference DECONSTRUCT: %d %p\n", identifier, this);
}

class JNIID : public JNIReference
{
public:
  JNIID (PRUint32 identifier, char const* signature);
  ~JNIID ();
  char const* signature;
};

JNIID::JNIID (PRUint32 identifier, char const* signature)
  : JNIReference (identifier),
    signature (strdup (signature))
{
  PLUGIN_DEBUG_2ARG ("JNIID CONSTRUCT: %d %p\n", identifier, this);
}

JNIID::~JNIID ()
{
  PLUGIN_DEBUG_2ARG ("JNIID DECONSTRUCT: %d %p\n", identifier, this);
}

char const* TYPES[10] = { "Object",
                          "boolean",
                          "byte",
                          "char",
                          "short",
                          "int",
                          "long",
                          "float",
                          "double",
                          "void" };


// FIXME: create index from security context.
#define MESSAGE_CREATE()                                     \
  nsCString message ("context ");                            \
  message.AppendInt (0);                                     \

#define MESSAGE_ADD_STACK_REFERENCE(reference) \
  message += " reference ";                                  \
  message.AppendInt (reference);                             \
  if (!factory->result_map.Get(reference, NULL)) {           \
	   ResultContainer *resultC = new ResultContainer();      \
	   factory->result_map.Put(reference, resultC);  \
	   PLUGIN_DEBUG_3ARG ("ResultMap %p created for reference %d found = %d\n", resultC, reference, factory->result_map.Get(reference, NULL)); \
  } \
  else                                                      \
  {                                                         \
       ResultContainer *resultC;                          \
       factory->result_map.Get(reference, &resultC);     \
       resultC->Clear();                                  \
  }

#define MESSAGE_ADD_SRC(src) \
	message += " src "; \
	message += src;

#define MESSAGE_ADD_PRIVILEGES(ctx)             \
  nsCString privileges("");                     \
  GetEnabledPrivileges(&privileges, ctx);       \
  if (privileges.Length() > 0)                  \
  {                                             \
    message += " privileges ";                  \
    message += privileges;                      \
  }

#define MESSAGE_ADD_FUNC() \
  message += " ";											 \
  message += __func__;

#define MESSAGE_ADD_STRING(name)                \
  message += " ";                               \
  message += name;

#define MESSAGE_ADD_SIZE(size)                  \
  message += " ";                               \
  message.AppendInt ((PRUintn) size);

// Pass character value through socket as an integer.
#define MESSAGE_ADD_TYPE(type)                  \
  message += " ";                               \
  message += TYPES[type];

#define MESSAGE_ADD_REFERENCE(clazz)                    \
  message += " ";                                       \
  message.AppendInt (clazz ? ID (clazz) : 0);

#define MESSAGE_ADD_ID(id)                                              \
  message += " ";                                                       \
  message.AppendInt (reinterpret_cast<JNIID*> (id)->identifier);

#define MESSAGE_ADD_ARGS(id, args)               \
  message += " ";                                \
  char* expandedArgs = ExpandArgs (reinterpret_cast<JNIID*> (id), args); \
  message += expandedArgs;                                              \
  free (reinterpret_cast<void*> (expandedArgs));                        \
  expandedArgs = NULL;
 

#define MESSAGE_ADD_VALUE(id, val)                      \
  message += " ";                                       \
  char* expandedValues =                                \
    ExpandArgs (reinterpret_cast<JNIID*> (id), &val);   \
  message += expandedValues;                            \
  free (expandedValues);                                \
  expandedValues = NULL;

#define MESSAGE_ADD_STRING_UCS(pointer, length) \
  for (int i = 0; i < length; i++)              \
    {                                           \
      message += " ";                           \
      message.AppendInt (pointer[i]);           \
    }

#define MESSAGE_ADD_STRING_UTF(pointer)         \
  int i = 0;                                    \
  while (pointer[i] != 0)                       \
    {                                           \
      message += " ";                           \
      message.AppendInt (pointer[i]);           \
      i++;                                      \
    }

#define MESSAGE_SEND()                          \
  factory->SendMessageToAppletViewer (message);

// FIXME: Right now, the macro below will exit only 
// if error occured and we are in the middle of a 
// shutdown (so that the permanent loop does not block 
// proper exit). We need better error handling

#define PROCESS_PENDING_EVENTS_REF(reference) \
    if (jvm_attached == PR_FALSE) \
	{ \
	    PLUGIN_DEBUG_0ARG("Error on Java side detected. Abandoning wait and returning.\n"); \
		return NS_ERROR_FAILURE; \
	} \
	if (g_main_context_pending (NULL)) { \
	   g_main_context_iteration(NULL, false); \
	} \
    PRBool hasPending;  \
    factory->current->HasPendingEvents(&hasPending); \
	if (hasPending == PR_TRUE) \
	{ \
	  PRBool processed = PR_FALSE; \
	  factory->current->ProcessNextEvent(PR_TRUE, &processed); \
	} else \
	{\
	    PR_Sleep(PR_INTERVAL_NO_WAIT); \
	}

#define PROCESS_PENDING_EVENTS \
	PRBool hasPending;  \
	factory->current->HasPendingEvents(&hasPending); \
	if (hasPending == PR_TRUE) { \
		PRBool processed = PR_FALSE; \
		factory->current->ProcessNextEvent(PR_TRUE, &processed); \
	} \
	if (g_main_context_pending (NULL)) { \
       g_main_context_iteration(NULL, false); \
    } else \
    { \
		PR_Sleep(PR_INTERVAL_NO_WAIT); \
	}

#define MESSAGE_RECEIVE_REFERENCE(reference, cast, name)                \
  nsresult res = NS_OK;                                                 \
  PLUGIN_DEBUG_0ARG ("RECEIVE 1\n");                                    \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnIdentifier == -1 &&\
	     resultC->errorOccurred == PR_FALSE)     \
    {                                                                   \
      PROCESS_PENDING_EVENTS_REF (reference);                                \
    }                                                                   \
  PLUGIN_DEBUG_0ARG ("RECEIVE 3\n"); \
  if (resultC->returnIdentifier == 0 || \
	  resultC->errorOccurred == PR_TRUE) \
  {  \
	  *name = NULL;                                                     \
  } else {                                                              \
  *name =                                                               \
    reinterpret_cast<cast>                                              \
    (factory->references.ReferenceObject (resultC->returnIdentifier)); \
  } \
  PLUGIN_DEBUG_3ARG ("RECEIVE_REFERENCE: %s result: %x = %d\n",                    \
          __func__, *name, resultC->returnIdentifier);

// FIXME: track and free JNIIDs.
#define MESSAGE_RECEIVE_ID(reference, cast, id, signature)              \
  PRBool processed = PR_FALSE;                                          \
  nsresult res = NS_OK;                                                 \
  PLUGIN_DEBUG_0ARG ("RECEIVE ID 1\n");                                             \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnIdentifier == -1 &&\
	     resultC->errorOccurred == PR_FALSE)     \
    {                                                                   \
      PROCESS_PENDING_EVENTS_REF (reference);                                \
    }                                                                   \
                                                                        \
  if (resultC->errorOccurred == PR_TRUE)	 	    \
  { \
	  *id = NULL; \
  } else \
  { \
  *id = reinterpret_cast<cast>                                  \
    (new JNIID (resultC->returnIdentifier, signature));         \
   PLUGIN_DEBUG_4ARG ("RECEIVE_ID: %s result: %x = %d, %s\n",               \
           __func__, *id, resultC->returnIdentifier,             \
           signature); \
  }

#define MESSAGE_RECEIVE_VALUE(reference, ctype, result)                    \
  nsresult res = NS_OK;                                                    \
  PLUGIN_DEBUG_0ARG ("RECEIVE VALUE 1\n");                                             \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnValue.IsVoid() == PR_TRUE && \
	     resultC->errorOccurred == PR_FALSE)            \
    {                                                                      \
      PROCESS_PENDING_EVENTS_REF (reference);                                   \
    }                                                                      \
    *result = ParseValue (type, resultC->returnValue);            
// \
//   char* valueString = ValueString (type, *result);              \
//   printf ("RECEIVE_VALUE: %s result: %x = %s\n",                \
//           __func__, result, valueString);                       \
//   free (valueString);                                           \
//   valueString = NULL;

#define MESSAGE_RECEIVE_SIZE(reference, result)                   \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  PLUGIN_DEBUG_0ARG("RECEIVE SIZE 1\n");                                 \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnValue.IsVoid() == PR_TRUE && \
	     resultC->errorOccurred == PR_FALSE) \
    {                                                           \
      PROCESS_PENDING_EVENTS_REF (reference);                        \
    }                                                           \
  nsresult conversionResult;                                    \
  if (resultC->errorOccurred == PR_TRUE) \
	*result = NULL; \
  else \
  { \
    *result = resultC->returnValue.ToInteger (&conversionResult); \
    PLUGIN_CHECK ("parse integer", conversionResult);             \
  }
// \
//   printf ("RECEIVE_SIZE: %s result: %x = %d\n",                 \
//           __func__, result, *result);

// strdup'd string must be freed by calling function.
#define MESSAGE_RECEIVE_STRING(reference, char_type, result)      \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  PLUGIN_DEBUG_0ARG("RECEIVE STRING 1\n");                                 \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnValue.IsVoid() == PR_TRUE && \
	     resultC->errorOccurred == PR_FALSE)  \
    {                                                           \
      PROCESS_PENDING_EVENTS_REF (reference);                        \
    }                                                           \
	if (resultC->errorOccurred == PR_TRUE) \
		*result = NULL; \
	else \
	{\
	  PLUGIN_DEBUG_1ARG("Setting result to: %s\n", strdup (resultC->returnValue.get ())); \
      *result = reinterpret_cast<char_type const*>                  \
                (strdup (resultC->returnValue.get ()));\
	}
// \
//   printf ("RECEIVE_STRING: %s result: %x = %s\n",               \
//           __func__, result, *result);

// strdup'd string must be freed by calling function.
#define MESSAGE_RECEIVE_STRING_UCS(reference, result)             \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  PLUGIN_DEBUG_0ARG("RECEIVE STRING UCS 1\n");                                 \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnValueUCS.IsVoid() == PR_TRUE && \
	     resultC->errorOccurred == PR_FALSE) \
    {                                                           \
      PROCESS_PENDING_EVENTS_REF (reference);                        \
    }                                                           \
	if (resultC->errorOccurred == PR_TRUE) \
		*result = NULL; \
	else \
	{ \
	  int length = resultC->returnValueUCS.Length ();               \
	  jchar* newstring = static_cast<jchar*> (PR_Malloc (length));  \
	  memset (newstring, 0, length);                                \
	  memcpy (newstring, resultC->returnValueUCS.get (), length);   \
	  *result = static_cast<jchar const*> (newstring); \
	}

// \
//   printf ("RECEIVE_STRING: %s result: %x = %s\n",               \
//           __func__, result, *result);

#define MESSAGE_RECEIVE_BOOLEAN(reference, result)                \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  PLUGIN_DEBUG_0ARG("RECEIVE BOOLEAN 1\n");                             \
  ResultContainer *resultC;                                              \
  factory->result_map.Get(reference, &resultC);                         \
  while (resultC->returnIdentifier == -1 && \
	     resultC->errorOccurred == PR_FALSE)               \
    {                                                           \
      PROCESS_PENDING_EVENTS_REF (reference);                        \
    }                                                           \
	if (resultC->errorOccurred == PR_TRUE) \
		*result = NULL; \
	else \
	  *result = resultC->returnIdentifier;
//      res = factory->current->ProcessNextEvent (PR_TRUE,        \
//                                                &processed);    \
//      PLUGIN_CHECK_RETURN (__func__, res);                      \

// \
//   printf ("RECEIVE_BOOLEAN: %s result: %x = %s\n",              \
//           __func__, result, *result ? "true" : "false");

#include <nsISupports.h>
#include <nsIFactory.h>
#include <nscore.h>
#include <prtypes.h>

// Factory functions.
extern "C" NS_EXPORT nsresult NSGetFactory (nsISupports* aServMgr,
                                            nsCID const& aClass,
                                            char const* aClassName,
                                            char const* aContractID,
                                            nsIFactory** aFactory);



#include <nsIFactory.h>
#include <nsIPlugin.h>
#include <nsIJVMManager.h>
#include <nsIJVMConsole.h>
#include <nsIJVMPlugin.h>
#include <nsIInputStream.h>
#include <nsIAsyncInputStream.h>
#include <nsISocketTransport.h>
#include <nsIOutputStream.h>
#include <nsIAsyncInputStream.h>
#include <prthread.h>
#include <nsIThread.h>
#include <nsILocalFile.h>
#include <nsIPluginInstance.h>
#include <nsIPluginInstancePeer.h>
#include <nsIJVMPluginInstance.h>
#include <nsIPluginTagInfo2.h>
#include <nsComponentManagerUtils.h>
#include <nsILocalFile.h>
#include <prthread.h>
#include <nsIEventTarget.h>
// // FIXME: I had to hack dist/include/xpcom/xpcom-config.h to comment
// // out this line: #define HAVE_CPP_2BYTE_WCHAR_T 1 so that
// // nsStringAPI.h would not trigger a compilation assertion failure:
// //
// // dist/include/xpcom/nsStringAPI.h:971: error: size of array ‘arg’
// // is negative
// #include <nsStringAPI.h>

// FIXME: if about:plugins doesn't show this plugin, try:
// export LD_LIBRARY_PATH=/home/fitzsim/sources/mozilla/xpcom/build

// FIXME: if the connection spins, printing:
//
// ICEDTEA PLUGIN: thread 0x84f4a68: wait for connection: process next event
// ICEDTEA PLUGIN: thread 0x84f4a68: Instance::IsConnected
// ICEDTEA PLUGIN: thread 0x84f4a68: Instance::IsConnected return
//
// repeatedly, it means there's a problem with pluginappletviewer.
// Try "make av".

#include <nsClassHashtable.h>
#include <nsDataHashtable.h>
#include <nsAutoPtr.h>

class IcedTeaPluginInstance;
class IcedTeaEventSink;

// TODO:
//
// 1) complete test suite for all used functions.
//
// 2) audit memory allocation/deallocation on C++ side
//
// 3) confirm objects/ids hashmap emptying on Java side

// IcedTeaPlugin cannot be a standard nsIScriptablePlugin.  It must
// conform to Mozilla's expectations since Mozilla treats Java as a
// special case.  See dom/src/base/nsDOMClassInfo.cpp:
// nsHTMLPluginObjElementSH::GetPluginJSObject.

// nsClassHashtable does JNIReference deallocation automatically,
// using nsAutoPtr.
class ReferenceHashtable
  : public nsClassHashtable<nsUint32HashKey, JNIReference>
{
public:
  jobject ReferenceObject (PRUint32);
  jobject ReferenceObject (PRUint32, char const*);
  void UnreferenceObject (PRUint32);
};

jobject
ReferenceHashtable::ReferenceObject (PRUint32 key)
{
  if (key == 0)
    return 0;

  JNIReference* reference;
  Get (key, &reference);
  if (reference == 0)
    {
      reference = new JNIReference (key);
      Put (key, reference);
    }
  reference->count++;
  PLUGIN_DEBUG_3ARG ("INCREMENTED: %d %p to: %d\n", key, reference, reference->count);
  return reinterpret_cast<jobject> (reference);
}

jobject
ReferenceHashtable::ReferenceObject (PRUint32 key, char const* signature)
{
  if (key == 0)
    return 0;

  JNIReference* reference;
  Get (key, &reference);
  if (reference == 0)
    {
      reference = new JNIID (key, signature);
      Put (key, reference);
    }
  reference->count++;
  PLUGIN_DEBUG_3ARG ("INCREMENTED: %d %p to: %d\n", key, reference, reference->count);
  return reinterpret_cast<jobject> (reference);
}

void
ReferenceHashtable::UnreferenceObject (PRUint32 key)
{
  JNIReference* reference;
  Get (key, &reference);
  if (reference != 0)
    {
      reference->count--;
      PLUGIN_DEBUG_3ARG ("DECREMENTED: %d %p to: %d\n", key, reference, reference->count);
      if (reference->count == 0)
        Remove (key);
    }
}

class ResultContainer 
{
	public:
		ResultContainer();
		~ResultContainer();
		void Clear();
		void start_timer();
		void stop_timer();
  		PRUint32 returnIdentifier;
		nsCString returnValue;
		nsString returnValueUCS;
		nsCString errorMessage;
		PRBool errorOccurred;
		suseconds_t time;
};

ResultContainer::ResultContainer () 
{
	returnIdentifier = -1;
	returnValue.Truncate();
	returnValueUCS.Truncate();
	returnValue.SetIsVoid(PR_TRUE);
	returnValueUCS.SetIsVoid(PR_TRUE);
	errorMessage.Truncate();
	errorOccurred = PR_FALSE;

	start_timer();
}

ResultContainer::~ResultContainer ()
{
    returnIdentifier = -1;
	returnValue.Truncate();
	returnValueUCS.Truncate();
	errorMessage.Truncate();

	stop_timer();
}

void
ResultContainer::Clear()
{
	returnIdentifier = -1;
	returnValue.Truncate();
	returnValueUCS.Truncate();
	returnValue.SetIsVoid(PR_TRUE);
	returnValueUCS.SetIsVoid(PR_TRUE);
	errorMessage.Truncate();
	errorOccurred = PR_FALSE;

	start_timer();
}

void
ResultContainer::start_timer()
{
	time = get_time_in_ms();
}


void
ResultContainer::stop_timer()
{
	PLUGIN_DEBUG_1ARG("Time elapsed = %ld\n", get_time_in_ms() - time);
}

#include <nsTArray.h>
#include <nsILiveconnect.h>
#include <nsICollection.h>
#include <nsIProcess.h>

#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

inline void js_id_to_string(char** str, PLUGIN_JAVASCRIPT_TYPE jsid)
{
	if (sizeof(PLUGIN_JAVASCRIPT_TYPE) == 4)
		sprintf(*str, "%"PRId32, jsid);

	if (sizeof(PLUGIN_JAVASCRIPT_TYPE) == 8)
		sprintf(*str, "%"PRId64, jsid);
}

inline PLUGIN_JAVASCRIPT_TYPE string_to_js_id(nsCString str)
{
	if (sizeof(PLUGIN_JAVASCRIPT_TYPE) == sizeof(int))
		return atoi(str.get());

	if (sizeof(PLUGIN_JAVASCRIPT_TYPE) == sizeof(long))
		return atol(str.get());

	if (sizeof(PLUGIN_JAVASCRIPT_TYPE) == sizeof(long long))
		return atoll(str.get());
}

class IcedTeaJNIEnv;

// nsIPlugin inherits from nsIFactory.
class IcedTeaPluginFactory : public nsIPlugin,
                             public nsIJVMManager,
                             public nsIJVMPlugin,
							 public nsIJVMConsole,
                             public nsIInputStreamCallback
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFACTORY
  NS_DECL_NSIPLUGIN
  NS_DECL_NSIJVMMANAGER
  // nsIJVMConsole does not provide an NS_DECL macro.
public:
  NS_IMETHOD Show (void);
  NS_IMETHOD Hide (void);
  NS_IMETHOD IsVisible (PRBool* result);
  NS_IMETHOD Print(const char* msg, const char* encodingName = NULL);
  // nsIJVMPlugin does not provide an NS_DECL macro.
public:
  NS_IMETHOD AddToClassPath (char const* dirPath);
  NS_IMETHOD RemoveFromClassPath (char const* dirPath);
  NS_IMETHOD GetClassPath (char const** result);
  NS_IMETHOD GetJavaWrapper (JNIEnv* jenv, PLUGIN_JAVASCRIPT_TYPE obj, jobject* jobj);
  NS_IMETHOD CreateSecureEnv (JNIEnv* proxyEnv, nsISecureEnv** outSecureEnv);
  NS_IMETHOD SpendTime (PRUint32 timeMillis);
  NS_IMETHOD UnwrapJavaWrapper (JNIEnv* jenv, jobject jobj, PLUGIN_JAVASCRIPT_TYPE* obj);
  NS_DECL_NSIINPUTSTREAMCALLBACK

  IcedTeaPluginFactory();
  nsresult SendMessageToAppletViewer (nsCString& message);
  PRUint32 RegisterInstance (IcedTeaPluginInstance* instance);
  void UnregisterInstance (PRUint32 instance_identifier);
  NS_IMETHOD GetJavaObject (PRUint32 instance_identifier, jobject* object);
  void HandleMessage (nsCString const& message);
  nsresult SetTransport (nsISocketTransport* transport);
  void Connected ();
  void Disconnected ();
  PRBool IsConnected ();
  nsCOMPtr<nsIAsyncInputStream> async;
  nsCOMPtr<nsIThread> current;
  nsCOMPtr<nsIInputStream> input;
  nsCOMPtr<nsIOutputStream> output;
  ReferenceHashtable references;
  PRBool shutting_down;
  // FIXME: make private?
  JNIEnv* proxyEnv;
  nsISecureEnv* secureEnv;
  nsDataHashtable<nsUint32HashKey,ResultContainer*> result_map;

  void InitializeJava();
  void GetMember ();
  void SetMember ();
  void GetSlot ();
  void SetSlot ();
  void Eval ();
  void RemoveMember ();
  void Call ();
  void Finalize ();
  void ToString ();
  void MarkInstancesVoid ();
  nsCOMPtr<nsILiveconnect> liveconnect;

  // normally, we shouldn't have to track unref'd handles, but in some cases, 
  // we may be in the middle of Eval() when finalize is called and completed. 
  // At this point, calling liveconnect->Eval causes bad, bad, bad things 
  // (first observed here after multiple refreshes: 
  // http://www.jigzone.com/puzzles/daily-jigsaw
  nsDataHashtable<nsUint32HashKey, PRBool> js_cleared_handles;

private:
  ~IcedTeaPluginFactory();
  nsresult TestAppletviewer ();
  void DisplayFailureDialog ();
  nsresult StartAppletviewer ();
  void ProcessMessage();
  void ConsumeMsgFromJVM();
  nsresult GetProxyInfo(const char* siteAddr, char** proxyScheme, char** proxyHost, char** proxyPort);
  nsresult GetCookieInfo(const char* siteAddr, char** cookieString);
  nsCOMPtr<IcedTeaEventSink> sink;
  nsCOMPtr<nsISocketTransport> transport;
  nsCOMPtr<nsIProcess> applet_viewer_process;
  PRBool connected;
  PRUint32 next_instance_identifier;
  PRUint32 object_identifier_return;
  PRUint32 instance_count;
  PLUGIN_JAVASCRIPT_TYPE javascript_identifier;
  int name_identifier;
  int args_identifier;
  int string_identifier;
  int slot_index;
  int value_identifier;
  // Does not do construction/deconstruction or reference counting.
  nsDataHashtable<nsUint32HashKey, IcedTeaPluginInstance*> instances;

  // Applet viewer input pipe name.
  gchar* in_pipe_name;
  // Applet viewer input watch source.
  gint in_watch_source;
  // Applet viewer output pipe name.
  gchar* out_pipe_name;
  // Applet viewer output watch source.
  gint out_watch_source;
  // Applet viewer output channel.
  GIOChannel* out_to_appletviewer;
};

class IcedTeaEventSink;

class IcedTeaPluginInstance : public nsIPluginInstance,
                              public nsIJVMPluginInstance
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPLUGININSTANCE
  NS_DECL_NSIJVMPLUGININSTANCE

  IcedTeaPluginInstance (IcedTeaPluginFactory* factory);
  ~IcedTeaPluginInstance ();

  void GetWindow ();

  nsIPluginInstancePeer* peer;
  PRBool initialized;
  PRBool fatalErrorOccurred;

private:

  PLUGIN_JAVASCRIPT_TYPE liveconnect_window;
  gpointer window_handle;
  guint32 window_width;
  guint32 window_height;
  PRBool is_active;
  // FIXME: nsCOMPtr.
  IcedTeaPluginFactory* factory;
  PRUint32 instance_identifier;
  nsCString instanceIdentifierPrefix;
};


#include <nsISocketProviderService.h>
#include <nsISocketProvider.h>
#include <nsIServerSocket.h>
#include <nsIComponentManager.h>
#include <nsIPluginInstance.h>
#include <sys/types.h>
#include <sys/stat.h>

class IcedTeaSocketListener : public nsIServerSocketListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISERVERSOCKETLISTENER

  IcedTeaSocketListener (IcedTeaPluginFactory* factory);

private:
  ~IcedTeaSocketListener ();
  IcedTeaPluginFactory* factory;
protected:
};

#include <nsITransport.h>

class IcedTeaEventSink : public nsITransportEventSink
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITRANSPORTEVENTSINK

  IcedTeaEventSink();

private:
  ~IcedTeaEventSink();
};

#include <nsISupports.h>
#include <nsISecureEnv.h>
#include <nsISecurityContext.h>
#include <nsIServerSocket.h>
#include <nsNetCID.h>
#include <nsThreadUtils.h>
#include <nsIThreadManager.h>
#include <nsIThread.h>
#include <nsXPCOMCIDInternal.h>

class IcedTeaJNIEnv : public nsISecureEnv
{
  NS_DECL_ISUPPORTS

  // nsISecureEnv does not provide an NS_DECL macro.
public:
  IcedTeaJNIEnv (IcedTeaPluginFactory* factory);

  NS_IMETHOD NewObject (jclass clazz,
                        jmethodID methodID,
                        jvalue* args,
                        jobject* result,
                        nsISecurityContext* ctx = NULL);

  NS_IMETHOD CallMethod (jni_type type,
                         jobject obj,
                         jmethodID methodID,
                         jvalue* args,
                         jvalue* result,
                         nsISecurityContext* ctx = NULL);

  NS_IMETHOD CallNonvirtualMethod (jni_type type,
                                   jobject obj,
                                   jclass clazz,
                                   jmethodID methodID,
                                   jvalue* args,
                                   jvalue* result,
                                   nsISecurityContext* ctx = NULL);

  NS_IMETHOD GetField (jni_type type,
                       jobject obj,
                       jfieldID fieldID,
                       jvalue* result,
                       nsISecurityContext* ctx = NULL);

  NS_IMETHOD SetField (jni_type type,
                       jobject obj,
                       jfieldID fieldID,
                       jvalue val,
                       nsISecurityContext* ctx = NULL);

  NS_IMETHOD CallStaticMethod (jni_type type,
                               jclass clazz,
                               jmethodID methodID,
                               jvalue* args,
                               jvalue* result,
                               nsISecurityContext* ctx = NULL);

  NS_IMETHOD GetStaticField (jni_type type,
                             jclass clazz,
                             jfieldID fieldID,
                             jvalue* result,
                             nsISecurityContext* ctx = NULL);


  NS_IMETHOD SetStaticField (jni_type type,
                             jclass clazz,
                             jfieldID fieldID,
                             jvalue val,
                             nsISecurityContext* ctx = NULL);


  NS_IMETHOD GetVersion (jint* version);

  NS_IMETHOD DefineClass (char const* name,
                          jobject loader,
                          jbyte const* buf,
                          jsize len,
                          jclass* clazz);

  NS_IMETHOD FindClass (char const* name,
                        jclass* clazz);

  NS_IMETHOD GetSuperclass (jclass sub,
                            jclass* super);

  NS_IMETHOD IsAssignableFrom (jclass sub,
                               jclass super,
                               jboolean* result);

  NS_IMETHOD Throw (jthrowable obj,
                    jint* result);

  NS_IMETHOD ThrowNew (jclass clazz,
                       char const* msg,
                       jint* result);

  NS_IMETHOD ExceptionOccurred (jthrowable* result);

  NS_IMETHOD ExceptionDescribe (void);

  NS_IMETHOD ExceptionClear (void);

  NS_IMETHOD FatalError (char const* msg);

  NS_IMETHOD NewGlobalRef (jobject lobj,
                           jobject* result);

  NS_IMETHOD DeleteGlobalRef (jobject gref);

  NS_IMETHOD DeleteLocalRef (jobject obj);

  NS_IMETHOD IsSameObject (jobject obj1,
                           jobject obj2,
                           jboolean* result);

  NS_IMETHOD AllocObject (jclass clazz,
                          jobject* result);

  NS_IMETHOD GetObjectClass (jobject obj,
                             jclass* result);

  NS_IMETHOD IsInstanceOf (jobject obj,
                           jclass clazz,
                           jboolean* result);

  NS_IMETHOD GetMethodID (jclass clazz,
                          char const* name,
                          char const* sig,
                          jmethodID* id);

  NS_IMETHOD GetFieldID (jclass clazz,
                         char const* name,
                         char const* sig,
                         jfieldID* id);

  NS_IMETHOD GetStaticMethodID (jclass clazz,
                                char const* name,
                                char const* sig,
                                jmethodID* id);

  NS_IMETHOD GetStaticFieldID (jclass clazz,
                               char const* name,
                               char const* sig,
                               jfieldID* id);

  NS_IMETHOD NewString (jchar const* unicode,
                        jsize len,
                        jstring* result);

  NS_IMETHOD GetStringLength (jstring str,
                              jsize* result);

  NS_IMETHOD GetStringChars (jstring str,
                             jboolean* isCopy,
                             jchar const** result);

  NS_IMETHOD ReleaseStringChars (jstring str,
                                 jchar const* chars);

  NS_IMETHOD NewStringUTF (char const* utf,
                           jstring* result);

  NS_IMETHOD GetStringUTFLength (jstring str,
                                 jsize* result);

  NS_IMETHOD GetStringUTFChars (jstring str,
                                jboolean* isCopy,
                                char const** result);

  NS_IMETHOD ReleaseStringUTFChars (jstring str,
                                    char const* chars);

  NS_IMETHOD GetArrayLength (jarray array,
                             jsize* result);

  NS_IMETHOD NewObjectArray (jsize len,
                             jclass clazz,
                             jobject init,
                             jobjectArray* result);

  NS_IMETHOD GetObjectArrayElement (jobjectArray array,
                                    jsize index,
                                    jobject* result);

  NS_IMETHOD SetObjectArrayElement (jobjectArray array,
                                    jsize index,
                                    jobject val);

  NS_IMETHOD NewArray (jni_type element_type,
                       jsize len,
                       jarray* result);

  NS_IMETHOD GetArrayElements (jni_type type,
                               jarray array,
                               jboolean* isCopy,
                               void* result);

  NS_IMETHOD ReleaseArrayElements (jni_type type,
                                   jarray array,
                                   void* elems,
                                   jint mode);

  NS_IMETHOD GetArrayRegion (jni_type type,
                             jarray array,
                             jsize start,
                             jsize len,
                             void* buf);

  NS_IMETHOD SetArrayRegion (jni_type type,
                             jarray array,
                             jsize start,
                             jsize len,
                             void* buf);

  NS_IMETHOD RegisterNatives (jclass clazz,
                              JNINativeMethod const* methods,
                              jint nMethods,
                              jint* result);

  NS_IMETHOD UnregisterNatives (jclass clazz,
                                jint* result);

  NS_IMETHOD MonitorEnter (jobject obj,
                           jint* result);

  NS_IMETHOD MonitorExit (jobject obj,
                          jint* result);

  NS_IMETHOD GetJavaVM (JavaVM** vm,
                        jint* result);

  jvalue ParseValue (jni_type type, nsCString& str);
  char* ExpandArgs (JNIID* id, jvalue* args);
  char* ValueString (jni_type type, jvalue value);

private:
  ~IcedTeaJNIEnv ();

  IcedTeaPluginFactory* factory;

  PRMonitor *contextCounterPRMonitor;

  int IncrementContextCounter();
  void DecrementContextCounter();
  nsresult GetCurrentContextAddr(char *addr);
  nsresult GetCurrentPageAddress(const char **addr);
  nsresult GetEnabledPrivileges(nsCString *privileges, nsISecurityContext *ctx);
  int contextCounter;
};


#include <nsIServerSocket.h>
#include <nsNetError.h>
#include <nsPIPluginInstancePeer.h>
#include <nsIPluginInstanceOwner.h>
#include <nsIRunnable.h>

class IcedTeaRunnable : public nsIRunnable
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIRUNNABLE

  IcedTeaRunnable ();

  ~IcedTeaRunnable ();
};

NS_IMPL_ISUPPORTS1 (IcedTeaRunnable, nsIRunnable)

IcedTeaRunnable::IcedTeaRunnable ()
{
}

IcedTeaRunnable::~IcedTeaRunnable ()
{
}

NS_IMETHODIMP
IcedTeaRunnable::Run ()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

template <class T>
class IcedTeaRunnableMethod : public IcedTeaRunnable
{
public:
  typedef void (T::*Method) ();

  IcedTeaRunnableMethod (T* object, Method method);
  NS_IMETHOD Run ();

  ~IcedTeaRunnableMethod ();

  T* object;
  Method method;
};

template <class T>
IcedTeaRunnableMethod<T>::IcedTeaRunnableMethod (T* object, Method method)
: object (object),
  method (method)
{
  NS_ADDREF (object);
}

template <class T>
IcedTeaRunnableMethod<T>::~IcedTeaRunnableMethod ()
{
  NS_RELEASE (object);
}

template <class T> NS_IMETHODIMP
IcedTeaRunnableMethod<T>::Run ()
{
    (object->*method) ();
    return NS_OK;
}


// FIXME: Special class just for dispatching GetURL to another 
// thread.. seriously, a class just for that? there has to be a better way!

class GetURLRunnable : public nsIRunnable
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIRUNNABLE

  GetURLRunnable (nsIPluginInstancePeer* peer, nsCString url, nsCString target);

  ~GetURLRunnable ();

private:
  nsIPluginInstancePeer* peer;
  nsCString url;
  nsCString target;
};

NS_IMPL_ISUPPORTS1 (GetURLRunnable, nsIRunnable)

GetURLRunnable::GetURLRunnable (nsIPluginInstancePeer* peer, nsCString url, nsCString target)
: peer(peer),
  url(url),
  target(target)
{
    NS_ADDREF (peer);
}

GetURLRunnable::~GetURLRunnable ()
{
  NS_RELEASE(peer);
}

NS_IMETHODIMP
GetURLRunnable::Run ()
{
   nsCOMPtr<nsPIPluginInstancePeer> ownerGetter =
                do_QueryInterface (peer);
   nsIPluginInstanceOwner* owner = nsnull;
   ownerGetter->GetOwner (&owner);

   return owner->GetURL ((const char*) url.get(), (const char*) target.get(),
                         nsnull, 0, nsnull, 0);
}

NS_IMPL_ISUPPORTS6 (IcedTeaPluginFactory, nsIFactory, nsIPlugin, nsIJVMManager,
                    nsIJVMPlugin, nsIJVMConsole, nsIInputStreamCallback)

// IcedTeaPluginFactory functions.
IcedTeaPluginFactory::IcedTeaPluginFactory ()
: next_instance_identifier (1),
  proxyEnv (0),
  javascript_identifier (0),
  name_identifier (0),
  args_identifier (0),
  string_identifier (0),
  slot_index (0),
  value_identifier (0),
  connected (PR_FALSE),
  liveconnect (0),
  instance_count(0),
  shutting_down(PR_FALSE),
  in_pipe_name(NULL),
  in_watch_source(NULL),
  out_pipe_name(NULL),
  out_watch_source(NULL),
  out_to_appletviewer(NULL)
{
  plugin_debug = getenv ("ICEDTEAPLUGIN_DEBUG") != NULL;
  PLUGIN_TRACE_FACTORY ();
  instances.Init ();
  references.Init ();
  js_cleared_handles.Init();
  result_map.Init();
  PLUGIN_DEBUG_0ARG ("CONSTRUCTING FACTORY\n");
  PLUGIN_DEBUG_1ARG("ICEDTEAPLUGIN_DEBUG = %s\n", getenv ("ICEDTEAPLUGIN_DEBUG"));
}

IcedTeaPluginFactory::~IcedTeaPluginFactory ()
{
  // FIXME: why did this crash with threadManager == 0x0 on shutdown?
  PLUGIN_TRACE_FACTORY ();
  secureEnv = 0;
  factory_created = PR_FALSE;
  factory = NULL;
  PLUGIN_DEBUG_0ARG ("DECONSTRUCTING FACTORY\n");

  // Removing a source is harmless if it fails since it just means the
  // source has already been removed.
  if (in_watch_source)
    g_source_remove (in_watch_source);
  in_watch_source = 0;

  // free input channel
  if (in_from_appletviewer)
    g_io_channel_unref (in_from_appletviewer);
  in_from_appletviewer = NULL;

  // cleanup_out_watch_source:
  if (out_watch_source)
    g_source_remove (out_watch_source);
  out_watch_source = 0;

  // free output channel
  if (out_to_appletviewer)
    g_io_channel_unref (out_to_appletviewer);
  out_to_appletviewer = NULL;

  // free its memory
  if (out_pipe_name)
  {
    // Delete output pipe.
    unlink (out_pipe_name);

    g_free (out_pipe_name);
    out_pipe_name = NULL;
  }

  if (in_pipe_name)
  {
    // Delete input pipe.
    unlink (in_pipe_name);

    // free its memory
    g_free (in_pipe_name);
    in_pipe_name = NULL;
  }
}

// nsIFactory functions.
NS_IMETHODIMP
IcedTeaPluginFactory::CreateInstance (nsISupports* aOuter, nsIID const& iid,
                                      void** result)
{
  PLUGIN_TRACE_FACTORY ();
  if (!result)
    return NS_ERROR_NULL_POINTER;

  *result = NULL;

  IcedTeaPluginInstance* instance = new IcedTeaPluginInstance (this);

  if (!instance)
    return NS_ERROR_OUT_OF_MEMORY;

  instance_count++;
  return instance->QueryInterface (iid, result);
}

NS_IMETHODIMP
IcedTeaPluginFactory::LockFactory (PRBool lock)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// nsIPlugin functions.
NS_IMETHODIMP
IcedTeaPluginFactory::CreatePluginInstance (nsISupports* aOuter,
                                            nsIID const& aIID,
                                            char const* aPluginMIMEType,
                                            void** aResult)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::Initialize ()
{
  PLUGIN_TRACE_FACTORY ();
  nsresult result = NS_OK;

  PLUGIN_DEBUG_TWO ("Factory::Initialize: using", appletviewer_executable);

  nsCOMPtr<nsIComponentManager> manager;
  result = NS_GetComponentManager (getter_AddRefs (manager));

  jvmMsgQueuePRMonitor = PR_NewMonitor();

  nsCOMPtr<nsIThreadManager> threadManager;
  result = manager->CreateInstanceByContractID
    (NS_THREADMANAGER_CONTRACTID, nsnull, NS_GET_IID (nsIThreadManager),
     getter_AddRefs (threadManager));
  PLUGIN_CHECK_RETURN ("thread manager", result);

  result = threadManager->GetCurrentThread (getter_AddRefs (current));
  PLUGIN_CHECK_RETURN ("current thread", result);

  if (jvm_attached == PR_FALSE)
  {
    // using printf on purpose.. this should happen rarely
    PLUGIN_DEBUG_0ARG("Initializing JVM...\n");

    // mark attached right away, in case another initialize() call 
    //is made (happens if multiple applets are present on the same page)
    jvm_attached = PR_TRUE;
    InitializeJava();
  }

  return NS_OK;
}

void
IcedTeaPluginFactory::InitializeJava ()
{

  PRBool processed;
  nsresult result;

  // Start appletviewer process for this plugin instance.
  nsCOMPtr<nsIComponentManager> manager;
  result = NS_GetComponentManager (getter_AddRefs (manager));
  PLUGIN_CHECK ("get component manager", result);

  result = manager->CreateInstance
    (nsILiveconnect::GetCID (),
     nsnull, NS_GET_IID (nsILiveconnect),
     getter_AddRefs (liveconnect));
  PLUGIN_CHECK ("liveconnect", result);

  nsCOMPtr<nsIThreadManager> threadManager;
  nsCOMPtr<nsIThread> curr_thread;
  result = manager->CreateInstanceByContractID
    (NS_THREADMANAGER_CONTRACTID, nsnull, NS_GET_IID (nsIThreadManager),
     getter_AddRefs (threadManager));
  PLUGIN_CHECK ("thread manager", result);

  result = threadManager->GetCurrentThread (getter_AddRefs (curr_thread));

  result = StartAppletviewer ();
  PLUGIN_CHECK ("started appletviewer", result);
}

void
IcedTeaPluginFactory::MarkInstancesVoid ()
{
      PLUGIN_TRACE_FACTORY ();
	
      IcedTeaPluginInstance* instance = NULL;

      int instance_id = 1;

      while (instance_id <= instance_count)
	  {
		if (instances.Get(instance_id, &instance))
		{
            PLUGIN_DEBUG_2ARG("Marking %d of %d void\n", instance_id, instance_count);
            instance->fatalErrorOccurred = PR_TRUE;
	    }
		instance_id++;
	  }
}

NS_IMETHODIMP
IcedTeaPluginFactory::Shutdown ()
{
  shutting_down = PR_TRUE;

  nsCString shutdownStr("shutdown");
  SendMessageToAppletViewer(shutdownStr);

  // wake up process thread to tell it to shutdown
  PRThread *prThread;
  processThread->GetPRThread(&prThread);
  PLUGIN_DEBUG_0ARG ("Interrupting process thread...");
  PRStatus res = PR_Interrupt(prThread);
  PLUGIN_DEBUG_0ARG (" done!\n");

  PRInt32 exitVal;
  applet_viewer_process->GetExitValue(&exitVal);

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetMIMEDescription (char const** aMIMEDescription)
{
  PLUGIN_TRACE_FACTORY ();
  *aMIMEDescription = PLUGIN_MIME_DESC;

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetValue (nsPluginVariable aVariable, void* aValue)
{
  PLUGIN_TRACE_FACTORY ();
  nsresult result = NS_OK;

  switch (aVariable)
    {
    case nsPluginVariable_NameString:
      *static_cast<char const**> (aValue) = PLUGIN_NAME;
      break;
    case nsPluginVariable_DescriptionString:
      *static_cast<char const**> (aValue) = PLUGIN_DESCRIPTION;
      break;
    default:
      PLUGIN_ERROR ("Unknown plugin value requested.");
      result = NS_ERROR_INVALID_ARG;
      break;
    }

  return result;
}

// nsIJVMManager functions.
NS_IMETHODIMP
IcedTeaPluginFactory::CreateProxyJNI (nsISecureEnv* secureEnv,
                                      JNIEnv** outProxyEnv)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetProxyJNI (JNIEnv** outProxyEnv)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::ShowJavaConsole ()
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::IsAllPermissionGranted (char const* lastFingerprint,
                                              char const* lastCommonName,
                                              char const* rootFingerprint,
                                              char const* rootCommonName,
                                              PRBool* _retval)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::IsAppletTrusted (char const* aRSABuf, PRUint32 aRSABufLen,
                                       char const* aPlaintext,
                                       PRUint32 aPlaintextLen,
                                       PRBool* isTrusted,
                                       nsIPrincipal**_retval)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetJavaEnabled (PRBool* aJavaEnabled)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

#include <typeinfo>

// nsIJVMConsole functions.
NS_IMETHODIMP
IcedTeaPluginFactory::Show (void)
{
  nsCString msg("plugin showconsole");
  this->SendMessageToAppletViewer(msg);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::Hide (void)
{
  nsCString msg("plugin hideconsole");
  this->SendMessageToAppletViewer(msg);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::IsVisible(PRBool* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::AddToClassPath (char const* dirPath)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::RemoveFromClassPath (char const* dirPath)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetClassPath (char const** result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetJavaWrapper (JNIEnv* jenv, PLUGIN_JAVASCRIPT_TYPE obj,
                                      jobject* jobj)
{
  jclass clazz;
  jmethodID method;
  jobject newobject;
  jvalue args[1];
  secureEnv->FindClass ("netscape.javascript.JSObject", &clazz);
  secureEnv->GetMethodID (clazz, "<init>",
                          PLUGIN_JAVASCRIPT_SIGNATURE, &method);
  PLUGIN_INITIALIZE_JAVASCRIPT_ARGUMENT(args, obj);
  secureEnv->NewObject (clazz, method, args, &newobject);
  *jobj = newobject;
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::CreateSecureEnv (JNIEnv* proxyEnv,
                                       nsISecureEnv** outSecureEnv)
{
  PLUGIN_TRACE_FACTORY ();
  *outSecureEnv = new IcedTeaJNIEnv (this);
  secureEnv = *outSecureEnv;
  IcedTeaPluginFactory::proxyEnv = proxyEnv;

  jclass clazz;
  jclass stringclazz;
  jmethodID method;
  jfieldID field;
  jvalue result;
  jvalue val;
  jvalue args[1];
  jarray array;
  jobjectArray objectarray;
  jobject newobject;
  jobject newglobalobject;
  jsize length;
  jboolean isCopy;
  char const* str;
  jchar const* jstr;
  jclass superclazz;
  jclass resultclazz;
  jboolean resultbool;

  PLUGIN_DEBUG_0ARG ("CREATESECUREENV\n");
#if 0

  // IcedTeaJNIEnv::AllocObject
  // IcedTeaJNIEnv::CallMethod
  // IcedTeaJNIEnv::CallNonvirtualMethod
  // IcedTeaJNIEnv::CallStaticMethod
  // IcedTeaJNIEnv::DefineClass
  // IcedTeaJNIEnv::DeleteGlobalRef
  // IcedTeaJNIEnv::DeleteLocalRef
  // IcedTeaJNIEnv::ExceptionClear
  // IcedTeaJNIEnv::ExceptionDescribe
  // IcedTeaJNIEnv::ExceptionOccurred
  // IcedTeaJNIEnv::ExpandArgs
  // IcedTeaJNIEnv::FatalError
  // IcedTeaJNIEnv::FindClass
  // IcedTeaJNIEnv::GetArrayElements
  // IcedTeaJNIEnv::GetArrayLength
  // IcedTeaJNIEnv::GetArrayRegion
  // IcedTeaJNIEnv::GetFieldID
  // IcedTeaJNIEnv::GetField
  // IcedTeaJNIEnv::GetJavaVM
  // IcedTeaJNIEnv::GetMethodID
  // IcedTeaJNIEnv::GetObjectArrayElement
  // IcedTeaJNIEnv::GetObjectClass
  // IcedTeaJNIEnv::GetStaticFieldID
  // IcedTeaJNIEnv::GetStaticField
  // IcedTeaJNIEnv::GetStaticMethodID
  // IcedTeaJNIEnv::GetStringChars
  // IcedTeaJNIEnv::GetStringLength
  // IcedTeaJNIEnv::GetStringUTFChars
  // IcedTeaJNIEnv::GetStringUTFLength
  // IcedTeaJNIEnv::GetSuperclass
  // IcedTeaJNIEnv::GetVersion
  // IcedTeaJNIEnv::~IcedTeaJNIEnv
  // IcedTeaJNIEnv::IcedTeaJNIEnv
  // IcedTeaJNIEnv::IsAssignableFrom
  // IcedTeaJNIEnv::IsInstanceOf
  // IcedTeaJNIEnv::IsSameObject
  // IcedTeaJNIEnv::MonitorEnter
  // IcedTeaJNIEnv::MonitorExit
  // IcedTeaJNIEnv::NewArray
  // IcedTeaJNIEnv::NewGlobalRef
  // IcedTeaJNIEnv::NewObjectArray
  // IcedTeaJNIEnv::NewObject
  // IcedTeaJNIEnv::NewString
  // IcedTeaJNIEnv::NewStringUTF
  // IcedTeaJNIEnv::ParseValue
  // IcedTeaJNIEnv::RegisterNatives
  // IcedTeaJNIEnv::ReleaseArrayElements
  // IcedTeaJNIEnv::ReleaseStringChars
  // IcedTeaJNIEnv::ReleaseStringUTFChars
  // IcedTeaJNIEnv::SetArrayRegion
  // IcedTeaJNIEnv::SetField
  // IcedTeaJNIEnv::SetObjectArrayElement
  // IcedTeaJNIEnv::SetStaticField
  // IcedTeaJNIEnv::Throw
  // IcedTeaJNIEnv::ThrowNew
  // IcedTeaJNIEnv::UnregisterNatives
  // IcedTeaJNIEnv::ValueString

  (*outSecureEnv)->FindClass ("sun.applet.TestEnv", &clazz);
  (*outSecureEnv)->GetStaticMethodID (clazz, "TestIt", "()V", &method);
  result.i = -1;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, NULL, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (void)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItBool", "(Z)V", &method);
  args[0].z = 1;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (bool)");
  args[0].z = 0;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (bool)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItByte", "(B)V", &method);
  args[0].b = 0x35;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (byte)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItChar", "(C)V", &method);
  args[0].c = 'a';
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (char)");
  args[0].c = 'T';
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (char)");
  args[0].c = static_cast<jchar> (0x6C34);
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (char)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItShort", "(S)V", &method);
  args[0].s = 254;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (short)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItInt", "(I)V", &method);
  args[0].i = 68477325;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (int)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItLong", "(J)V", &method);
  args[0].j = 268435455;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (long)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItFloat", "(F)V", &method);
  args[0].f = 2.6843;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (float)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItDouble", "(D)V", &method);
  args[0].d = 3.6843E32;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (double)");

  (*outSecureEnv)->GetMethodID (clazz, "<init>", "()V", &method);
  PLUGIN_DEBUG_0ARG ("HERE1\n");
  (*outSecureEnv)->NewObject (clazz, method, NULL, &newobject);
  PLUGIN_DEBUG_0ARG ("HERE2\n");
  (*outSecureEnv)->IsSameObject (newobject, newobject, &resultbool);
  PLUGIN_TEST (resultbool, "IsSameObject: obj, obj");
  (*outSecureEnv)->IsSameObject (newobject, NULL, &resultbool);
  PLUGIN_TEST (!resultbool, "IsSameObject: obj, NULL");
  (*outSecureEnv)->IsSameObject (NULL, newobject, &resultbool);
  PLUGIN_TEST (!resultbool, "IsSameObject: NULL, obj");
  (*outSecureEnv)->IsSameObject (NULL, NULL, &resultbool);
  PLUGIN_TEST (resultbool, "IsSameObject: NULL, NULL");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObject",
                                      "(Lsun/applet/TestEnv;)V", &method);
  args[0].l = newobject;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (object)");

  
  PLUGIN_DEBUG_0ARG ("HERE3\n");
  (*outSecureEnv)->NewGlobalRef (newobject, &newglobalobject);
  PLUGIN_DEBUG_0ARG ("HERE4\n");
  (*outSecureEnv)->DeleteLocalRef (newobject);
  PLUGIN_DEBUG_0ARG ("HERE5\n");
  (*outSecureEnv)->DeleteGlobalRef (newglobalobject);
  PLUGIN_DEBUG_0ARG ("HERE6\n");

  (*outSecureEnv)->NewArray (jint_type, 10, &array);
  (*outSecureEnv)->GetArrayLength (array, &length);
  PLUGIN_TEST (length == 10, "GetArrayLength");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItIntArray", "([I)V",
                                      &method);
  args[0].l = array;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (int_array)");

  (*outSecureEnv)->FindClass ("java.lang.String", &stringclazz);
  (*outSecureEnv)->NewObjectArray (10, stringclazz, NULL, &objectarray);
  (*outSecureEnv)->GetMethodID (stringclazz, "<init>", "()V", &method);
  (*outSecureEnv)->NewObject (stringclazz, method, NULL, &newobject);
  (*outSecureEnv)->SetObjectArrayElement (objectarray, 3, newobject);

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectArray",
                                      "([Ljava/lang/String;)V", &method);
  args[0].l = objectarray;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (result.i == 0, "CallStaticMethod: static void (object_array)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItBoolReturnTrue", "()Z",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jboolean_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.z == JNI_TRUE, "CallStaticMethod: static bool (void)");
  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItBoolReturnFalse", "()Z",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jboolean_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.z == JNI_FALSE, "CallStaticMethod: static bool (void)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItByteReturn", "()B",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jbyte_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.b == static_cast<jbyte> (0xfe),
               "CallStaticMethod: static byte (void)");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItCharReturn", "()C",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jchar_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.c == 'K', "CHAR STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItCharUnicodeReturn", "()C",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jchar_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.c == static_cast<jchar> (0x6C34), "char static void: 0x6c34");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItShortReturn", "()S",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jshort_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.s == static_cast<jshort> (23), "SHORT STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItIntReturn", "()I",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jint_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.i == 3445, "INT STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItLongReturn", "()J",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jlong_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.j == 3242883, "LONG STATIC VOID");
  

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItFloatReturn", "()F",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jfloat_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.f == 9.21E4f, "FLOAT STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItDoubleReturn", "()D",
                                      &method);
  (*outSecureEnv)->CallStaticMethod (jdouble_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (result.d == 8.33E88, "DOUBLE STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectReturn",
                                      "()Ljava/lang/Object;", &method);
  (*outSecureEnv)->CallStaticMethod (jobject_type, clazz, method, NULL,
                                     &result);

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectString",
                                      "(Ljava/lang/String;)V", &method);
  args[0].l = result.l;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  // FIXME:
  PLUGIN_TEST (1, "RETURNED OBJECT");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItIntArrayReturn", "()[I",
                                      &method);
  PLUGIN_DEBUG_1ARG ("GOT METHOD: %d\n", reinterpret_cast<JNIID*> (method)->identifier);
  (*outSecureEnv)->CallStaticMethod (jobject_type, clazz, method, NULL,
                                     &result);
  // FIXME:
  PLUGIN_TEST (1, "INT ARRAY STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItIntArray", "([I)V",
                                      &method);
  args[0].l = result.l;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (1, "RETURNED INT ARRAY");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectArrayReturn",
                                      "()[Ljava/lang/String;", &method);
  (*outSecureEnv)->CallStaticMethod (jobject_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (1, "OBJECT ARRAY STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectArray",
                                      "([Ljava/lang/String;)V", &method);
  args[0].l = result.l;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (1, "RETURNED OBJECT ARRAY");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectArrayMultiReturn",
                                      "()[[Ljava/lang/String;", &method);
  (*outSecureEnv)->CallStaticMethod (jobject_type, clazz, method, NULL,
                                     &result);
  PLUGIN_TEST (1, "OBJECT MULTIDIMENTIONAL ARRAY STATIC VOID");

  (*outSecureEnv)->GetStaticMethodID (clazz, "TestItObjectArrayMulti",
                                      "([[Ljava/lang/String;)V", &method);
  args[0].l = result.l;
  (*outSecureEnv)->CallStaticMethod (jvoid_type, clazz, method, args, &result);
  PLUGIN_TEST (1, "RETURNED OBJECT MULTIDIMENTIONAL ARRAY");

  (*outSecureEnv)->GetStaticFieldID (clazz, "intField", "I",
                                     &field);
  (*outSecureEnv)->GetStaticField (jint_type, clazz, field, &result);
  val.i = 788;
  (*outSecureEnv)->SetStaticField (jint_type, clazz, field, val);
  (*outSecureEnv)->GetStaticField (jint_type, clazz, field, &result);
  PLUGIN_TEST (1, "STATIC INT");
  PLUGIN_TEST (result.i == 788, "OBJECT STATIC VOID");

  (*outSecureEnv)->GetMethodID (clazz, "<init>", "()V", &method);
  (*outSecureEnv)->NewObject (clazz, method, NULL, &newobject);
  (*outSecureEnv)->GetMethodID (clazz, "TestItIntInstance", "(I)I", &method);
  args[0].i = 6322;
  (*outSecureEnv)->CallMethod (jint_type, newobject, method, args, &result);
  PLUGIN_TEST (result.i == 899, "NEW OBJECT");

  (*outSecureEnv)->GetFieldID (clazz, "intInstanceField", "I", &field);
  (*outSecureEnv)->GetField (jint_type, newobject, field, &result);
  val.i = 3224;
  (*outSecureEnv)->SetField (jint_type, newobject, field, val);
  (*outSecureEnv)->GetField (jint_type, newobject, field, &result);
  PLUGIN_TEST (result.i == 3224, "int field: 3224");

  (*outSecureEnv)->GetFieldID (clazz, "stringField",
                               "Ljava/lang/String;", &field);
  (*outSecureEnv)->GetField (jobject_type, newobject, field, &result);
  (*outSecureEnv)->GetStringUTFLength (
    reinterpret_cast<jstring> (result.l), &length);
  PLUGIN_TEST (length == 5, "UTF-8 STRING");
  (*outSecureEnv)->GetStringUTFChars (
    reinterpret_cast<jstring> (result.l), &isCopy, &str);
  PLUGIN_TEST (!strcmp (str, "hello"), "HI");

  (*outSecureEnv)->GetFieldID (clazz, "complexStringField",
                               "Ljava/lang/String;", &field);
  (*outSecureEnv)->GetField (jobject_type, newobject, field, &result);
  (*outSecureEnv)->GetStringUTFLength (
    reinterpret_cast<jstring> (result.l), &length);
  PLUGIN_TEST (length == 4, "HI");
  (*outSecureEnv)->GetStringUTFChars (
    reinterpret_cast<jstring> (result.l), &isCopy, &str);
  char expected1[8] = { 0x7A, 0xF0, 0x9D, 0x84, 0x9E, 0xE6, 0xB0, 0xB4 };
  PLUGIN_TEST (!memcmp (str, expected1, 8),
               "string field: 0x7A, 0xF0, 0x9D, 0x84, 0x9E, 0xE6, 0xB0, 0xB4");

  (*outSecureEnv)->GetFieldID (clazz, "stringField",
                               "Ljava/lang/String;", &field);
  (*outSecureEnv)->GetField (jobject_type, newobject, field, &result);
  (*outSecureEnv)->GetStringLength (
    reinterpret_cast<jstring> (result.l), &length);
  PLUGIN_TEST (length == 5, "UTF-16 STRING");
  (*outSecureEnv)->GetStringChars (
    reinterpret_cast<jstring> (result.l), &isCopy, &jstr);
  char expected2[10] = { 'h', 0x0, 'e', 0x0, 'l', 0x0, 'l', 0x0, 'o', 0x0 };
  PLUGIN_TEST (!memcmp (jstr, expected2, 10), "string field: hello");

  (*outSecureEnv)->GetFieldID (clazz, "complexStringField",
                               "Ljava/lang/String;", &field);
  (*outSecureEnv)->GetField (jobject_type, newobject, field, &result);
  (*outSecureEnv)->GetStringLength (
    reinterpret_cast<jstring> (result.l), &length);
  PLUGIN_TEST (length == 4, "HI");
  (*outSecureEnv)->GetStringChars (
    reinterpret_cast<jstring> (result.l), &isCopy, &jstr);
  char expected3[8] = { 0x7A, 0x00, 0x34, 0xD8, 0x1E, 0xDD, 0x34, 0x6C };
  PLUGIN_TEST (!memcmp (jstr, expected3, 8),
               "string field: 0x7A, 0x00, 0x34, 0xD8, 0x1E, 0xDD, 0x34, 0x6C");

  (*outSecureEnv)->FindClass ("java.awt.Container", &clazz);
  (*outSecureEnv)->FindClass ("java.awt.Component", &superclazz);
  (*outSecureEnv)->GetSuperclass(clazz, &resultclazz);
  PLUGIN_TEST (ID (superclazz) == ID (resultclazz), "CLASS HIERARCHY");
  (*outSecureEnv)->IsAssignableFrom(clazz, superclazz, &resultbool);
  PLUGIN_TEST (resultbool, "HI");
  (*outSecureEnv)->IsAssignableFrom(superclazz, clazz, &resultbool);
  PLUGIN_TEST (!resultbool, "IsAssignableFrom: JNI_FALSE");

  (*outSecureEnv)->FindClass ("java.awt.Container", &clazz);
  (*outSecureEnv)->GetMethodID (clazz, "<init>", "()V", &method);
  (*outSecureEnv)->NewObject (clazz, method, NULL, &newobject);
  (*outSecureEnv)->IsInstanceOf(newobject, clazz, &resultbool);
  PLUGIN_TEST (resultbool, "IsInstanceOf: JNI_TRUE");
  (*outSecureEnv)->FindClass ("java.lang.String", &superclazz);
  (*outSecureEnv)->IsInstanceOf(newobject, superclazz, &resultbool);
  PLUGIN_TEST (!resultbool, "HI");

  // FIXME: test NewString and NewStringUTF
#endif
  // FIXME: call set accessible: these methods should ignore access
  // permissions (public/protected/package private/private -> public)

  // Test multi-dimentional array passing/return.

  // WRITE EXCEPTION HANDLING.
  // WRITE ARRAY ELEMENT SETTING/GETTING.

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::SpendTime (PRUint32 timeMillis)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::UnwrapJavaWrapper (JNIEnv* jenv, jobject jobj,
                                         PLUGIN_JAVASCRIPT_TYPE* obj)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

void
IcedTeaPluginFactory::DisplayFailureDialog ()
{
  PLUGIN_TRACE_FACTORY ();
  GtkWidget* dialog = NULL;

  dialog = gtk_message_dialog_new (NULL,
                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_CLOSE,
                                   FAILURE_MESSAGE,
                                   appletviewer_executable);
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

}

NS_IMETHODIMP
IcedTeaPluginFactory::Print(const char* msg, const char* encoding)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

#include <nsICookieService.h>
#include <nsIIOService.h>
#include <nsIScriptSecurityManager.h>
#include <nsIURI.h>
#include <nsServiceManagerUtils.h>

NS_IMPL_ISUPPORTS2 (IcedTeaPluginInstance, nsIPluginInstance,
                    nsIJVMPluginInstance)


NS_IMETHODIMP
IcedTeaPluginInstance::Initialize (nsIPluginInstancePeer* aPeer)
{
  PLUGIN_TRACE_INSTANCE ();

  // Ensure that there is a jvm running...
 
  if (jvm_attached == PR_FALSE)
  {
    // using printf on purpose.. this should happen rarely
    PLUGIN_DEBUG_0ARG("WARNING: Looks like the JVM is not up. Attempting to re-initialize...\n");

    // mark attached right away, in case another initialize() call 
    //is made (happens if multiple applets are present on the same page)
    jvm_attached = PR_TRUE;
    factory->InitializeJava();
  }

  // Send applet tag message to appletviewer.
  // FIXME: nsCOMPtr
  char const* documentbase;
  unsigned int i = 0;
  nsresult result = NS_OK;

  nsCOMPtr<nsIPluginTagInfo2> taginfo = do_QueryInterface (aPeer);
  if (!taginfo)
    {
      PLUGIN_ERROR ("Documentbase retrieval failed."
                    "  Browser not Mozilla-based?");
      result = NS_ERROR_FAILURE;
    }
  taginfo->GetDocumentBase (&documentbase);
  if (!documentbase)
    {
      PLUGIN_ERROR ("Documentbase retrieval failed."
                    "  Browser not Mozilla-based?");
      return NS_ERROR_FAILURE;
    }

  char const* appletTag = NULL;
  taginfo->GetTagText (&appletTag);

  nsCString tagMessage (instanceIdentifierPrefix);
  tagMessage += "tag ";
  tagMessage += documentbase;
  tagMessage += " ";
  tagMessage += appletTag;
  tagMessage += "</embed>";

  PLUGIN_DEBUG_1ARG("TAG FROM BROWSER = %s\n", tagMessage.get());

  // encode newline characters in the message
  nsCString encodedAppletTag("");
  for (int i=0; i < tagMessage.Length(); i++)
  {
	  if (tagMessage.get()[i] == '\r')
	  {
		  encodedAppletTag += "&#13;";
		  continue;
	  }

	  if (tagMessage.get()[i] == '\n')
	  {
		  encodedAppletTag += "&#10;";
		  continue;
	  }

	  encodedAppletTag += tagMessage.get()[i];
  }

  factory->SendMessageToAppletViewer (encodedAppletTag);

  // Set back-pointer to peer instance.
  PLUGIN_DEBUG_1ARG ("SETTING PEER!!!: %p\n", aPeer);
  peer = aPeer;
  NS_ADDREF (aPeer);
  PLUGIN_DEBUG_1ARG ("DONE SETTING PEER!!!: %p\n", aPeer);

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetPeer (nsIPluginInstancePeer** aPeer)
{

  PRBool processed;
  nsresult result;
  while (!peer)
    {
      result = factory->current->ProcessNextEvent(PR_TRUE, &processed);
      PLUGIN_CHECK_RETURN ("wait for peer: process next event", result);
    }

  PLUGIN_DEBUG_1ARG ("GETTING PEER!!!: %p\n", peer);
  *aPeer = peer;
  // FIXME: where is this unref'd?
  NS_ADDREF (peer);
  PLUGIN_DEBUG_2ARG ("DONE GETTING PEER!!!: %p, %p\n", peer, *aPeer);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::Start ()
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginInstance::Stop ()
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginInstance::Destroy ()
{
  PLUGIN_TRACE_INSTANCE ();

  if (fatalErrorOccurred == PR_TRUE)
  {
      return NS_OK;
  }

  nsCString destroyMessage (instanceIdentifierPrefix);
  destroyMessage += "destroy";
  factory->SendMessageToAppletViewer (destroyMessage);
  is_active = PR_FALSE;

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::SetWindow (nsPluginWindow* aWindow)
{
  PLUGIN_TRACE_INSTANCE ();

  // Simply return if we receive a NULL window.
  if ((aWindow == NULL) || (aWindow->window == NULL))
    {
      PLUGIN_DEBUG ("Instance::SetWindow: got NULL window.");

      return NS_OK;
    }

  if (window_handle)
    {

       if (initialized == PR_FALSE) 
       {

           PLUGIN_DEBUG_1ARG ("IcedTeaPluginInstance::SetWindow: Instance %p waiting for initialization...\n", this);

           long startTime = get_time_in_s();
           PRBool timedOut = PR_FALSE;

           while (initialized == PR_FALSE && 
                  this->fatalErrorOccurred == PR_FALSE && 
                  this->is_active == PR_FALSE) 
           {
               PROCESS_PENDING_EVENTS;

               if ((get_time_in_s() - startTime) > TIMEOUT)
               {
                   timedOut = PR_TRUE;
                   break;
                }
            }

            // we timed out
            if (timedOut == PR_TRUE)
			{
                PLUGIN_DEBUG_1ARG ("Initialization for instance %d has timed out. Marking it void\n", instance_identifier);
				this->fatalErrorOccurred = PR_TRUE;
                return NS_ERROR_FAILURE;
			}

            // did we bail because there is no jvm?
            if (this->fatalErrorOccurred == PR_TRUE)
			{
				PLUGIN_DEBUG_0ARG("Initialization failed. SetWindow returning\n");
				return NS_ERROR_FAILURE;
			}

            PLUGIN_DEBUG_1ARG ("Instance %p initialization complete...\n", this);
       }

      // The window already exists.
      if (window_handle == aWindow->window)
	{
          // The parent window is the same as in previous calls.
          PLUGIN_DEBUG ("Instance::SetWindow: window already exists.");

		  nsCString message (instanceIdentifierPrefix);
		  PRBool changed = PR_FALSE;

          // The window is the same as it was for the last
          // SetWindow call.
          if (aWindow->width != window_width)
            {
			  // width has changed
              PLUGIN_DEBUG ("Instance::SetWindow: window width changed.");

			  window_width = aWindow->width;
			  changed = PR_TRUE;
            }

          if (aWindow->height != window_height)
            {
			  // width has changed
              PLUGIN_DEBUG ("Instance::SetWindow: window height changed.");

			  window_height = aWindow->height;
			  changed = PR_TRUE;
            }

           if (changed == PR_TRUE)
		   {
			  message += "width ";
			  message.AppendInt (window_width);
              message += " height ";
              message.AppendInt (window_height);
              factory->SendMessageToAppletViewer (message);
		   }

	}
      else
	{
	  // The parent window has changed.  This branch does run but
	  // doing nothing in response seems to be sufficient.
	  PLUGIN_DEBUG ("Instance::SetWindow: parent window changed.");
	}
    }
  else
    {
      PLUGIN_DEBUG ("Instance::SetWindow: setting window.");

      nsCString windowMessage (instanceIdentifierPrefix);
      windowMessage += "handle ";
      windowMessage.AppendInt (reinterpret_cast<PRInt64>
                               (aWindow->window));
      factory->SendMessageToAppletViewer (windowMessage);

      // Store the window handle.
      window_handle = aWindow->window;
    }

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::NewStream (nsIPluginStreamListener** aListener)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginInstance::Print (nsPluginPrint* aPlatformPrint)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetValue (nsPluginInstanceVariable aVariable,
                                 void* aValue)
{
  PLUGIN_TRACE_INSTANCE ();
  nsresult result = NS_OK;

  switch (aVariable)
    {
    case nsPluginInstanceVariable_WindowlessBool:
      *static_cast<PRBool*> (aValue) = PR_FALSE;
      break;
    case nsPluginInstanceVariable_TransparentBool:
      *static_cast<PRBool*> (aValue) = PR_FALSE;
      break;
    case nsPluginInstanceVariable_DoCacheBool:
      *static_cast<PRBool*> (aValue) = PR_FALSE;
      break;
    case nsPluginInstanceVariable_CallSetWindowAfterDestroyBool:
      *static_cast<PRBool*> (aValue) = PR_FALSE;
      break;
    case nsPluginInstanceVariable_NeedsXEmbed:
      *static_cast<PRBool*> (aValue) = PR_TRUE;
      break;
    case nsPluginInstanceVariable_ScriptableInstance:
      // Fall through.
    case nsPluginInstanceVariable_ScriptableIID:
      // Fall through.
    default:
      result = NS_ERROR_INVALID_ARG;
      PLUGIN_ERROR ("Unknown plugin value");
    }

  return result;
}

NS_IMETHODIMP
IcedTeaPluginInstance::HandleEvent (nsPluginEvent* aEvent, PRBool* aHandled)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetJavaObject (jobject* object)
{
  PLUGIN_TRACE_INSTANCE ();

  // wait for instance to initialize

  if (initialized == PR_FALSE) 
    {

      PLUGIN_DEBUG_1ARG ("IcedTeaPluginInstance::GetJavaObject: Instance %p waiting for initialization...\n", this);

      long startTime = get_time_in_s();
      PRBool timedOut = PR_FALSE;
      while (initialized == PR_FALSE && 
             this->fatalErrorOccurred == PR_FALSE && 
             this->is_active == PR_FALSE) 
      {
          PROCESS_PENDING_EVENTS;

          if ((get_time_in_s() - startTime) > TIMEOUT)
          {
              timedOut = PR_TRUE;
              break;
           }
      }

      // we timed out
      if (timedOut == PR_TRUE)
	  {
          PLUGIN_DEBUG_1ARG ("IcedTeaPluginInstance::GetJavaObject: Initialization for instance %d has timed out. Marking it void\n", instance_identifier);
          this->fatalErrorOccurred = PR_TRUE;
          return NS_ERROR_FAILURE;
	  }

      PLUGIN_DEBUG_1ARG ("Instance %p initialization complete...\n", this);
    }
 
  return factory->GetJavaObject (instance_identifier, object);
}

#include <nsIDNSRecord.h>
#include <nsIDNSService.h>
#include <nsIHttpAuthManager.h>
#include <nsIProxyInfo.h>
#include <nsIProtocolProxyService.h>
#include <nsILoginManager.h>
#include <nsILoginInfo.h>

/** 
 *
 * Returns the proxy information for the given url
 *
 * The proxy query part of this function can be made much smaller by using 
 * nsIPluginManager2::FindProxyForURL() .. however, because we need to parse 
 * the return components in various ways, it is easier to query 
 * nsIProtocolProxyService directly
 *
 * @param siteAddr The URL to check
 * @param  proxyScheme Return parameter containing the proxy URI scheme (http/socks/etc.)
 * @param proxyHost Return parameter containing the proxy host
 * @param proxyPort Return parameter containing the proxy port
 */

NS_IMETHODIMP
IcedTeaPluginFactory::GetProxyInfo(const char* siteAddr, char** proxyScheme, char** proxyHost, char** proxyPort)
{
  nsresult rv;

  // Initialize service variables
  nsCOMPtr<nsIProtocolProxyService> proxy_svc = do_GetService(NS_PROTOCOLPROXYSERVICE_CONTRACTID, &rv);

  if (!proxy_svc) {
	  printf("Cannot initialize proxy service\n");
	  return rv;
  }

  nsCOMPtr<nsIIOService> io_svc = do_GetService(NS_IOSERVICE_CONTRACTID, &rv);

  if (NS_FAILED(rv) || !io_svc) {
    printf("Cannot initialize io service\n");
    return NS_ERROR_FAILURE;
  }

  // uri which needs to be accessed
  nsCOMPtr<nsIURI> uri;
  io_svc->NewURI(nsCString(siteAddr), NULL, NULL, getter_AddRefs(uri));

  // find the proxy address if any
  nsCOMPtr<nsIProxyInfo> info;
  proxy_svc->Resolve(uri, 0, getter_AddRefs(info));

  // if there is no proxy found, return immediately
  if (!info) {
     PLUGIN_DEBUG_1ARG("%s does not need a proxy\n", siteAddr);
	 return NS_ERROR_FAILURE;
  }

  // if proxy info is available, extract it
  nsCString phost;
  PRInt32 pport;
  nsCString ptype;

  info->GetHost(phost);
  info->GetPort(&pport);
  info->GetType(ptype);

  // resolve the proxy address to an IP
  nsCOMPtr<nsIDNSService> dns_svc = do_GetService(NS_DNSSERVICE_CONTRACTID, &rv);

  if (!dns_svc) {
      printf("Cannot initialize DNS service\n");
      return rv;
  }

  nsCOMPtr<nsIDNSRecord> record;
  dns_svc->Resolve(phost, 0U, getter_AddRefs(record));

  // TODO: Add support for multiple ips
  nsDependentCString ipAddr;
  record->GetNextAddrAsString(ipAddr);

  // pack information in return variables
  snprintf(*proxyScheme, sizeof(char)*32, "%s", ptype.get());
  snprintf(*proxyHost, sizeof(char)*64, "%s", ipAddr.get());
  snprintf(*proxyPort, sizeof(char)*8, "%d", pport);

  PLUGIN_DEBUG_4ARG("Proxy info for %s: %s %s %s\n", siteAddr, *proxyScheme, *proxyHost, *proxyPort);

  return NS_OK;
}

/** 
 * Returns the cookie information for the given url
 *
 * @param siteAddr The URI to check (must be decoded)
 * @return cookieString The cookie string for the given URI
 */

NS_IMETHODIMP
IcedTeaPluginFactory::GetCookieInfo(const char* siteAddr, char** cookieString) 
{

  nsresult rv;
  nsCOMPtr<nsIScriptSecurityManager> sec_man = 
    do_GetService(NS_SCRIPTSECURITYMANAGER_CONTRACTID, &rv);

  if (!sec_man) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsIIOService> io_svc = do_GetService(NS_IOSERVICE_CONTRACTID, &rv);

  if (NS_FAILED(rv) || !io_svc) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsIURI> uri;
  io_svc->NewURI(nsCString(siteAddr), NULL, NULL, getter_AddRefs(uri));

  nsCOMPtr<nsICookieService> cookie_svc = do_GetService(NS_COOKIESERVICE_CONTRACTID, &rv);

  if (NS_FAILED(rv) || !cookie_svc) {
    return NS_ERROR_FAILURE;
  }

  rv = cookie_svc->GetCookieString(uri, NULL, cookieString);

  if (NS_FAILED(rv) || !*cookieString) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginFactory::GetJavaObject (PRUint32 instance_identifier,
                                     jobject* object)
{
  // add stub to hash table, mapping index to object_stub pointer.  by
  // definition jobject is a pointer, so our jobject representation
  // has to be real pointers.

  // ask Java for index of CODE class
  object_identifier_return = 0;

  int reference = 0;

  nsCString objectMessage ("instance ");
  objectMessage.AppendInt (instance_identifier);
  objectMessage += " reference ";
  objectMessage.AppendInt (reference);
  objectMessage += " GetJavaObject";
  PLUGIN_DEBUG_1ARG ("Sending object message: %s\n", objectMessage.get());
  ResultContainer *container = new ResultContainer();
  result_map.Put(reference, container);
  SendMessageToAppletViewer (objectMessage);

  PRBool processed = PR_FALSE;
  nsresult result = NS_OK;

  // wait for result
  long startTime = get_time_in_s();
  while (object_identifier_return == 0) {
	  current->ProcessNextEvent(PR_TRUE, &processed);

	  // If we have been waiting for more than 20 seconds, something is wrong
	  if ((get_time_in_ms() - startTime) > TIMEOUT)
		  break;
  }

  PLUGIN_DEBUG_1ARG ("GOT JAVA OBJECT IDENTIFIER: %d\n", object_identifier_return);
  if (object_identifier_return == 0)
    PLUGIN_DEBUG_0ARG ("WARNING: received object identifier 0\n");

  *object = references.ReferenceObject (object_identifier_return);

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetText (char const** result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// plugin_in_pipe_callback is called when data is available on the
// input pipe, or when the appletviewer crashes or is killed.  It may
// be called after data has been destroyed in which case it simply
// returns FALSE to remove itself from the glib main loop.
static gboolean
plugin_in_pipe_callback (GIOChannel* source,
                         GIOCondition condition,
                         gpointer plugin_data)
{
  PLUGIN_DEBUG ("plugin_in_pipe_callback");

  gchar* message = NULL;
  gboolean keep_installed = TRUE;

  // If data is NULL then GCJ_Destroy has already been called and
  // plugin_in_pipe_callback is being called after plugin
  // destruction.  In that case all we need to do is return FALSE so
  // that the plugin_in_pipe_callback watch is removed.
  if (condition & G_IO_IN)
  {
      if (g_io_channel_read_line (in_from_appletviewer,
                                  &message, NULL, NULL,
                                  &channel_error)
          != G_IO_STATUS_NORMAL)
       {
           if (channel_error)
           {
               PLUGIN_ERROR_TWO ("Failed to read line from input channel",
                                 channel_error->message);
               g_error_free (channel_error);
               channel_error = NULL;
            }
            else
                PLUGIN_ERROR ("Failed to read line from input channel");
        }
        else
        {

             // Remove trailing newline from message.
             //message[strlen (message) - 1] = '\0';
             PLUGIN_DEBUG_1ARG ("Received message: %s\n", message);
             PLUGIN_DEBUG_1ARG ("  PIPE: plugin read: %s\n", message);
        }

        keep_installed = TRUE;
  }

  if (condition & (G_IO_ERR | G_IO_HUP))
  {
      PLUGIN_DEBUG ("appletviewer has stopped.");
      keep_installed = FALSE;
	  jvm_attached = PR_FALSE;

	  factory->MarkInstancesVoid();
  } else
  {
  

    // push message to queue
    PR_EnterMonitor(jvmMsgQueuePRMonitor);
    jvmMsgQueue.push(nsCString(message));
    PR_ExitMonitor(jvmMsgQueuePRMonitor);

    // poke process thread
    PRThread *prThread;
    processThread->GetPRThread(&prThread);
    PRStatus res = PR_Interrupt(prThread);

  }

  PLUGIN_DEBUG ("plugin_in_pipe_callback return");
  return keep_installed;
}


NS_IMETHODIMP
IcedTeaPluginFactory::OnInputStreamReady (nsIAsyncInputStream* aStream)
{
  PLUGIN_TRACE_INSTANCE ();

  return NS_OK;

  // FIXME: change to NSCString.  Why am I getting symbol lookup errors?
  // /home/fitzsim/sources/mozilla/dist/bin/firefox-bin: symbol lookup error:
  // /usr/lib/jvm/java-1.7.0-icedtea-1.7.0.0/jre/lib/i386/IcedTeaPlugin.so:
  // undefined symbol: _ZNK10nsACString12BeginReadingEv
  char message[10000];
  message[0] = 0;
  char byte = 0;
  PRUint32 readCount = 0;
  int index = 0;

  printf ("ONINPUTSTREAMREADY 1 %p\n", current_thread ());
  // Omit return value checking for speed.
  input->Read (&byte, 1, &readCount);
  if (readCount != 1)
    {
      PLUGIN_ERROR ("failed to read next byte");
      return NS_ERROR_FAILURE;
    }
  while (byte != 0)
    {
      message[index++] = byte;
      // Omit return value checking for speed.
      nsresult result = input->Read (&byte, 1, &readCount);
      if (readCount != 1)
        {
          PLUGIN_ERROR ("failed to read next byte");
          return NS_ERROR_FAILURE;
        }
    }
  message[index] = byte;

  printf ("  PIPE: plugin read: %s\n", message);


  // push message to queue
  printf("Got response. Processing... %s\n", message);
  PR_EnterMonitor(jvmMsgQueuePRMonitor);
  printf("Acquired lock on queue\n");
  jvmMsgQueue.push(nsCString(message));
  printf("Pushed to queue\n");
  PR_ExitMonitor(jvmMsgQueuePRMonitor);

  // poke process thread
  PRThread *prThread;
  processThread->GetPRThread(&prThread);
  printf("Interrupting process thread...\n");
  PRStatus res = PR_Interrupt(prThread);
  printf("Handler event dispatched\n");

  nsresult result = async->AsyncWait (this, 0, 0, current);
  PLUGIN_CHECK_RETURN ("re-add async wait", result);

  return NS_OK;
}

#include <nsServiceManagerUtils.h>
#include <nsINetUtil.h>

void
IcedTeaPluginFactory::HandleMessage (nsCString const& message)
{
  PLUGIN_DEBUG_1ARG ("received message: %s\n", message.get());

  nsresult conversionResult;
  PRUint32 space;
  char msg[message.Length()];
  char *pch;

  strcpy(msg, message.get());
  pch = strtok (msg, " ");
  nsDependentCSubstring prefix(pch, strlen(pch));
  pch = strtok (NULL, " ");
  PRUint32 identifier = nsDependentCSubstring(pch, strlen(pch)).ToInteger (&conversionResult);

  /* Certain prefixes may not have an identifier. if they don't. we have a command here */
  nsDependentCSubstring command;
  if (NS_FAILED(conversionResult)) {
    command.Rebind(pch, strlen(pch));
  }

  PRUint32 reference = -1;

  if (strstr(message.get(), "reference") != NULL) {
	  pch = strtok (NULL, " "); // skip "reference" literal
	  pch = strtok (NULL, " ");
	  reference = nsDependentCSubstring(pch, strlen(pch)).ToInteger (&conversionResult);
  }

  if (command.Length() == 0) {
    pch = strtok (NULL, " ");
    command.Rebind(pch, strlen(pch));
  }

  pch = strtok (NULL, " ");

  nsDependentCSubstring rest("", 0);
  while (pch != NULL) {
	rest += pch;
	pch = strtok (NULL, " ");

	if (pch != NULL)
		rest += " ";
  }

  ResultContainer *resultC;
  if (reference != -1 && result_map.Get(reference, &resultC))
  {
    resultC->stop_timer();
  }

//  printf ("Parse results: prefix: %s, identifier: %d, reference: %d, command: %s, rest: %s\n", (nsCString (prefix)).get(), identifier, reference, (nsCString (command)).get(), (nsCString (rest)).get());

  if (prefix == "instance")
    {
      if (command == "status")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0)
		  {
            instance->peer->ShowStatus (nsCString (rest).get ());
          }
        }
      else if (command == "initialized")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0) {
			PLUGIN_DEBUG_2ARG ("Setting instance.initialized for %p from %d ", instance, instance->initialized);
            instance->initialized = PR_TRUE;
			PLUGIN_DEBUG_1ARG ("to %d...\n", instance->initialized);
		  }
		}
      else if (command == "fatalError")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0) {
			PLUGIN_DEBUG_2ARG ("Setting instance.fatalErrorOccurred for %p from %d ", instance, instance->fatalErrorOccurred);
            instance->fatalErrorOccurred = PR_TRUE;
			PLUGIN_DEBUG_1ARG ("to %d...\n", instance->fatalErrorOccurred);
		  }
		}
      else if (command == "url")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0)
            {
              space = rest.FindChar (' ');
              nsDependentCSubstring escapedUrl = Substring (rest, 0, space);

              nsresult rv;
              nsCOMPtr<nsINetUtil> net_util = do_GetService(NS_NETUTIL_CONTRACTID, &rv);

              if (!net_util)
                printf("Error instantiating NetUtil service.\n");

              nsDependentCSubstring url;
              net_util->UnescapeString(escapedUrl, 0, url);

              nsDependentCSubstring target = Substring (rest, space + 1);
              nsCOMPtr<nsPIPluginInstancePeer> ownerGetter =
                do_QueryInterface (instance->peer);
              nsIPluginInstanceOwner* owner = nsnull;
              ownerGetter->GetOwner (&owner);
			  PLUGIN_DEBUG_2ARG ("Calling GetURL with %s and %s\n", nsCString (url).get (), nsCString (target).get ());
              nsCOMPtr<nsIRunnable> event = new GetURLRunnable (instance->peer,
													 nsCString (url),
													 nsCString (target));
              current->Dispatch(event, nsIEventTarget::DISPATCH_NORMAL);
            }
        }
      else if (command == "GetWindow")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);

		  PLUGIN_DEBUG_1ARG ("GetWindow instance: %d\n", instance);
          if (instance != 0)
            {
              nsCOMPtr<nsIRunnable> event =
                new IcedTeaRunnableMethod<IcedTeaPluginInstance>
                (instance,
                 &IcedTeaPluginInstance::IcedTeaPluginInstance::GetWindow);
              NS_DispatchToMainThread (event);
            }
        }
      else if (command == "GetMember")
        {
          PLUGIN_DEBUG_0ARG ("POSTING GetMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring nameID = Substring (rest, space + 1);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::GetMember);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING GetMember DONE\n");
        }
      else if (command == "SetMember")
        {
          PLUGIN_DEBUG_0ARG ("POSTING SetMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring nameAndValue = Substring (rest, space + 1);
          space = nameAndValue.FindChar (' ');
          nsDependentCSubstring nameID = Substring (nameAndValue, 0, space);
          // FIXME: these member variables need to be keyed on thread id
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);
          nsDependentCSubstring valueID = Substring (nameAndValue, space + 1);
          value_identifier = valueID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse value id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::SetMember);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING SetMember DONE\n");
        }
      else if (command == "GetSlot")
        {
          PLUGIN_DEBUG_0ARG ("POSTING GetSlot\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring indexStr = Substring (rest, space + 1);
          slot_index = indexStr.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::GetSlot);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING GetSlot DONE\n");
        }
      else if (command == "SetSlot")
        {
          PLUGIN_DEBUG_0ARG ("POSTING SetSlot\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring nameAndValue = Substring (rest, space + 1);
          space = nameAndValue.FindChar (' ');
          nsDependentCSubstring indexStr = Substring (nameAndValue, 0, space);
          slot_index = indexStr.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);
          nsDependentCSubstring valueID = Substring (nameAndValue, space + 1);
          value_identifier = valueID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse value id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::SetSlot);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING SetSlot DONE\n");
        }
      else if (command == "Eval")
        {
          PLUGIN_DEBUG_0ARG ("POSTING Eval\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring stringID = Substring (rest, space + 1);
          string_identifier = stringID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse string id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::Eval);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING Eval DONE\n");
        }
      else if (command == "RemoveMember")
        {
          PLUGIN_DEBUG_0ARG ("POSTING RemoveMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring nameID = Substring (rest, space + 1);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::RemoveMember);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING RemoveMember DONE\n");
        }
      else if (command == "Call")
        {
          PLUGIN_DEBUG_0ARG ("POSTING Call\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = string_to_js_id ((nsCString) javascriptID);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);
          nsDependentCSubstring nameAndArgs = Substring (rest, space + 1);
          space = nameAndArgs.FindChar (' ');
          nsDependentCSubstring nameID = Substring (nameAndArgs, 0, space);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse method name id", conversionResult);
          nsDependentCSubstring argsID = Substring (nameAndArgs, space + 1);
          args_identifier = argsID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse args id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::Call);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING Call DONE\n");
        }
      else if (command == "Finalize")
        {
          PLUGIN_DEBUG_0ARG ("POSTING Finalize\n");
          javascript_identifier = string_to_js_id ((nsCString) rest);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::Finalize);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING Finalize DONE\n");
        }
      else if (command == "ToString")
        {
          PLUGIN_DEBUG_0ARG ("POSTING ToString\n");
          javascript_identifier = string_to_js_id ((nsCString) rest);
          PLUGIN_DEBUG_1ARG ("parse javascript id %ld\n", javascript_identifier);

          nsCOMPtr<nsIRunnable> event =
            new IcedTeaRunnableMethod<IcedTeaPluginFactory>
            (this,
             &IcedTeaPluginFactory::IcedTeaPluginFactory::ToString);
          NS_DispatchToMainThread (event);
          PLUGIN_DEBUG_0ARG ("POSTING ToString DONE\n");
        }
      else if (command == "Error")
        {

			ResultContainer *resultC;
			if (reference != -1 && result_map.Get(reference, &resultC))
			{
				PLUGIN_DEBUG_1ARG ("Error occured. Setting error flag for container @ %d to true\n", reference);               

				resultC->errorOccurred = PR_TRUE;
				resultC->errorMessage = (nsCString) rest;
			}

			rest += "ERROR: ";
			IcedTeaPluginInstance* instance = NULL;
			instances.Get (identifier, &instance);
			if (instance != 0)
			{
				instance->peer->ShowStatus (nsCString (rest).get ());
			}
		}
    }
  else if (prefix == "context")
    {
      // FIXME: switch context to identifier.

      //printf ("HandleMessage: XXX%sXXX\n", nsCString (command).get ());
      if (command == "GetJavaObject")
        {
          // FIXME: undefine XPCOM_GLUE_AVOID_NSPR?
          // object_identifier_return = rest.ToInteger (&result);
          // FIXME: replace with returnIdentifier ?
          object_identifier_return = rest.ToInteger (&conversionResult);
          PLUGIN_DEBUG_1ARG ("Patrsed integer: %d\n", object_identifier_return);
          PLUGIN_CHECK ("parse integer", conversionResult);

        }
      else if (command == "FindClass"
               || command == "GetSuperclass"
               || command == "IsAssignableFrom"
               || command == "IsInstanceOf"
               || command == "GetStaticMethodID"
               || command == "GetMethodID"
               || command == "GetStaticFieldID"
               || command == "GetFieldID"
               || command == "GetObjectClass"
               || command == "NewObject"
               || command == "NewString"
               || command == "NewStringUTF"
               || command == "GetObjectArrayElement"
               || command == "NewObjectArray"
               || command == "ExceptionOccurred"
               || command == "NewGlobalRef"
               || command == "NewArray")
        {
		  ResultContainer *resultC;
		  result_map.Get(reference, &resultC);
		  PLUGIN_DEBUG_2ARG("Looking in map for %d and found = %d\n", reference, resultC);
		  PLUGIN_DEBUG_1ARG("Curr val = %p\n", resultC);
		  resultC->returnIdentifier = rest.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse integer", conversionResult);
          PLUGIN_DEBUG_1ARG ("GOT RETURN IDENTIFIER %d\n", resultC->returnIdentifier);

        }
      else if (command == "GetField"
               || command == "GetStaticField"
               || command == "CallStaticMethod"
               || command == "GetArrayLength"
               || command == "GetStringUTFLength"
               || command == "GetStringLength"
               || command == "CallMethod")
        {
//          if (returnValue != "")
//            PLUGIN_ERROR ("Return value already defined.");
          
		   ResultContainer *resultC;
		   result_map.Get(reference, &resultC);
		   resultC->returnValue = rest;
		   resultC->returnValue.SetIsVoid(PR_FALSE);
           PLUGIN_DEBUG_1ARG ("PLUGIN GOT RETURN VALUE: %s\n", resultC->returnValue.get());
        }
      else if (command == "GetStringUTFChars")
        {
//          if (returnValue != "")
//            PLUGIN_ERROR ("Return value already defined.");


          nsCString returnValue("");

          // Read byte stream into return value.
          PRUint32 offset = 0;
          PRUint32 previousOffset = 0;

          offset = rest.FindChar (' ');
          int length = Substring (rest, 0,
                                  offset).ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse integer", conversionResult);

          for (int i = 0; i < length; i++)
            {
              previousOffset = offset + 1;
              offset = rest.FindChar (' ', previousOffset);
              returnValue += static_cast<char>
                (Substring (rest, previousOffset,
                            offset - previousOffset).ToInteger (&conversionResult, 16));
              PLUGIN_CHECK ("parse integer", conversionResult);
            }
		  ResultContainer *resultC;
		  result_map.Get(reference, &resultC);
		  resultC->returnValue = returnValue;
          PLUGIN_DEBUG_1ARG ("PLUGIN GOT RETURN UTF-8 STRING: %s\n", resultC->returnValue.get ());
        }
      else if (command == "GetStringChars")
        {
 //         if (!returnValueUCS.IsEmpty ())
//            PLUGIN_ERROR ("Return value already defined.");

          // Read byte stream into return value.
		  nsString returnValueUCS;
		  returnValueUCS.Truncate();

          PRUint32 offset = 0;
          PRUint32 previousOffset = 0;

          offset = rest.FindChar (' ');

          int length = Substring (rest, 0,
                                  offset).ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse integer", conversionResult);
          for (int i = 0; i < length; i++)
            {
              previousOffset = offset + 1;
              offset = rest.FindChar (' ', previousOffset);
              char low = static_cast<char> (
                                            Substring (rest, previousOffset,
                                                       offset - previousOffset).ToInteger (&conversionResult, 16));
              PLUGIN_CHECK ("parse integer", conversionResult);
              previousOffset = offset + 1;
              offset = rest.FindChar (' ', previousOffset);
              char high = static_cast<char> (
                                             Substring (rest, previousOffset,
                                                        offset - previousOffset).ToInteger (&conversionResult, 16));
              PLUGIN_CHECK ("parse integer", conversionResult);
              // FIXME: swap on big-endian systems.
              returnValueUCS += static_cast<PRUnichar> ((high << 8) | low);
            }
          PLUGIN_DEBUG_1ARG ("PLUGIN GOT RETURN UTF-16 STRING: %d: ",
                  returnValueUCS.Length());
          for (int i = 0; i < returnValueUCS.Length(); i++)
            {
              if ((returnValueUCS[i] >= 'A'
                   && returnValueUCS[i] <= 'Z')
                  || (returnValueUCS[i] >= 'a'
                      && returnValueUCS[i] <= 'z')
                  || (returnValueUCS[i] >= '0'
                      && returnValueUCS[i] <= '9'))
			  {
                PLUGIN_DEBUG_1ARG ("%c", returnValueUCS[i]);
			  }
              else
                PLUGIN_DEBUG_0ARG ("?");
            }
          PLUGIN_DEBUG_0ARG ("\n");
		  ResultContainer *resultC;
		  result_map.Get(reference, &resultC);
		  resultC->returnValueUCS = returnValueUCS;
		  resultC->returnValueUCS.SetIsVoid(PR_FALSE);

        }
      // Do nothing for: SetStaticField, SetField, ExceptionClear,
      // DeleteGlobalRef, DeleteLocalRef
    }
	else if (prefix == "plugin")
    {

        if (command == "PluginProxyInfo") {

          nsresult rv;
          nsCOMPtr<nsINetUtil> net_util = do_GetService(NS_NETUTIL_CONTRACTID, &rv);

          if (!net_util)
            printf("Error instantiating NetUtil service.\n");

          // decode the url
          nsDependentCSubstring url;
          net_util->UnescapeString(rest, 0, url);

          char* proxyScheme = (char*) malloc(sizeof(char)*32);
          char* proxyHost = (char*) malloc(sizeof(char)*64);
          char* proxyPort = (char*) malloc(sizeof(char)*8);

          nsCString proxyInfo("plugin PluginProxyInfo ");

          // get proxy info
          if (GetProxyInfo(((nsCString) url).get(), &proxyScheme, &proxyHost, &proxyPort) == NS_OK)
          {
              proxyInfo += proxyScheme;
              proxyInfo += " ";
              proxyInfo += proxyHost;
              proxyInfo += " ";
              proxyInfo += proxyPort;

              PLUGIN_DEBUG_4ARG("Proxy for %s is %s %s %s\n", ((nsCString) url).get(), proxyScheme, proxyHost, proxyPort);
          } else {
              PLUGIN_DEBUG_1ARG("No suitable proxy found for %s\n", ((nsCString) url).get());
          }

          // send back what we found
          SendMessageToAppletViewer (proxyInfo);

		  // free allocated memory
          delete proxyScheme, proxyHost, proxyPort;

		} else if (command == "PluginCookieInfo") 
        {

          nsresult rv;
          nsCOMPtr<nsINetUtil> net_util = do_GetService(NS_NETUTIL_CONTRACTID, &rv);

          if (!net_util)
            printf("Error instantiating NetUtil service.\n");

          // decode the url
          nsDependentCSubstring url;
          net_util->UnescapeString(rest, 0, url);

          nsCString cookieInfo("plugin PluginCookieInfo ");
          cookieInfo += rest;
          cookieInfo += " ";

          char* cookieString;
          if (GetCookieInfo(((nsCString) url).get(), &cookieString) == NS_OK)
          {
              cookieInfo += cookieString;
              PLUGIN_DEBUG_2ARG("Cookie for %s is %s\n", ((nsCString) url).get(), cookieString);
          } else {
              PLUGIN_DEBUG_1ARG("No cookie found for %s\n", ((nsCString) url).get());
          }

          // send back what we found
          SendMessageToAppletViewer (cookieInfo);

		}
	}
}

void IcedTeaPluginFactory::ProcessMessage ()
{
	while (true) {
		PR_Sleep(1000);

		// If there was an interrupt, clear it
		PR_ClearInterrupt();

		// Was I interrupted for shutting down?
		if (shutting_down == PR_TRUE) {
			break;
		}

		// Nope. Ok, is there work to do?
		if (!jvmMsgQueue.empty())
		    ConsumeMsgFromJVM();

		// All done. Now let's process pending events

		// Were there new events dispatched?
		
		PRBool this_has_pending, curr_has_pending, processed = PR_FALSE;
		PRBool continue_processing = PR_TRUE;

		while (continue_processing == PR_TRUE) {

		  processThread->HasPendingEvents(&this_has_pending);
		  if (this_has_pending == PR_TRUE) {
			  processThread->ProcessNextEvent(PR_TRUE, &processed);
			  PLUGIN_DEBUG_1ARG ("Pending event processed (this) ... %d\n", processed);
		  }

		  current->HasPendingEvents(&curr_has_pending);
		  if (curr_has_pending == PR_TRUE) {
			  current->ProcessNextEvent(PR_TRUE, &processed);
			  PLUGIN_DEBUG_1ARG ("Pending event processed (current) ... %d\n", processed);
		  }

		  if (this_has_pending != PR_TRUE && curr_has_pending != PR_TRUE) {
			  continue_processing = PR_FALSE;
		  }
		}
	}

}

void IcedTeaPluginFactory::ConsumeMsgFromJVM ()
{
	PLUGIN_TRACE_INSTANCE ();

	while (!jvmMsgQueue.empty()) {

    	PR_EnterMonitor(jvmMsgQueuePRMonitor);
		nsCString message = jvmMsgQueue.front();
		jvmMsgQueue.pop();
    	PR_ExitMonitor(jvmMsgQueuePRMonitor);

		HandleMessage (message);
		PLUGIN_DEBUG_0ARG ("Processing complete\n");
	}
}

/**
 *
 * JNI I/O code
 *

#include <jni.h>

typedef jint (JNICALL *CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);

void IcedTeaPluginFactory::InitJVM ()
{

  JavaVMOption options[2];
  JavaVMInitArgs vm_args;
  long result;
  jmethodID mid;
  jfieldID fid;
  jobject jobj;
  int i, asize;

  void *handle = dlopen(libjvm_so, RTLD_NOW);
  if (!handle) {
    printf("Cannot open library: %s\n", dlerror());
  }

  options[0].optionString = ".";
  options[1].optionString = "-Djava.compiler=NONE";
//  options[2].optionString = "-Xdebug";
//  options[3].optionString = "-Xagent";
//  options[4].optionString = "-Xrunjdwp:transport=dt_socket,address=8787,server=y,suspend=n";

  vm_args.version = JNI_VERSION_1_2;
  vm_args.options = options;
  vm_args.nOptions = 2;
  vm_args.ignoreUnrecognized = JNI_TRUE;

  PLUGIN_DEBUG("invoking vm...\n");

  PR_EnterMonitor(jvmPRMonitor);

  CreateJavaVM_t JNI_CreateJavaVM = (CreateJavaVM_t) dlsym(handle, "JNI_CreateJavaVM");
  result = (*JNI_CreateJavaVM)(&jvm,(void **)&javaEnv, &vm_args);
  if(result == JNI_ERR ) {
    printf("Error invoking the JVM");
	exit(1);
    //return NS_ERROR_FAILURE;
  }

  PLUGIN_DEBUG("Looking for the PluginMain constructor...");

  javaPluginClass = (javaEnv)->FindClass("Lsun/applet/PluginMain;");
  if( javaPluginClass == NULL ) {
    printf("can't find class PluginMain\n");
	exit(1);
    //return NS_ERROR_FAILURE;
  }
  (javaEnv)->ExceptionClear();
  mid=(javaEnv)->GetMethodID(javaPluginClass, "<init>", "()V");

  if( mid == NULL ) {
    printf("can't find method init\n");
	exit(1);
    //return NS_ERROR_FAILURE;
  }

  PLUGIN_DEBUG("Creating PluginMain object...");

  javaPluginObj=(javaEnv)->NewObject(javaPluginClass, mid);

  if( javaPluginObj == NULL ) {
    printf("can't create jobj\n");
	exit(1);
    //return NS_ERROR_FAILURE;
  }

  PLUGIN_DEBUG("PluginMain object created...");

  postMessageMID = (javaEnv)->GetStaticMethodID(javaPluginClass, "postMessage", "(Ljava/lang/String;)V");

  if( postMessageMID == NULL ) {
    printf("can't find method postMessage(Ljava/lang/String;)V\n");
	exit(1);
  }

  getMessageMID = (javaEnv)->GetStaticMethodID(javaPluginClass, "getMessage", "()Ljava/lang/String;");

  if( getMessageMID == NULL ) {
    printf("can't find method getMessage()Ljava/lang/String;\n");
	exit(1);
  }

  jvm->DetachCurrentThread();

  printf("VM Invocation complete, detached");

  PR_ExitMonitor(jvmPRMonitor);

  // Start another thread to periodically poll for available messages

  nsCOMPtr<nsIRunnable> readThreadEvent =
							new IcedTeaRunnableMethod<IcedTeaPluginFactory>
							(this, &IcedTeaPluginFactory::IcedTeaPluginFactory::ReadFromJVM);

  NS_NewThread(getter_AddRefs(readThread), readThreadEvent);

  nsCOMPtr<nsIRunnable> processMessageEvent =
							new IcedTeaRunnableMethod<IcedTeaPluginFactory>
							(this, &IcedTeaPluginFactory::IcedTeaPluginFactory::ProcessMessage);

  NS_NewThread(getter_AddRefs(processThread), processMessageEvent);


  //printf("PluginMain initialized...\n");
  //(jvm)->DestroyJavaVM();
  //dlclose(handle);
}

void IcedTeaPluginFactory::ReadFromJVM ()
{

	PLUGIN_TRACE_INSTANCE ();

	int noResponseCycles = 20;

	const char *message;
	int responseSize;
	jstring response;

	while (true) {

		// Lock, attach, read, detach, unlock
		PR_EnterMonitor(jvmPRMonitor);
		(jvm)->AttachCurrentThread((void**)&javaEnv, NULL);

		response = (jstring) (javaEnv)->CallStaticObjectMethod(javaPluginClass, getMessageMID);
		responseSize = (javaEnv)->GetStringLength(response);

		message = responseSize > 0 ? (javaEnv)->GetStringUTFChars(response, NULL) : "";
		(jvm)->DetachCurrentThread();
		PR_ExitMonitor(jvmPRMonitor);

		if (responseSize > 0) {

			noResponseCycles = 0;

			PR_EnterMonitor(jvmMsgQueuePRMonitor);

			printf("Async processing: %s\n", message);
			jvmMsgQueue.push(nsCString(message));

			PR_ExitMonitor(jvmMsgQueuePRMonitor);
	
			// poke process thread
			PRThread *prThread;
			processThread->GetPRThread(&prThread);

			printf("Interrupting process thread...\n");
			PRStatus res = PR_Interrupt(prThread);

			// go back to bed
			PR_Sleep(PR_INTERVAL_NO_WAIT);
		} else {
			//printf("Async processor sleeping...\n");
            if (noResponseCycles >= 5) {
			    PR_Sleep(1000);
			} else {
				PR_Sleep(PR_INTERVAL_NO_WAIT);
			}

            noResponseCycles++;
		}
	}
}

void IcedTeaPluginFactory::IcedTeaPluginFactory::WriteToJVM(nsCString& message)
{

  PLUGIN_TRACE_INSTANCE ();

  PR_EnterMonitor(jvmPRMonitor);

  (jvm)->AttachCurrentThread((void**)&javaEnv, NULL);

  PLUGIN_DEBUG("Sending to VM:");
  PLUGIN_DEBUG(message.get());
  (javaEnv)->CallStaticVoidMethod(javaPluginClass, postMessageMID, (javaEnv)->NewStringUTF(message.get()));
  PLUGIN_DEBUG("... sent!");

  (jvm)->DetachCurrentThread();
  PR_ExitMonitor(jvmPRMonitor);

  return;

  // Try sync read first. Why you ask? Let me tell you why! because attaching
  // and detaching to the jvm is very expensive. In a standard run, 
  // ReadFromJVM(), takes up 96.7% of the time, of which 66.5% is spent 
  // attaching, and 30.7% is spent detaching. 

  int responseSize;
  jstring response;
  int tries = 0;
  int maxTries = 100;
  const char* retMessage;

  responseSize = 1;
  PRBool processed = PR_FALSE;

  while (responseSize > 0 || tries < maxTries) {

      fflush(stdout);
      fflush(stderr);

	  //printf("trying... %d\n", tries);
	  response = (jstring) (javaEnv)->CallStaticObjectMethod(javaPluginClass, getMessageMID);
	  responseSize = (javaEnv)->GetStringLength(response);

	  retMessage = (javaEnv)->GetStringUTFChars(response, NULL);

	  if (responseSize > 0) {

		    printf("Got response. Processing... %s\n", retMessage);
   
			PR_EnterMonitor(jvmMsgQueuePRMonitor);

		    printf("Acquired lock on queue\n");

			jvmMsgQueue.push(nsCString(retMessage));

		    printf("Pushed to queue\n");

			PR_ExitMonitor(jvmMsgQueuePRMonitor);

            processed = PR_TRUE;

			// If we have a response, bump tries up so we are not looping un-necessarily
			tries = maxTries - 2;
	  } else {
        PR_Sleep(2);
	  }
	  tries++;
  }

  printf("Polling complete...\n");

  (jvm)->DetachCurrentThread();

  PR_ExitMonitor(jvmPRMonitor);

  // wake up asynch read thread if needed

  if (processed == PR_TRUE) {
      // poke process thread
      PRThread *prThread;
      processThread->GetPRThread(&prThread);

      printf("Interrupting process thread...\n");
      PRStatus res = PR_Interrupt(prThread);

      printf("Handler event dispatched\n");
  } else {
      PRThread *prThread;
      readThread->GetPRThread(&prThread);

      printf("Interrupting thread...\n");
      PRStatus res = PR_Interrupt(prThread);
      printf("Interrupted! %d\n", res);
  }

}

*/

nsresult
IcedTeaPluginFactory::StartAppletviewer ()
{

  PLUGIN_TRACE_INSTANCE ();
  nsresult result;

  nsCOMPtr<nsIComponentManager> manager;
  result = NS_GetComponentManager (getter_AddRefs (manager));
  PLUGIN_CHECK_RETURN ("get component manager", result);

  nsCOMPtr<nsILocalFile> file;
  result = manager->CreateInstanceByContractID (NS_LOCAL_FILE_CONTRACTID,
                                                nsnull,
                                                NS_GET_IID (nsILocalFile),
                                                getter_AddRefs (file));
  PLUGIN_CHECK_RETURN ("create local file", result);

  result = file->InitWithNativePath (nsCString (appletviewer_executable));
  PLUGIN_CHECK_RETURN ("init with path", result);

  result = manager->CreateInstanceByContractID (NS_PROCESS_CONTRACTID,
                                                nsnull,
                                                NS_GET_IID (nsIProcess),
                                                getter_AddRefs (applet_viewer_process));
  PLUGIN_CHECK_RETURN ("create process", result);

  result = applet_viewer_process->Init (file);
  PLUGIN_CHECK_RETURN ("init process", result);

  // FIXME: hard-coded port number.
  int numArgs;
  char const** args;
  
  if (getenv("ICEDTEAPLUGIN_DEBUG"))
  {
	  numArgs = 4;
      char const* javaArgs[4] = { "-Xdebug", "-Xnoagent", "-Xrunjdwp:transport=dt_socket,address=8787,server=y,suspend=n", "sun.applet.PluginMain" };
	  args = javaArgs;
  } else
  {
	  numArgs = 1;
	  char const* javaArgs[1] = { "sun.applet.PluginMain" };
	  args = javaArgs;
  }

  // start processing thread
  nsCOMPtr<nsIRunnable> processMessageEvent =
							new IcedTeaRunnableMethod<IcedTeaPluginFactory>
							(this, &IcedTeaPluginFactory::IcedTeaPluginFactory::ProcessMessage);

  NS_NewThread(getter_AddRefs(processThread), processMessageEvent);

  // data->in_pipe_name
  in_pipe_name = g_strdup_printf ("%s/icedtea-appletviewer-to-plugin",
                                         data_directory);
  if (!in_pipe_name)
    {
      PLUGIN_ERROR ("Failed to create input pipe name.");
     // If data->in_pipe_name is NULL then the g_free at
      // cleanup_in_pipe_name will simply return.
      
      result = NS_ERROR_OUT_OF_MEMORY;
	  goto cleanup_in_pipe_name;
    }

  // clear the file first
  PLUGIN_DEBUG_TWO ("clearing old input fifo (if any):", in_pipe_name);
  g_remove(in_pipe_name);

  PLUGIN_DEBUG_TWO ("creating input fifo:", in_pipe_name);
  if (mkfifo (in_pipe_name, 0700) == -1 && errno != EEXIST)
    {
      PLUGIN_ERROR_TWO ("Failed to create input pipe", strerror (errno));
      result = NS_ERROR_OUT_OF_MEMORY;
      goto cleanup_in_pipe_name;
    }
  PLUGIN_DEBUG_TWO ("created input fifo:", in_pipe_name);

  // Create plugin-to-appletviewer pipe which we refer to as the
  // output pipe.

  // data->out_pipe_name
  out_pipe_name = g_strdup_printf ("%s/icedtea-plugin-to-appletviewer",
                                         data_directory);

  PLUGIN_DEBUG("got confirmation that appletviewer is running");

  if (!out_pipe_name)
    {
      PLUGIN_ERROR ("Failed to create output pipe name.");
      result = NS_ERROR_OUT_OF_MEMORY;
      goto cleanup_out_pipe_name;
    }

  // clear the file first
  PLUGIN_DEBUG_TWO ("clearing old output fifo (if any):", out_pipe_name);
  
  g_remove(out_pipe_name);
  PLUGIN_DEBUG_TWO ("creating output fifo:", out_pipe_name);
  if (mkfifo (out_pipe_name, 0700) == -1 && errno != EEXIST)
    {
      PLUGIN_ERROR_TWO ("Failed to create output pipe", strerror (errno));
      result = NS_ERROR_OUT_OF_MEMORY;
      goto cleanup_out_pipe_name;
    }
  PLUGIN_DEBUG_TWO ("created output fifo:", out_pipe_name);

#if MOZILLA_VERSION_COLLAPSED < 1090100
  result = applet_viewer_process->Run (PR_FALSE, args, numArgs, nsnull);
#else
  result = applet_viewer_process->Run (PR_FALSE, args, numArgs);
#endif
  PLUGIN_CHECK_RETURN ("run process", result);

  out_to_appletviewer = g_io_channel_new_file (out_pipe_name,
                                                  "w", &channel_error);

  if (!out_to_appletviewer)
    {
      if (channel_error)
        {
          PLUGIN_ERROR_TWO ("Failed to create output channel",
                            channel_error->message);
          g_error_free (channel_error);
          channel_error = NULL;
        }
      else
        PLUGIN_ERROR ("Failed to create output channel");

      result = NS_ERROR_UNEXPECTED;
      goto cleanup_out_to_appletviewer;
    }

  // Create appletviewer-to-plugin channel.  The default encoding for
  // the file is UTF-8.
  // data->in_from_appletviewer
  in_from_appletviewer = g_io_channel_new_file (in_pipe_name,
                                                  "r", &channel_error);
  if (!in_from_appletviewer)
    {
      if (channel_error)
        {
          PLUGIN_ERROR_TWO ("Failed to create input channel",
                            channel_error->message);
          g_error_free (channel_error);
          channel_error = NULL;
        }
      else
        PLUGIN_ERROR ("Failed to create input channel");

      result = NS_ERROR_UNEXPECTED;
      goto cleanup_in_from_appletviewer;
    }

  // Watch for hangup and error signals on the input pipe.
  in_watch_source =
    g_io_add_watch (in_from_appletviewer,
                    (GIOCondition) (G_IO_IN | G_IO_ERR | G_IO_HUP),
                    plugin_in_pipe_callback, NULL);

 goto cleanup_done;

 cleanup_in_watch_source:
  // Removing a source is harmless if it fails since it just means the
  // source has already been removed.
  g_source_remove (in_watch_source);
  in_watch_source = 0;

 cleanup_in_from_appletviewer:
  if (in_from_appletviewer)
    g_io_channel_unref (in_from_appletviewer);
  in_from_appletviewer = NULL;

  // cleanup_out_watch_source:
  g_source_remove (out_watch_source);
  out_watch_source = 0;

 cleanup_out_to_appletviewer:
  if (out_to_appletviewer)
    g_io_channel_unref (out_to_appletviewer);
  out_to_appletviewer = NULL;

  // cleanup_out_pipe:
  // Delete output pipe.
  PLUGIN_DEBUG_TWO ("deleting input fifo:", in_pipe_name);
  unlink (out_pipe_name);
  PLUGIN_DEBUG_TWO ("deleted input fifo:", in_pipe_name);

 cleanup_out_pipe_name:
  g_free (out_pipe_name);
  out_pipe_name = NULL;

  // cleanup_in_pipe:
  // Delete input pipe.
  PLUGIN_DEBUG_TWO ("deleting output fifo:", out_pipe_name);
  unlink (in_pipe_name);
  PLUGIN_DEBUG_TWO ("deleted output fifo:", out_pipe_name);

 cleanup_in_pipe_name:
  g_free (in_pipe_name);
  in_pipe_name = NULL;

 cleanup_done:

 return result;
}

nsresult
IcedTeaPluginFactory::SendMessageToAppletViewer (nsCString& message)
{
  PLUGIN_TRACE_INSTANCE ();

  nsresult result;
  PRBool processed;

  PLUGIN_DEBUG_1ARG ("Writing to JVM: %s\n", message.get());

  gsize bytes_written = 0;

  message.Append('\n');

  // g_io_channel_write_chars will return something other than
  // G_IO_STATUS_NORMAL if not all the data is written.  In that
  // case we fail rather than retrying.
  if (g_io_channel_write_chars (out_to_appletviewer,
                                message.get(), -1, &bytes_written,
                                &channel_error)
      != G_IO_STATUS_NORMAL)
  {
        if (channel_error)
        {
            PLUGIN_ERROR_TWO ("Failed to write bytes to output channel",
                                channel_error->message);
            g_error_free (channel_error);
            channel_error = NULL;
         }
          else
            PLUGIN_ERROR ("Failed to write bytes to output channel");
  }

  if (g_io_channel_flush (out_to_appletviewer, &channel_error)
      != G_IO_STATUS_NORMAL)
  {
      if (channel_error)
      {
          PLUGIN_ERROR_TWO ("Failed to flush bytes to output channel",
                            channel_error->message);
          g_error_free (channel_error);
          channel_error = NULL;
      }
      else
          PLUGIN_ERROR ("Failed to flush bytes to output channel");
  }

  PLUGIN_DEBUG_1ARG ("Wrote %d bytes to pipe\n", bytes_written);

  return NS_OK;
}

PRUint32
IcedTeaPluginFactory::RegisterInstance (IcedTeaPluginInstance* instance)
{
  // FIXME: do locking?
  PRUint32 identifier = next_instance_identifier;
  next_instance_identifier++;
  instances.Put (identifier, instance);
  return identifier;
}

void
IcedTeaPluginFactory::UnregisterInstance (PRUint32 instance_identifier)
{
  // FIXME: do locking?
  instances.Remove (instance_identifier);
}

IcedTeaPluginInstance::IcedTeaPluginInstance (IcedTeaPluginFactory* factory)
: window_handle (0),
  window_width (0),
  window_height (0),
  peer(0),
  liveconnect_window (0),
  initialized(PR_FALSE),
  fatalErrorOccurred(PR_FALSE),
  is_active(PR_TRUE),
  instanceIdentifierPrefix ("")
{
  PLUGIN_TRACE_INSTANCE ();
  IcedTeaPluginInstance::factory = factory;
  instance_identifier = factory->RegisterInstance (this);

  instanceIdentifierPrefix += "instance ";
  instanceIdentifierPrefix.AppendInt (instance_identifier);
  instanceIdentifierPrefix += " ";
}

#include <nsIScriptSecurityManager.h>

void
IcedTeaPluginInstance::GetWindow ()
{

  nsresult result;
  PLUGIN_DEBUG_1ARG ("HERE 22: %d\n", liveconnect_window);

  // principalsArray, numPrincipals and securitySupports
  // are ignored by GetWindow.  See:
  //
  // nsCLiveconnect.cpp: nsCLiveconnect::GetWindow
  // jsj_JSObject.c: jsj_enter_js
  // lcglue.cpp: enter_js_from_java_impl
  // so they can all safely be null.
  if (factory->proxyEnv != NULL)
    {
      PLUGIN_DEBUG_2ARG ("HERE 23: %d, %p\n", liveconnect_window, current_thread ());

      // there is a bad race condition here where if the instance is active, 
      // this code remains active after destruction.. so double check
	  if (is_active != PR_TRUE)
	  {
		  PLUGIN_DEBUG_1ARG("Plugin %d is no longer active. Bypassing \
                             GetWindow request.\n", instance_identifier);
		  return;
	  }

      result = factory->liveconnect->GetWindow(factory->proxyEnv,
                                               this,
                                               NULL, 0, NULL,
                                               &liveconnect_window);
      PLUGIN_CHECK ("get window", result);
      PLUGIN_DEBUG_1ARG ("HERE 24: %ld\n", liveconnect_window);
    }

  PLUGIN_DEBUG_1ARG ("HERE 20: %ld\n", liveconnect_window);

  char *windowAddr;
  windowAddr = (char*) malloc(20*sizeof(char));
  js_id_to_string(&windowAddr, liveconnect_window);

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptGetWindow";
  message += " ";
  message += windowAddr;
  factory->SendMessageToAppletViewer (message);

  free(windowAddr);
}

IcedTeaPluginInstance::~IcedTeaPluginInstance ()
{
  PLUGIN_TRACE_INSTANCE ();
  factory->UnregisterInstance (instance_identifier);
}

// FIXME: these LiveConnect member functions need to be in instance
// since nsCLiveConnect objects associate themselves with an instance,
// for JavaScript context switching.
void
IcedTeaPluginFactory::GetMember ()
{
  nsresult result;
  PLUGIN_DEBUG_0ARG ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  PLUGIN_DEBUG_0ARG ("AFTER GETTING NAMESTRING\n");

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        PLUGIN_DEBUG_2ARG ("Calling GETMEMBER: %d, %d\n", javascript_identifier, strSize);
        result = liveconnect->GetMember(proxyEnv,
                                        javascript_identifier,
                                        nameString, strSize,
                                        NULL, 0, NULL,
                                        &liveconnect_member);
        PLUGIN_CHECK ("get member", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. GetMember call skipped\n", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  PLUGIN_DEBUG_1ARG ("GOT MEMBER: %d\n", ID (liveconnect_member));
  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptGetMember";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::GetSlot ()
{
  nsresult result;
  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        result = liveconnect->GetSlot(proxyEnv,
                                      javascript_identifier,
                                      slot_index,
                                      NULL, 0, NULL,
                                      &liveconnect_member);
        PLUGIN_CHECK ("get slot", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. GetSlot call skipped\n", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  PLUGIN_DEBUG_1ARG ("GOT SLOT: %d\n", ID (liveconnect_member));
  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptGetSlot";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::SetMember ()
{
  nsresult result;
  PLUGIN_DEBUG_0ARG ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  PLUGIN_DEBUG_0ARG ("AFTER GETTING NAMESTRING\n");

  jobject value = references.ReferenceObject (value_identifier);
  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        PLUGIN_DEBUG_2ARG ("Calling SETMEMBER: %d, %d\n", javascript_identifier, strSize);
        result = liveconnect->SetMember(proxyEnv,
                                        javascript_identifier,
                                        nameString, strSize,
                                        value,
                                        NULL, 0, NULL);
        PLUGIN_CHECK ("set member", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. SetMember call skipped\n", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptSetMember";
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::SetSlot ()
{
  nsresult result;
  jobject value = references.ReferenceObject (value_identifier);
  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        result = liveconnect->SetSlot(proxyEnv,
                                      javascript_identifier,
                                      slot_index,
                                      value,
                                      NULL, 0, NULL);
        PLUGIN_CHECK ("set slot", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. SetSlot call skipped\n", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptSetSlot";
  SendMessageToAppletViewer (message);
}

#include <nsIJSContextStack.h>

void
IcedTeaPluginFactory::Eval ()
{
  nsresult result;
  PLUGIN_DEBUG_0ARG ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  // FIXME: unreference after SendMessageToAppletViewer call.
  jstring name = static_cast<jstring> (references.ReferenceObject (string_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
	    nsCString evStr("");
		for (int i=0; i < strSize; i++)
			evStr += nameString[i];

        PLUGIN_DEBUG_2ARG ("Calling Eval: %d, %s\n", javascript_identifier, evStr.get());
        result = liveconnect->Eval(proxyEnv,
                                   javascript_identifier,
                                   nameString, strSize,
                                   NULL, 0, NULL,
                                   &liveconnect_member);
        PLUGIN_CHECK ("eval", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. Eval call skipped\n", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptEval";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::RemoveMember ()
{
  nsresult result;
  PLUGIN_DEBUG_0ARG ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  PLUGIN_DEBUG_0ARG ("AFTER GETTING NAMESTRING\n");

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        PLUGIN_DEBUG_2ARG ("Calling RemoveMember: %d, %d\n", javascript_identifier, strSize);
        result = liveconnect->RemoveMember(proxyEnv,
                                           javascript_identifier,
                                           nameString, strSize,
                                           NULL, 0, NULL);
        PLUGIN_CHECK ("RemoveMember", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. Eval call skipped", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptRemoveMember";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::Call ()
{
  nsresult result;
  PLUGIN_DEBUG_0ARG ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (
    references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  PLUGIN_DEBUG_0ARG ("AFTER GETTING NAMESTRING\n");
  jobjectArray args = static_cast<jobjectArray> (
    references.ReferenceObject (args_identifier));

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        PLUGIN_DEBUG_2ARG ("CALL: %d, %d\n", javascript_identifier, strSize);
        result = liveconnect->Call(proxyEnv,
                                   javascript_identifier,
                                   nameString, strSize,
                                   args,
                                   NULL, 0, NULL,
                                   &liveconnect_member);
        PLUGIN_CHECK ("call", result);
	  } else
	  {
		  PLUGIN_DEBUG_1ARG("%d has been cleared. Call skipped", javascript_identifier);
		  liveconnect_member = NULL;
	  }
    }

  PLUGIN_DEBUG_1ARG ("GOT RETURN FROM CALL : %d\n", ID (liveconnect_member));
  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptCall";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::Finalize ()
{
  nsresult result;
  if (proxyEnv != NULL)
    {
      PLUGIN_DEBUG_1ARG ("FINALIZE: %d\n", javascript_identifier);

      if (!factory->js_cleared_handles.Get(javascript_identifier, NULL))
	  {
        // remove reference -- set to PR_FALSE rather than removing from table, 
        // because that allows us to guarantee to all functions using the table, 
	    // that the entry exists
        factory->js_cleared_handles.Put(javascript_identifier, PR_TRUE);
        result = liveconnect->FinalizeJSObject(proxyEnv,
                                             javascript_identifier);
        PLUGIN_CHECK ("finalize", result);
	  } else
	  {
	    PLUGIN_DEBUG_1ARG("%d has no references. Finalization skipped.\n", javascript_identifier);
	  }
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptFinalize";
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::ToString ()
{
  nsresult result;

  jstring liveconnect_member;
  if (proxyEnv != NULL)
    {
      PLUGIN_DEBUG_1ARG ("Calling ToString: %d\n", javascript_identifier);
      result = liveconnect->ToString(proxyEnv,
                                     javascript_identifier,
                                     &liveconnect_member);
      PLUGIN_CHECK ("ToString", result);
    }

  PLUGIN_DEBUG_1ARG ("ToString: %d\n", ID (liveconnect_member));
  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptToString";
  message += " ";
  message.AppendInt (ID (liveconnect_member));
  SendMessageToAppletViewer (message);
}

nsresult
IcedTeaPluginFactory::SetTransport (nsISocketTransport* transport)
{
  PLUGIN_TRACE_INSTANCE ();
  IcedTeaPluginFactory::transport = transport;
//   sink = new IcedTeaEventSink ();
//   nsresult result = transport->SetEventSink (sink, nsnull);
//   PLUGIN_CHECK ("socket event sink", result);
//   return result;
  // FIXME: remove return if EventSink not needed.
  return NS_OK;
}

void
IcedTeaPluginFactory::Connected ()
{
  PLUGIN_TRACE_INSTANCE ();
  connected = PR_TRUE;
}

void
IcedTeaPluginFactory::Disconnected ()
{
  PLUGIN_TRACE_INSTANCE ();
  connected = PR_FALSE;
}

PRBool
IcedTeaPluginFactory::IsConnected ()
{
//  PLUGIN_TRACE_INSTANCE ();
  return connected;
}

NS_IMPL_ISUPPORTS1 (IcedTeaSocketListener, nsIServerSocketListener)

IcedTeaSocketListener::IcedTeaSocketListener (IcedTeaPluginFactory* factory)
{
  PLUGIN_TRACE_LISTENER ();

  IcedTeaSocketListener::factory = factory;
}

IcedTeaSocketListener::~IcedTeaSocketListener ()
{
  PLUGIN_TRACE_LISTENER ();
}

NS_IMETHODIMP
IcedTeaSocketListener::OnSocketAccepted (nsIServerSocket* aServ,
                                         nsISocketTransport* aTransport)
{
  PLUGIN_TRACE_LISTENER ();

  nsresult result = factory->SetTransport (aTransport);
  PLUGIN_CHECK_RETURN ("set transport", result);
  factory->Connected ();

  result = aTransport->OpenOutputStream (nsITransport::OPEN_BLOCKING,
                                        nsnull, nsnull,
                                        getter_AddRefs (factory->output));
  PLUGIN_CHECK_RETURN ("output stream", result);

  result = aTransport->OpenInputStream (0, nsnull, nsnull,
                                       getter_AddRefs (factory->input));
  PLUGIN_CHECK_RETURN ("input stream", result);

  factory->async = do_QueryInterface (factory->input, &result);
  PLUGIN_CHECK_RETURN ("async input stream", result);

  result = factory->async->AsyncWait (factory, 0, 0, factory->current);
  PLUGIN_CHECK_RETURN ("add async wait", result);


  return NS_OK;
}

// FIXME: handle appletviewer crash and shutdown scenarios.
NS_IMETHODIMP
IcedTeaSocketListener::OnStopListening (nsIServerSocket *aServ,
                                        nsresult aStatus)
{
  PLUGIN_TRACE_LISTENER ();

  nsCString shutdownStr("shutdown");
  PLUGIN_DEBUG_1ARG("stop listening: %uld\n", aStatus);

  nsresult result = NS_OK;

  switch (aStatus)
  {
  case NS_ERROR_ABORT:
    factory->SendMessageToAppletViewer(shutdownStr);
    PLUGIN_DEBUG ("appletviewer stopped");
    // FIXME: privatize?
    result = factory->async->AsyncWait (nsnull, 0, 0, factory->current);
    PLUGIN_CHECK_RETURN ("clear async wait", result);
    break;
  default:
    PLUGIN_DEBUG_1ARG ("ERROR %x\n", aStatus);
    PLUGIN_DEBUG ("Listener: Unknown status value.");
  }
  return NS_OK;
}

NS_IMPL_ISUPPORTS1 (IcedTeaEventSink, nsITransportEventSink)

IcedTeaEventSink::IcedTeaEventSink ()
{
  PLUGIN_TRACE_EVENTSINK ();
}

IcedTeaEventSink::~IcedTeaEventSink ()
{
  PLUGIN_TRACE_EVENTSINK ();
}

//static int connected = 0;

NS_IMETHODIMP
IcedTeaEventSink::OnTransportStatus (nsITransport *aTransport,
                                     nsresult aStatus,
                                     PRUint64 aProgress,
                                     PRUint64 aProgressMax)
{
  PLUGIN_TRACE_EVENTSINK ();

  switch (aStatus)
    {
      case nsISocketTransport::STATUS_RESOLVING:
        PLUGIN_DEBUG ("RESOLVING");
        break;
      case nsISocketTransport::STATUS_CONNECTING_TO:
        PLUGIN_DEBUG ("CONNECTING_TO");
        break;
      case nsISocketTransport::STATUS_CONNECTED_TO:
        PLUGIN_DEBUG ("CONNECTED_TO");
        //        connected = 1;
        break;
      case nsISocketTransport::STATUS_SENDING_TO:
        PLUGIN_DEBUG ("SENDING_TO");
        break;
      case nsISocketTransport::STATUS_WAITING_FOR:
        PLUGIN_DEBUG ("WAITING_FOR");
        break;
      case nsISocketTransport::STATUS_RECEIVING_FROM:
        PLUGIN_DEBUG ("RECEIVING_FROM");
        break;
    default:
      PLUGIN_ERROR ("Unknown transport status.");
    }

  return NS_OK;
}

NS_IMPL_ISUPPORTS1 (IcedTeaJNIEnv, nsISecureEnv)

#include <nsCOMPtr.h>
#include <nsIOutputStream.h>
#include <nsISocketTransportService.h>
#include <nsISocketTransport.h>
#include <nsITransport.h>
#include <nsNetCID.h>
#include <nsIPrincipal.h>
#include <xpcjsid.h>

IcedTeaJNIEnv::IcedTeaJNIEnv (IcedTeaPluginFactory* factory)
: factory (factory)
{
  PLUGIN_TRACE_JNIENV ();
  contextCounter = 1;

  contextCounterPRMonitor = PR_NewMonitor();
}

IcedTeaJNIEnv::~IcedTeaJNIEnv ()
{
  PLUGIN_TRACE_JNIENV ();
}

int
IcedTeaJNIEnv::IncrementContextCounter ()
{

	PLUGIN_TRACE_JNIENV ();

    PR_EnterMonitor(contextCounterPRMonitor);
    contextCounter++;
    PR_ExitMonitor(contextCounterPRMonitor);

	return contextCounter;
}

void
IcedTeaJNIEnv::DecrementContextCounter ()
{
	PLUGIN_TRACE_JNIENV ();

    PR_EnterMonitor(contextCounterPRMonitor);
    contextCounter--;
    PR_ExitMonitor(contextCounterPRMonitor);
}

#include "nsCRT.h"

nsresult
IcedTeaJNIEnv::GetEnabledPrivileges(nsCString *privileges, nsISecurityContext *ctx)
{
	// check privileges one by one

	privileges->Truncate();

	// see: http://docs.sun.com/source/816-6170-10/index.htm
	
    if (ctx)
    {

       PRBool hasUniversalBrowserRead = PR_FALSE;
       PRBool hasUniversalJavaPermission = PR_FALSE;

       ctx->Implies("UniversalBrowserRead", "UniversalBrowserRead", &hasUniversalBrowserRead);
       if (hasUniversalBrowserRead == PR_TRUE)
       {
	       *privileges += "UniversalBrowserRead";
       }

       ctx->Implies("UniversalJavaPermission", "UniversalJavaPermission", &hasUniversalJavaPermission);
       if (hasUniversalJavaPermission == PR_TRUE)
       {
	       *privileges += ",";
  	       *privileges += "UniversalJavaPermission";
       }
    }

     return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewObject (jclass clazz,
                          jmethodID methodID,
                          jvalue* args,
                          jobject* result,
                          nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jobject, result); 
  DecrementContextCounter ();

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::CallMethod (jni_type type,
                           jobject obj,
                           jmethodID methodID,
                           jvalue* args,
                           jvalue* result,
                           nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (reference, type, result);
  DecrementContextCounter ();

  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::CallNonvirtualMethod (jni_type type,
                                     jobject obj,
                                     jclass clazz,
                                     jmethodID methodID,
                                     jvalue* args,
                                     jvalue* result,
                                     nsISecurityContext* ctx)
{

  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// nsPrintfCString is not exported.
class NS_COM IcedTeaPrintfCString : public nsCString
  {
    typedef nsCString string_type;

    enum { kLocalBufferSize = 15 };

    public:
      explicit IcedTeaPrintfCString( const char_type* format, ... );

    private:
      char_type  mLocalBuffer[ kLocalBufferSize + 1 ];
  };

IcedTeaPrintfCString::IcedTeaPrintfCString( const char_type* format, ... )
  : string_type(mLocalBuffer, 0, NS_STRING_CONTAINER_INIT_DEPEND)
  {
    va_list ap;

    size_type logical_capacity = kLocalBufferSize;
    size_type physical_capacity = logical_capacity + 1;

    va_start(ap, format);
    int len = PR_vsnprintf(mLocalBuffer, physical_capacity, format, ap);
    SetLength (len);
    NS_CStringSetData(*this, mLocalBuffer, len);
    va_end(ap);
  }

char*
IcedTeaJNIEnv::ValueString (jni_type type, jvalue value)
{
  PLUGIN_TRACE_JNIENV ();
  nsCString retstr ("");

  char* longVal = (char*) malloc(sizeof(char)*20);
  switch (type)
    {
    case jboolean_type:
      retstr += value.z ? "true" : "false";
      break;
    case jbyte_type:
      retstr.AppendInt (value.b & 0x0ff, 16);
      break;
    case jchar_type:
      retstr += value.c;
      break;
    case jshort_type:
      retstr.AppendInt (value.s);
      break;
    case jint_type:
      retstr.AppendInt (value.i);
      break;
    case jlong_type:
      sprintf(longVal, "%"PRId64, value.j);
      retstr += longVal;
      break;
    case jfloat_type:
      retstr += IcedTeaPrintfCString ("%f", value.f);
      break;
    case jdouble_type:
      retstr += IcedTeaPrintfCString ("%g", value.d);
      break;
    case jobject_type:
      retstr.AppendInt (ID (value.l));
      break;
    case jvoid_type:
      break;
    default:
      break;
    }

  free(longVal);

  // Freed by calling function.
  return strdup (retstr.get ());
}

jvalue
IcedTeaJNIEnv::ParseValue (jni_type type, nsCString& str)
{
  PLUGIN_TRACE_JNIENV ();
  jvalue retval;
  PRUint32 id;
  char** bytes;
  int low;
  int high;
  int offset;
  nsresult conversionResult;

  switch (type)
    {
    case jboolean_type:
      retval.z = (jboolean) (str == "true");
      break;
    case jbyte_type:
      retval.b = (jbyte) str.ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse int", conversionResult);
      break;
    case jchar_type:
      offset = str.FindChar ('_', 0);
      low = Substring (str, 0, offset).ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse integer", conversionResult);
      high = Substring (str, offset + 1).ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse integer", conversionResult);
      retval.c = ((high << 8) & 0x0ff00) | (low & 0x0ff);
      break;
    case jshort_type:
      // Assume number is in range.
      retval.s = str.ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse int", conversionResult);
      break;
    case jint_type:
      retval.i = str.ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse int", conversionResult);
      break;
    case jlong_type:
      retval.j =  str.ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse int", conversionResult);
      break;
    case jfloat_type:
      retval.f = strtof (str.get (), NULL);
      break;
    case jdouble_type:
      retval.d = strtold (str.get (), NULL);
      break;
    case jobject_type:
      // Have we referred to this object before?
      id = str.ToInteger (&conversionResult);
      PLUGIN_CHECK ("parse int", conversionResult);
      retval.l = factory->references.ReferenceObject (id);
      break;
    case jvoid_type:
      // Clear.
      retval.i = 0;
      break;
    default:
      PLUGIN_DEBUG_0ARG ("WARNING: didn't handle parse type\n");
      break;
    }

  return retval;
}

// FIXME: make ExpandArgs extend nsACString
char*
IcedTeaJNIEnv::ExpandArgs (JNIID* id, jvalue* args)
{
  PLUGIN_TRACE_JNIENV ();
  nsCString retstr ("");

  int i = 0;
  char stopchar = '\0';
  // FIXME: check for end-of-string throughout.
  if (id->signature[0] == '(')
    {
      i = 1;
      stopchar = ')';
    }
 
  // Method.
  int arg = 0;
  char* fl;
  char* longVal = (char*) malloc(sizeof(char)*20);
  while (id->signature[i] != stopchar)
    {
      switch (id->signature[i])
        {
        case 'Z':
          retstr += args[arg].z ? "true" : "false";
          break;
        case 'B':
          retstr.AppendInt (args[arg].b);
          break;
        case 'C':
          retstr.AppendInt (static_cast<int> (args[arg].c) & 0x0ff);
          retstr += "_";
          retstr.AppendInt ((static_cast<int> (args[arg].c)
                             >> 8) & 0x0ff);
          break;
        case 'S':
          retstr.AppendInt (args[arg].s);
          break;
        case 'I':
          retstr.AppendInt (args[arg].i);
          break;
        case 'J':
          sprintf(longVal, "%"PRId64, args[arg].j);
          retstr += longVal;
          break;
        case 'F':
          retstr += IcedTeaPrintfCString ("%f", args[arg].f);
          break;
        case 'D':
          retstr += IcedTeaPrintfCString ("%g", args[arg].d);
          break;
        case 'L':
          retstr.AppendInt (ID (args[arg].l));
          i++;
          while (id->signature[i] != ';')
            i++;
          break;
        case '[':
          retstr.AppendInt (ID (args[arg].l));
          i++;
          while (id->signature[i] == '[')
            i++;
          if (id->signature[i] == 'L')
            {
              while (id->signature[i] != ';')
                i++;
            }
          else
            {
              if (!(id->signature[i] == 'Z'
                    || id->signature[i] == 'B'
                    || id->signature[i] == 'C'
                    || id->signature[i] == 'S'
                    || id->signature[i] == 'I'
                    || id->signature[i] == 'J'
                    || id->signature[i] == 'F'
                    || id->signature[i] == 'D'))
                PLUGIN_ERROR_TWO ("Failed to parse signature", id->signature);
            }
          break;
        default:
          PLUGIN_ERROR_TWO ("Failed to parse signature", id->signature);
          PLUGIN_DEBUG_1ARG ("FAILED ID: %d\n", id->identifier);
          break;
        }
	
	  retstr += " ";
      i++;
	  arg++;
    }

  free(longVal);

  // Freed by calling function.
  return strdup (retstr.get ());
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetField (jni_type type,
                         jobject obj,
                         jfieldID fieldID,
                         jvalue* result,
                         nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (reference, type, result);
  DecrementContextCounter ();

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::SetField (jni_type type,
                         jobject obj,
                         jfieldID fieldID,
                         jvalue val,
                         nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_TYPE (type);
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_ADD_VALUE (fieldID, val);
  MESSAGE_SEND ();

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::CallStaticMethod (jni_type type,
                                 jclass clazz,
                                 jmethodID methodID,
                                 jvalue* args,
                                 jvalue* result,
                                 nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (reference, type, result);
  DecrementContextCounter ();

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStaticField (jni_type type,
                               jclass clazz,
                               jfieldID fieldID,
                               jvalue* result,
                               nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (reference, type, result);
  DecrementContextCounter ();

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::SetStaticField (jni_type type,
                               jclass clazz,
                               jfieldID fieldID,
                               jvalue val,
                               nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();

  char origin[1024];
  sprintf(origin, "");

  if (ctx)
	  ctx->GetOrigin(origin, 1024);

  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_SRC(origin);
  MESSAGE_ADD_PRIVILEGES(ctx);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_TYPE (type);
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_ADD_VALUE (fieldID, val);
  MESSAGE_SEND ();

  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::GetVersion (jint* version)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::DefineClass (char const* name,
                            jobject loader,
                            jbyte const* buf,
                            jsize len,
                            jclass* clazz)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaJNIEnv::FindClass (char const* name,
                          jclass* clazz)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_STRING (name);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jclass, clazz);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetSuperclass (jclass sub,
                              jclass* super)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (sub);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jclass, super);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::IsAssignableFrom (jclass sub,
                                 jclass super,
                                 jboolean* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (sub);
  MESSAGE_ADD_REFERENCE (super);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_BOOLEAN (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

// IMPLEMENTME.
NS_IMETHODIMP
IcedTeaJNIEnv::Throw (jthrowable obj,
                      jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::ThrowNew (jclass clazz,
                         char const* msg,
                         jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaJNIEnv::ExceptionOccurred (jthrowable* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_SEND ();
  // FIXME: potential leak here: when is result free'd?
  MESSAGE_RECEIVE_REFERENCE (reference, jthrowable, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::ExceptionDescribe (void)
{
  PLUGIN_TRACE_JNIENV ();
  // Do nothing.
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::ExceptionClear (void)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_FUNC();
  MESSAGE_SEND ();
  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::FatalError (char const* msg)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewGlobalRef (jobject lobj,
                             jobject* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (lobj);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE(reference, jobject, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::DeleteGlobalRef (jobject gref)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (gref);
  MESSAGE_SEND ();
  factory->references.UnreferenceObject (ID (gref));
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::DeleteLocalRef (jobject obj)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_SEND ();
//  factory->references.UnreferenceObject (ID (obj));
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::IsSameObject (jobject obj1,
                             jobject obj2,
                             jboolean* result)
{
  PLUGIN_TRACE_JNIENV ();
  *result = (obj1 == NULL && obj2 == NULL) ||
    ((obj1 != NULL && obj2 != NULL)
     && (ID (obj1) == ID (obj2)));
  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::AllocObject (jclass clazz,
                            jobject* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetObjectClass (jobject obj,
                               jclass* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jclass, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::IsInstanceOf (jobject obj,
                             jclass clazz,
                             jboolean* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_BOOLEAN (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetMethodID (jclass clazz,
                            char const* name,
                            char const* sig,
                            jmethodID* id)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (reference, jmethodID, id, sig);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetFieldID (jclass clazz,
                           char const* name,
                           char const* sig,
                           jfieldID* id)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (reference, jfieldID, id, sig);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStaticMethodID (jclass clazz,
                                  char const* name,
                                  char const* sig,
                                  jmethodID* id)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (reference, jmethodID, id, sig);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStaticFieldID (jclass clazz,
                                 char const* name,
                                 char const* sig,
                                 jfieldID* id)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (reference, jfieldID, id, sig);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewString (jchar const* unicode,
                          jsize len,
                          jstring* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_SIZE (len);
  MESSAGE_ADD_STRING_UCS (unicode, len);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jstring, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringLength (jstring str,
                                jsize* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringChars (jstring str,
                               jboolean* isCopy,
                               jchar const** result)
{
  PLUGIN_TRACE_JNIENV ();
  if (isCopy)
    *isCopy = JNI_TRUE;

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_STRING_UCS (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::ReleaseStringChars (jstring str,
                                   jchar const* chars)
{
  PLUGIN_TRACE_JNIENV ();
  PR_Free ((void*) chars);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewStringUTF (char const* utf,
                             jstring* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_STRING_UTF (utf);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jstring, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringUTFLength (jstring str,
                                   jsize* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringUTFChars (jstring str,
                                  jboolean* isCopy,
                                  char const** result)
{
  PLUGIN_TRACE_JNIENV ();
  if (isCopy)
    *isCopy = JNI_TRUE;

  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_STRING (reference, char, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::ReleaseStringUTFChars (jstring str,
                                      char const* chars)
{
  PLUGIN_TRACE_JNIENV ();
  PR_Free ((void*) chars);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetArrayLength (jarray array,
                               jsize* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (array);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (reference, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewObjectArray (jsize len,
                               jclass clazz,
                               jobject init,
                               jobjectArray* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_SIZE (len);
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_REFERENCE (init);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jobjectArray, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetObjectArrayElement (jobjectArray array,
                                      jsize index,
                                      jobject* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (array);
  MESSAGE_ADD_SIZE (index);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jobject, result);
  DecrementContextCounter ();
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::SetObjectArrayElement (jobjectArray array,
                                      jsize index,
                                      jobject val)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(-1);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_REFERENCE (array);
  MESSAGE_ADD_SIZE (index);
  MESSAGE_ADD_REFERENCE (val);
  MESSAGE_SEND ();
  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::NewArray (jni_type element_type,
                         jsize len,
                         jarray* result)
{
  PLUGIN_TRACE_JNIENV ();
  int reference = IncrementContextCounter ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_STACK_REFERENCE(reference);
  MESSAGE_ADD_FUNC();
  MESSAGE_ADD_TYPE (element_type);
  MESSAGE_ADD_SIZE (len);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (reference, jarray, result);
  DecrementContextCounter ();
  return NS_OK;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::GetArrayElements (jni_type type,
                                 jarray array,
                                 jboolean* isCopy,
                                 void* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::ReleaseArrayElements (jni_type type,
                                     jarray array,
                                     void* elems,
                                     jint mode)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::GetArrayRegion (jni_type type,
                               jarray array,
                               jsize start,
                               jsize len,
                               void* buf)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::SetArrayRegion (jni_type type,
                               jarray array,
                               jsize start,
                               jsize len,
                               void* buf)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::RegisterNatives (jclass clazz,
                                JNINativeMethod const* methods,
                                jint nMethods,
                                jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::UnregisterNatives (jclass clazz,
                                  jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::MonitorEnter (jobject obj,
                             jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::MonitorExit (jobject obj,
                            jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// FIXME: never called?
// see nsJVMManager.cpp:878
// Not used.
NS_IMETHODIMP
IcedTeaJNIEnv::GetJavaVM (JavaVM** vm,
                          jint* result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

// Module loading function.  See nsPluginsDirUNIX.cpp.
// FIXME: support unloading, free'ing memory NSGetFactory allocates?
extern "C" NS_EXPORT nsresult
NSGetFactory (nsISupports* aServMgr, nsCID const& aClass,
              char const* aClassName, char const* aContractID,
              nsIFactory** aFactory)
{
  PLUGIN_DEBUG_0ARG("NSGetFactory called\n");

  static NS_DEFINE_CID (PluginCID, NS_PLUGIN_CID);
  if (!aClass.Equals (PluginCID))
    return NS_ERROR_FACTORY_NOT_LOADED;

  // Set appletviewer_executable.
  Dl_info info;
  char* filename = NULL;
  if (dladdr (reinterpret_cast<void const*> (NSGetFactory),
              &info) == 0)
    {
      PLUGIN_ERROR_TWO ("Failed to determine plugin shared object filename",
                        dlerror ());
      return NS_ERROR_FAILURE;
    }
  // Freed below.
 filename = strdup (info.dli_fname);
  if (!filename)
    {
      PLUGIN_ERROR ("Failed to create plugin shared object filename.");
      return NS_ERROR_OUT_OF_MEMORY;
    }
  nsCString executable (dirname (filename));

  free (filename);
  filename = NULL;

  executable += nsCString ("/../../bin/java");

  // Never freed.
  appletviewer_executable = strdup (executable.get ());
  if (!appletviewer_executable)
    {
      PLUGIN_ERROR ("Failed to create java executable name.");
      return NS_ERROR_OUT_OF_MEMORY;
    }

  // Make sure the plugin data directory exists, creating it if
  // necessary.
  data_directory = g_strconcat (getenv ("HOME"), "/.icedteaplugin", NULL);
  if (!data_directory)
    {
      PLUGIN_ERROR ("Failed to create data directory name.");
      return NS_ERROR_OUT_OF_MEMORY;
    }

  if (!g_file_test (data_directory,
                    (GFileTest) (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)))
    {
      int file_error = 0;

      file_error = g_mkdir (data_directory, 0700);
      if (file_error != 0)
        {
          PLUGIN_ERROR_THREE ("Failed to create data directory",
                              data_directory,
                              strerror (errno));
         
          if (data_directory)
          {
            g_free (data_directory);
            data_directory = NULL;
           };

           return NS_ERROR_UNEXPECTED;
        }
    }

  if (factory_created == PR_TRUE)
  {
	  // wait for factory to initialize
	  while (!factory)
	  {
		  PR_Sleep(200);
		  PLUGIN_DEBUG("Waiting for factory to be created...");
	  }


      PLUGIN_DEBUG("NSGetFactory: Returning existing factory");

	  *aFactory = factory;
	  NS_ADDREF (factory);
  } else
  {
    factory_created = PR_TRUE;
    PLUGIN_DEBUG("NSGetFactory: Creating factory");
    factory = new IcedTeaPluginFactory ();
    if (!factory)
      return NS_ERROR_OUT_OF_MEMORY;

    NS_ADDREF (factory);
    *aFactory = factory;
  }

  return NS_OK;
}

// FIXME: replace with NS_GetCurrentThread.
PRThread*
current_thread ()
{
  nsCOMPtr<nsIComponentManager> manager;
  nsresult result = NS_GetComponentManager (getter_AddRefs (manager));
  PLUGIN_CHECK ("get component manager", result);

  nsCOMPtr<nsIThreadManager> threadManager;
  result = manager->CreateInstanceByContractID
    (NS_THREADMANAGER_CONTRACTID, nsnull, NS_GET_IID (nsIThreadManager),
     getter_AddRefs (threadManager));
  PLUGIN_CHECK ("thread manager", result);

  // threadManager is NULL during shutdown.
  if (threadManager)
    {
      nsCOMPtr<nsIThread> current;
      result = threadManager->GetCurrentThread (getter_AddRefs (current));
      //  PLUGIN_CHECK ("current thread", result);

      PRThread* threadPointer;
      result = current->GetPRThread (&threadPointer);
      //  PLUGIN_CHECK ("thread pointer", result);

      return threadPointer;
    }
  else
    return NULL;
}
