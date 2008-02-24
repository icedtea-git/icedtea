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

#define NOT_IMPLEMENTED() \
  printf ("NOT IMPLEMENTED: %s\n", __PRETTY_FUNCTION__)

#define ID(object) \
  (object == NULL ? (PRUint32) 0 : reinterpret_cast<JNIReference*> (object)->identifier)

// Tracing.
class Trace
{
public:
  Trace (char const* name, char const* function)
  {
    Trace::name = name;
    Trace::function = function;
    printf ("ICEDTEA PLUGIN: thread %p: %s%s\n", current_thread (),
             name, function);
  }

  ~Trace ()
  {
    printf ("ICEDTEA PLUGIN: thread %p: %s%s %s\n", current_thread (),
             name, function, "return");
  }
private:
  char const* name;
  char const* function;
};

#if 0
// Debugging macros.
#define PLUGIN_DEBUG(message)                                           \
  printf ("ICEDTEA PLUGIN: thread %p: %s\n", current_thread (), message)

#define PLUGIN_DEBUG_TWO(first, second)                                 \
  printf ("ICEDTEA PLUGIN: thread %p: %s %s\n", current_thread (),      \
           first, second)

// Testing macro.
#define PLUGIN_TEST(expression, message)  \
  do                                            \
    {                                           \
      if (!(expression))                        \
        printf ("FAIL: %d: %s\n", __LINE__,     \
                message);                       \
    }                                           \
  while (0);

// __func__ is a variable, not a string literal, so it cannot be
// concatenated by the preprocessor.
#define PLUGIN_TRACE_JNIENV() Trace _trace ("JNIEnv::", __func__)
#define PLUGIN_TRACE_FACTORY() Trace _trace ("Factory::", __func__)
#define PLUGIN_TRACE_INSTANCE() Trace _trace ("Instance::", __func__)
#define PLUGIN_TRACE_EVENTSINK() Trace _trace ("EventSink::", __func__)
#define PLUGIN_TRACE_LISTENER() Trace _trace ("Listener::", __func__)

// Error reporting macros.
#define PLUGIN_ERROR(message)                                       \
  fprintf (stderr, "%s:%d: thread %p: Error: %s\n", __FILE__, __LINE__,  \
           current_thread (), message)

#define PLUGIN_ERROR_TWO(first, second)                                 \
  fprintf (stderr, "%s:%d: thread %p: Error: %s: %s\n", __FILE__, __LINE__,  \
           current_thread (), first, second)

#define PLUGIN_ERROR_THREE(first, second, third)                        \
  fprintf (stderr, "%s:%d: thread %p: Error: %s: %s: %s\n", __FILE__,        \
           __LINE__, current_thread (), first, second, third)

#define PLUGIN_CHECK_RETURN(message, result)           \
  if (NS_SUCCEEDED (result))                    \
    PLUGIN_DEBUG (message);                     \
  else                                          \
    {                                           \
      PLUGIN_ERROR (message);                   \
      return result;                            \
    }

#define PLUGIN_CHECK(message, result)           \
  if (NS_SUCCEEDED (result))                    \
    PLUGIN_DEBUG (message);                     \
  else                                          \
    PLUGIN_ERROR (message);

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
  fprintf (stderr, "%s:%d: thread %p: Error: %s\n", __FILE__, __LINE__,  \
           current_thread (), message)

#define PLUGIN_ERROR_TWO(first, second)                                 \
  fprintf (stderr, "%s:%d: thread %p: Error: %s: %s\n", __FILE__, __LINE__,  \
           current_thread (), first, second)

#define PLUGIN_ERROR_THREE(first, second, third)                        \
  fprintf (stderr, "%s:%d: thread %p: Error: %s: %s: %s\n", __FILE__,        \
           __LINE__, current_thread (), first, second, third)
#define PLUGIN_CHECK_RETURN(message, result)
#define PLUGIN_CHECK(message, result)
#endif

#define PLUGIN_NAME "IcedTea Web Browser Plugin"
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
  "application/x-java-applet;version=1.7:class,jar:IcedTea;"           \
  "application/x-java-applet;jpi-version=1.7.0_00:class,jar:IcedTea;"  \
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
  "application/x-java-bean;version=1.7:class,jar:IcedTea;"             \
  "application/x-java-bean;jpi-version=1.7.0_00:class,jar:IcedTea;"

#define FAILURE_MESSAGE "IcedTeaPluginFactory error: Failed to run %s." \
  "  For more detail rerun \"firefox -g\" in a terminal window."

// Global instance counter.
// A global variable for reporting GLib errors.  This must be free'd
// and set to NULL after each use.
static GError* channel_error = NULL;
// Fully-qualified appletviewer executable.
static char* appletviewer_executable = NULL;

#include <nspr.h>

#include <prtypes.h>

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
  printf ("JNIReference CONSTRUCT: %d %p\n", identifier, this);
}

JNIReference::~JNIReference ()
{
  printf ("JNIReference DECONSTRUCT: %d %p\n", identifier, this);
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
  printf ("JNIID CONSTRUCT: %d %p\n", identifier, this);
}

JNIID::~JNIID ()
{
  printf ("JNIID DECONSTRUCT: %d %p\n", identifier, this);
}

char* TYPES[10] = { "Object",
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
  message += " ";                                            \
  message += __func__;

#define MESSAGE_ADD_STRING(name)                \
  message += " ";                               \
  message += name;

#define MESSAGE_ADD_SIZE(size)                  \
  message += " ";                               \
  message.AppendInt (size);

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

#define MESSAGE_RECEIVE_REFERENCE(cast, name)                           \
  PRBool processed = PR_FALSE;                                          \
  nsresult res = NS_OK;                                                 \
  factory->returnIdentifier = -1;                                       \
  printf ("RECEIVE 1\n"); \
  while (factory->returnIdentifier == -1)                               \
    {                                                                   \
  printf ("RECEIVE 2\n"); \
      res = factory->current->ProcessNextEvent (PR_TRUE,                \
                                                &processed);            \
      PLUGIN_CHECK_RETURN (__func__, res);                              \
    }                                                                   \
  printf ("RECEIVE 3\n"); \
  *name =                                                               \
    reinterpret_cast<cast>                                              \
    (factory->references.ReferenceObject (factory->returnIdentifier)); \
  printf ("RECEIVE_REFERENCE: %s result: %x = %d\n",                    \
          __func__, *name, factory->returnIdentifier);

// FIXME: track and free JNIIDs.
#define MESSAGE_RECEIVE_ID(cast, id, signature)                 \
  PRBool processed = PR_FALSE;                                  \
  nsresult result = NS_OK;                                      \
  factory->returnIdentifier = -1;                               \
  while (factory->returnIdentifier == -1)                       \
    {                                                           \
      result = factory->current->ProcessNextEvent (PR_TRUE,     \
                                                   &processed); \
      PLUGIN_CHECK_RETURN (__func__, result);                   \
    }                                                           \
                                                                \
  *id = reinterpret_cast<cast>                                  \
    (new JNIID (factory->returnIdentifier, signature));
//  \
//   printf ("RECEIVE_ID: %s result: %x = %d, %s\n",               \
//           __func__, *id, factory->returnIdentifier,             \
//           signature);

#define MESSAGE_RECEIVE_VALUE(type, result)                     \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  factory->returnValue = "";                                    \
  while (factory->returnValue == "")                            \
    {                                                           \
      res = factory->current->ProcessNextEvent (PR_TRUE,        \
                                                &processed);    \
      PLUGIN_CHECK_RETURN (__func__, res);                      \
    }                                                           \
  *result = ParseValue (type, factory->returnValue);            
// \
//   char* valueString = ValueString (type, *result);              \
//   printf ("RECEIVE_VALUE: %s result: %x = %s\n",                \
//           __func__, result, valueString);                       \
//   free (valueString);                                           \
//   valueString = NULL;

#define MESSAGE_RECEIVE_SIZE(result)                            \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  factory->returnValue = "";                                    \
  while (factory->returnValue == "")                            \
    {                                                           \
      res = factory->current->ProcessNextEvent (PR_TRUE,        \
                                                &processed);    \
      PLUGIN_CHECK_RETURN (__func__, res);                      \
    }                                                           \
  nsresult conversionResult;                                    \
  *result = factory->returnValue.ToInteger (&conversionResult); \
  PLUGIN_CHECK ("parse integer", conversionResult);             
// \
//   printf ("RECEIVE_SIZE: %s result: %x = %d\n",                 \
//           __func__, result, *result);

// strdup'd string must be freed by calling function.
#define MESSAGE_RECEIVE_STRING(char_type, result)               \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  factory->returnValue = "";                                    \
  while (factory->returnValue == "")                            \
    {                                                           \
      res = factory->current->ProcessNextEvent (PR_TRUE,        \
                                                &processed);    \
      PLUGIN_CHECK_RETURN (__func__, res);                      \
    }                                                           \
  *result = reinterpret_cast<char_type const*>                  \
    (strdup (factory->returnValue.get ()));                     
// \
//   printf ("RECEIVE_STRING: %s result: %x = %s\n",               \
//           __func__, result, *result);

// strdup'd string must be freed by calling function.
#define MESSAGE_RECEIVE_STRING_UCS(result)                      \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  factory->returnValueUCS.Truncate ();                          \
  while (factory->returnValueUCS.IsEmpty ())                    \
    {                                                           \
      res = factory->current->ProcessNextEvent (PR_TRUE,        \
                                                &processed);    \
      PLUGIN_CHECK_RETURN (__func__, res);                      \
    }                                                           \
  int length = factory->returnValueUCS.Length ();               \
  jchar* newstring = static_cast<jchar*> (PR_Malloc (length));  \
  memset (newstring, 0, length);                                \
  memcpy (newstring, factory->returnValueUCS.get (), length);   \
  *result = static_cast<jchar const*> (newstring);

// \
//   printf ("RECEIVE_STRING: %s result: %x = %s\n",               \
//           __func__, result, *result);

#define MESSAGE_RECEIVE_BOOLEAN(result)                         \
  PRBool processed = PR_FALSE;                                  \
  nsresult res = NS_OK;                                         \
  factory->returnIdentifier = -1;                               \
  while (factory->returnIdentifier == -1)                       \
    {                                                           \
      res = factory->current->ProcessNextEvent (PR_TRUE,        \
                                                &processed);    \
      PLUGIN_CHECK_RETURN (__func__, res);                      \
    }                                                           \
  *result = factory->returnIdentifier;
// \
//   printf ("RECEIVE_BOOLEAN: %s result: %x = %s\n",              \
//           __func__, result, *result ? "true" : "false");

#include <nscore.h>
#include <nsISupports.h>
#include <nsIFactory.h>

// Factory functions.
extern "C" NS_EXPORT nsresult NSGetFactory (nsISupports* aServMgr,
                                            nsCID const& aClass,
                                            char const* aClassName,
                                            char const* aContractID,
                                            nsIFactory** aFactory);



#include <nsIFactory.h>
#include <nsIPlugin.h>
#include <nsIJVMManager.h>
#include <nsIJVMPrefsWindow.h>
#include <nsIJVMPlugin.h>
#include <nsIInputStream.h>
#include <nsIAsyncInputStream.h>
#include <nsISocketTransport.h>
#include <nsIOutputStream.h>
#include <nsIAsyncInputStream.h>
#include <prthread.h>
#include <nsIThread.h>
#include <nsILocalFile.h>
#include <nsCOMPtr.h>
#include <nsIPluginInstance.h>
#include <nsIPluginInstancePeer.h>
#include <nsIJVMPluginInstance.h>
#include <nsIPluginTagInfo2.h>
#include <nsComponentManagerUtils.h>
#include <nsCOMPtr.h>
#include <nsILocalFile.h>
#include <prthread.h>
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
  printf ("INCREMENTED: %d %p to: %d\n", key, reference, reference->count);
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
  printf ("INCREMENTED: %d %p to: %d\n", key, reference, reference->count);
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
      printf ("DECREMENTED: %d %p to: %d\n", key, reference, reference->count);
      if (reference->count == 0)
        Remove (key);
    }
}

#include <nsTArray.h>
#include <nsILiveconnect.h>

class IcedTeaJNIEnv;

// nsIPlugin inherits from nsIFactory.
class IcedTeaPluginFactory : public nsIPlugin,
                             public nsIJVMManager,
                             public nsIJVMPrefsWindow,
                             public nsIJVMPlugin,
                             public nsIInputStreamCallback
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFACTORY
  NS_DECL_NSIPLUGIN
  NS_DECL_NSIJVMMANAGER
  // nsIJVMPrefsWindow does not provide an NS_DECL macro.
public:
  NS_IMETHOD Show (void);
  NS_IMETHOD Hide (void);
  NS_IMETHOD IsVisible (PRBool* result);
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
  PRUint32 returnIdentifier;
  nsCString returnValue;
  nsString returnValueUCS;
  ReferenceHashtable references;
  // FIXME: make private?
  JNIEnv* proxyEnv;
  nsISecureEnv* secureEnv;
  void GetMember ();
  void SetMember ();
  void GetSlot ();
  void SetSlot ();
  void Eval ();
  void RemoveMember ();
  void Call ();
  void Finalize ();
  void ToString ();
  nsCOMPtr<nsILiveconnect> liveconnect;

private:
  ~IcedTeaPluginFactory();
  nsresult TestAppletviewer ();
  void DisplayFailureDialog ();
  nsresult StartAppletviewer ();
  nsCOMPtr<IcedTeaEventSink> sink;
  nsCOMPtr<nsISocketTransport> transport;
  nsCOMPtr<nsIInputStream> input;
  nsCOMPtr<nsIOutputStream> output;
  PRBool connected;
  PRUint32 next_instance_identifier;
  // Does not do construction/deconstruction or reference counting.
  nsDataHashtable<nsUint32HashKey, IcedTeaPluginInstance*> instances;
  PRUint32 object_identifier_return;
  int javascript_identifier;
  int name_identifier;
  int args_identifier;
  int string_identifier;
  int slot_index;
  int value_identifier;
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

private:

  PLUGIN_JAVASCRIPT_TYPE liveconnect_window;
  gpointer window_handle;
  guint32 window_width;
  guint32 window_height;
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
};

NS_IMPL_ISUPPORTS6 (IcedTeaPluginFactory, nsIFactory, nsIPlugin, nsIJVMManager,
                    nsIJVMPrefsWindow, nsIJVMPlugin, nsIInputStreamCallback)

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
  connected (PR_FALSE)
{
  PLUGIN_TRACE_FACTORY ();
  instances.Init ();
  references.Init ();
  printf ("CONSTRUCTING FACTORY");

  nsCOMPtr<nsIComponentManager> manager;
  nsresult result = NS_GetComponentManager (getter_AddRefs
                                            (manager));
  PLUGIN_CHECK ("get component manager", result);
  result = manager->CreateInstance
    (nsILiveconnect::GetCID (),
     nsnull, NS_GET_IID (nsILiveconnect),
     getter_AddRefs (liveconnect));
  PLUGIN_CHECK ("liveconnect", result);
}

IcedTeaPluginFactory::~IcedTeaPluginFactory ()
{
  // FIXME: why did this crash with threadManager == 0x0 on shutdown?
  PLUGIN_TRACE_FACTORY ();
  secureEnv = 0;
  printf ("DECONSTRUCTING FACTORY");
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

  // Start appletviewer process for this plugin instance.
  nsCOMPtr<nsIComponentManager> manager;
  result = NS_GetComponentManager (getter_AddRefs (manager));
  PLUGIN_CHECK_RETURN ("get component manager", result);

  nsCOMPtr<nsIServerSocket> socket;
  result = manager->CreateInstanceByContractID (NS_SERVERSOCKET_CONTRACTID,
                                                nsnull,
                                                NS_GET_IID (nsIServerSocket),
                                                getter_AddRefs (socket));
  PLUGIN_CHECK_RETURN ("create server socket", result);

  // FIXME: hard-coded port
  result = socket->Init (50007, PR_TRUE, -1);
  PLUGIN_CHECK_RETURN ("socket init", result);

  nsCOMPtr<IcedTeaSocketListener> listener = new IcedTeaSocketListener (this);
  result = socket->AsyncListen (listener);
  PLUGIN_CHECK_RETURN ("add socket listener", result);

  result = StartAppletviewer ();
  PLUGIN_CHECK_RETURN ("started appletviewer", result);

  nsCOMPtr<nsIThreadManager> threadManager;
  result = manager->CreateInstanceByContractID
    (NS_THREADMANAGER_CONTRACTID, nsnull, NS_GET_IID (nsIThreadManager),
     getter_AddRefs (threadManager));
  PLUGIN_CHECK_RETURN ("thread manager", result);

  result = threadManager->GetCurrentThread (getter_AddRefs (current));
  PLUGIN_CHECK_RETURN ("current thread", result);

  PLUGIN_DEBUG ("Instance::Initialize: awaiting connection from appletviewer");
  PRBool processed;
  // FIXME: move this somewhere applet-window specific so it doesn't block page
  // display.
  // FIXME: this doesn't work with thisiscool.com.
  while (!IsConnected ())
    {
      result = current->ProcessNextEvent (PR_TRUE, &processed);
      PLUGIN_CHECK_RETURN ("wait for connection: process next event", result);
    }
  PLUGIN_DEBUG ("Instance::Initialize:"
                " got confirmation that appletviewer is running.");

  result = transport->OpenOutputStream (nsITransport::OPEN_BLOCKING,
                                        nsnull, nsnull,
                                        getter_AddRefs (output));
  PLUGIN_CHECK_RETURN ("output stream", result);

  result = transport->OpenInputStream (0, nsnull, nsnull,
                                       getter_AddRefs (input));
  PLUGIN_CHECK_RETURN ("input stream", result);

  async = do_QueryInterface (input, &result);
  PLUGIN_CHECK_RETURN ("async input stream", result);

  result = async->AsyncWait (this, 0, 0, current);
  PLUGIN_CHECK_RETURN ("add async wait", result);

  return result;
}

NS_IMETHODIMP
IcedTeaPluginFactory::Shutdown ()
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
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

// nsIJVMPrefsWindow functions.
NS_IMETHODIMP
IcedTeaPluginFactory::Show (void)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::Hide (void)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
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

  printf ("CREATESECUREENV\n");
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
  printf ("HERE1\n");
  (*outSecureEnv)->NewObject (clazz, method, NULL, &newobject);
  printf ("HERE2\n");
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

  
  printf ("HERE3\n");
  (*outSecureEnv)->NewGlobalRef (newobject, &newglobalobject);
  printf ("HERE4\n");
  (*outSecureEnv)->DeleteLocalRef (newobject);
  printf ("HERE5\n");
  (*outSecureEnv)->DeleteGlobalRef (newglobalobject);
  printf ("HERE6\n");

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
  printf ("GOT METHOD: %d\n", reinterpret_cast<JNIID*> (method)->identifier);
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

NS_IMPL_ISUPPORTS2 (IcedTeaPluginInstance, nsIPluginInstance,
                    nsIJVMPluginInstance)

NS_IMETHODIMP
IcedTeaPluginInstance::Initialize (nsIPluginInstancePeer* aPeer)
{
  PLUGIN_TRACE_INSTANCE ();

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
  factory->SendMessageToAppletViewer (tagMessage);

  // Set back-pointer to peer instance.
  printf ("SETTING PEER!!!: %p\n", aPeer);
  peer = aPeer;
  NS_ADDREF (aPeer);
  printf ("DONE SETTING PEER!!!: %p\n", aPeer);

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetPeer (nsIPluginInstancePeer** aPeer)
{
  printf ("GETTING PEER!!!: %p\n", peer);
  *aPeer = peer;
  // FIXME: where is this unref'd?
  NS_ADDREF (peer);
  printf ("DONE GETTING PEER!!!: %p, %p\n", peer, *aPeer);
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

  nsCString destroyMessage (instanceIdentifierPrefix);
  destroyMessage += "destroy";
  factory->SendMessageToAppletViewer (destroyMessage);

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
      // The window already exists.
      if (window_handle == aWindow->window)
	{
          // The parent window is the same as in previous calls.
          PLUGIN_DEBUG ("Instance::SetWindow: window already exists.");

          // The window is the same as it was for the last
          // SetWindow call.
          if (aWindow->width != window_width)
            {
              PLUGIN_DEBUG ("Instance::SetWindow: window width changed.");
              // The width of the plugin window has changed.

              // Send the new width to the appletviewer.
              nsCString widthMessage (instanceIdentifierPrefix);
              widthMessage += "width ";
              widthMessage.AppendInt (aWindow->width);
              factory->SendMessageToAppletViewer (widthMessage);

              // Store the new width.
              window_width = aWindow->width;
            }

          if (aWindow->height != window_height)
            {
              PLUGIN_DEBUG ("Instance::SetWindow: window height changed.");
              // The height of the plugin window has changed.

              // Send the new height to the appletviewer.
              nsCString heightMessage (instanceIdentifierPrefix);
              heightMessage += "height ";
              heightMessage.AppendInt (aWindow->height);
              factory->SendMessageToAppletViewer (heightMessage);

              // Store the new height.
              window_height = aWindow->height;
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

  return factory->GetJavaObject (instance_identifier, object);
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

  nsCString objectMessage ("instance ");
  objectMessage.AppendInt (instance_identifier);
  objectMessage += " GetJavaObject";
  SendMessageToAppletViewer (objectMessage);

  PRBool processed = PR_FALSE;
  nsresult result = NS_OK;
  while (object_identifier_return == 0)
    {
      result = current->ProcessNextEvent (PR_TRUE, &processed);
      PLUGIN_CHECK_RETURN ("wait for java object: process next event", result);
    }
  //printf ("GOT JAVA OBJECT IDENTIFIER: %d\n", object_identifier_return);
  if (object_identifier_return == 0)
    printf ("WARNING: received object identifier 0\n");

  *object = references.ReferenceObject (object_identifier_return);

  return NS_OK;
}

NS_IMETHODIMP
IcedTeaPluginInstance::GetText (char const** result)
{
  NOT_IMPLEMENTED ();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IcedTeaPluginFactory::OnInputStreamReady (nsIAsyncInputStream* aStream)
{
  PLUGIN_TRACE_INSTANCE ();

  // FIXME: change to NSCString.  Why am I getting symbol lookup errors?
  // /home/fitzsim/sources/mozilla/dist/bin/firefox-bin: symbol lookup error:
  // /usr/lib/jvm/java-1.7.0-icedtea-1.7.0.0/jre/lib/i386/IcedTeaPlugin.so:
  // undefined symbol: _ZNK10nsACString12BeginReadingEv
  char message[10000];
  message[0] = 0;
  char byte = 0;
  PRUint32 readCount = 0;
  int index = 0;

  printf ("ONINPUTSTREAMREADY 1 %p\n", current_thread());
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

  HandleMessage (nsCString (message));

  nsresult result = async->AsyncWait (this, 0, 0, current);
  PLUGIN_CHECK_RETURN ("re-add async wait", result);

  return NS_OK;
}

#include <nsIProcess.h>
#include <nsIServerSocket.h>
#include <nsNetError.h>
#include <nsPIPluginInstancePeer.h>
#include <nsIPluginInstanceOwner.h>
#include <nsIRunnable.h>

void
IcedTeaPluginFactory::HandleMessage (nsCString const& message)
{
  PLUGIN_DEBUG_TWO ("received message:", message.get());

  nsresult conversionResult;
  PRUint32 space = message.FindChar (' ');
  nsDependentCSubstring prefix = Substring (message, 0, space);
  nsDependentCSubstring rest_prefix = Substring (message, space + 1);
  space = rest_prefix.FindChar (' ');
  PRUint32 identifier =
    Substring (rest_prefix, 0, space).ToInteger (&conversionResult);
  PLUGIN_CHECK ("parse integer", conversionResult);
  nsDependentCSubstring rest_command = Substring (rest_prefix, space + 1);
  space = rest_command.FindChar (' ');
  nsDependentCSubstring command = Substring (rest_command, 0, space);
  nsDependentCSubstring rest = Substring (rest_command, space + 1);

  if (prefix == "instance")
    {
      if (command == "status")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0)
            instance->peer->ShowStatus (nsCString (rest).get ());
        }
      else if (command == "url")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0)
            {
              space = rest.FindChar (' ');
              nsDependentCSubstring url = Substring (rest, 0, space);
              nsDependentCSubstring target = Substring (rest, space + 1);
              nsCOMPtr<nsPIPluginInstancePeer> ownerGetter =
                do_QueryInterface (instance->peer);
              PLUGIN_CHECK (owner, "get plugin owner");
              nsIPluginInstanceOwner* owner = nsnull;
              ownerGetter->GetOwner (&owner);
              owner->GetURL (nsCString (url).get (),
                             nsCString (target).get (),
                             nsnull, 0, nsnull, 0);
            }
        }
      else if (command == "GetWindow")
        {
          IcedTeaPluginInstance* instance = NULL;
          instances.Get (identifier, &instance);
          if (instance != 0)
            {
              nsCOMPtr<nsIRunnable> event =
                NS_NEW_RUNNABLE_METHOD(IcedTeaPluginInstance, instance,
                                       IcedTeaPluginInstance::GetWindow);
              NS_DispatchToMainThread (event);
            }
        }
      else if (command == "GetMember")
        {
          printf ("POSTING GetMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring nameID = Substring (rest, space + 1);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::GetMember);
          NS_DispatchToMainThread (event);
          printf ("POSTING GetMember DONE\n");
        }
      else if (command == "SetMember")
        {
          printf ("POSTING SetMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
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
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::SetMember);
          NS_DispatchToMainThread (event);
          printf ("POSTING SetMember DONE\n");
        }
      else if (command == "GetSlot")
        {
          printf ("POSTING GetSlot\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring indexStr = Substring (rest, space + 1);
          slot_index = indexStr.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::GetSlot);
          NS_DispatchToMainThread (event);
          printf ("POSTING GetSlot DONE\n");
        }
      else if (command == "SetSlot")
        {
          printf ("POSTING SetSlot\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring nameAndValue = Substring (rest, space + 1);
          space = nameAndValue.FindChar (' ');
          nsDependentCSubstring indexStr = Substring (nameAndValue, 0, space);
          slot_index = indexStr.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);
          nsDependentCSubstring valueID = Substring (nameAndValue, space + 1);
          value_identifier = valueID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse value id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::SetSlot);
          NS_DispatchToMainThread (event);
          printf ("POSTING SetSlot DONE\n");
        }
      else if (command == "Eval")
        {
          printf ("POSTING Eval\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring stringID = Substring (rest, space + 1);
          string_identifier = stringID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse string id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::Eval);
          NS_DispatchToMainThread (event);
          printf ("POSTING Eval DONE\n");
        }
      else if (command == "RemoveMember")
        {
          printf ("POSTING RemoveMember\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring nameID = Substring (rest, space + 1);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse name id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::RemoveMember);
          NS_DispatchToMainThread (event);
          printf ("POSTING RemoveMember DONE\n");
        }
      else if (command == "Call")
        {
          printf ("POSTING Call\n");
          space = rest.FindChar (' ');
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = javascriptID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);
          nsDependentCSubstring nameAndArgs = Substring (rest, space + 1);
          space = nameAndArgs.FindChar (' ');
          nsDependentCSubstring nameID = Substring (nameAndArgs, 0, space);
          name_identifier = nameID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse method name id", conversionResult);
          nsDependentCSubstring argsID = Substring (nameAndArgs, space + 1);
          args_identifier = argsID.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse args id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::Call);
          NS_DispatchToMainThread (event);
          printf ("POSTING Call DONE\n");
        }
      else if (command == "Finalize")
        {
          printf ("POSTING Finalize\n");
          nsDependentCSubstring javascriptID = Substring (rest, 0, space);
          javascript_identifier = rest.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::Finalize);
          NS_DispatchToMainThread (event);
          printf ("POSTING Finalize DONE\n");
        }
      else if (command == "ToString")
        {
          printf ("POSTING ToString\n");
          javascript_identifier = rest.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse javascript id", conversionResult);

          nsCOMPtr<nsIRunnable> event =
            NS_NEW_RUNNABLE_METHOD(IcedTeaPluginFactory, this,
                                   IcedTeaPluginFactory::ToString);
          NS_DispatchToMainThread (event);
          printf ("POSTING ToString DONE\n");
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
          returnIdentifier = rest.ToInteger (&conversionResult);
          PLUGIN_CHECK ("parse integer", conversionResult);
          //printf ("GOT RETURN IDENTIFIER %d\n", returnIdentifier);
        }
      else if (command == "GetField"
               || command == "GetStaticField"
               || command == "CallStaticMethod"
               || command == "GetArrayLength"
               || command == "GetStringUTFLength"
               || command == "GetStringLength"
               || command == "CallMethod")
        {
          if (returnValue != "")
            PLUGIN_ERROR ("Return value already defined.");

          returnValue = rest;
          //printf ("PLUGIN GOT RETURN VALUE: %s\n", returnValue);
        }
      else if (command == "GetStringUTFChars")
        {
          if (returnValue != "")
            PLUGIN_ERROR ("Return value already defined.");

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
          // printf ("PLUGIN GOT RETURN UTF-8 STRING: %s\n", returnValue.get ());
        }
      else if (command == "GetStringChars")
        {
          if (!returnValueUCS.IsEmpty ())
            PLUGIN_ERROR ("Return value already defined.");

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
          printf ("PLUGIN GOT RETURN UTF-16 STRING: %d: ",
                  returnValueUCS.Length());
          for (int i = 0; i < returnValueUCS.Length(); i++)
            {
              if ((returnValueUCS[i] >= 'A'
                   && returnValueUCS[i] <= 'Z')
                  || (returnValueUCS[i] >= 'a'
                      && returnValueUCS[i] <= 'z')
                  || (returnValueUCS[i] >= '0'
                      && returnValueUCS[i] <= '9'))
                printf ("%c", returnValueUCS[i]);
              else
                printf ("?");
            }
          printf ("\n");
        }
      // Do nothing for: SetStaticField, SetField, ExceptionClear,
      // DeleteGlobalRef, DeleteLocalRef
    }
}

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

  nsCOMPtr<nsIProcess> process;
  result = manager->CreateInstanceByContractID (NS_PROCESS_CONTRACTID,
                                                nsnull,
                                                NS_GET_IID (nsIProcess),
                                                getter_AddRefs (process));
  PLUGIN_CHECK_RETURN ("create process", result);

  result = process->Init (file);
  PLUGIN_CHECK_RETURN ("init process", result);

  // FIXME: hard-coded port number.
  char const* args[1] = { "50007" };
  result = process->Run (PR_FALSE, args, 1, nsnull);
  PLUGIN_CHECK_RETURN ("run process", result);

  return NS_OK;
}

nsresult
IcedTeaPluginFactory::SendMessageToAppletViewer (nsCString& message)
{
  PLUGIN_TRACE_INSTANCE ();

  PRUint32 writeCount = 0;
  // Write trailing \0 as message termination character.
  // FIXME: check that message is a valid UTF-8 string.
  //  printf ("MESSAGE: %s\n", message.get ());
  nsresult result = output->Write (message.get (),
                                   message.Length () + 1,
                                   &writeCount);
  PLUGIN_CHECK_RETURN ("wrote bytes", result);
  if (writeCount != message.Length () + 1)
    {
      PLUGIN_ERROR ("Failed to write all bytes.");
      return NS_ERROR_FAILURE;
    }

  result = output->Flush ();
  PLUGIN_CHECK_RETURN ("flushed output", result);

  printf ("  PIPE: plugin wrote: %s\n", message.get ());

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
  instanceIdentifierPrefix ("")
{
  PLUGIN_TRACE_INSTANCE ();
  IcedTeaPluginInstance::factory = factory;
  instance_identifier = factory->RegisterInstance (this);

  instanceIdentifierPrefix += "instance ";
  instanceIdentifierPrefix.AppendInt (instance_identifier);
  instanceIdentifierPrefix += " ";
}

void
IcedTeaPluginInstance::GetWindow ()
{
  nsresult result;
  printf ("HERE 22: %d\n", liveconnect_window);
  // principalsArray, numPrincipals and securitySupports
  // are ignored by GetWindow.  See:
  //
  // nsCLiveconnect.cpp: nsCLiveconnect::GetWindow
  // jsj_JSObject.c: jsj_enter_js
  // lcglue.cpp: enter_js_from_java_impl
  // so they can all safely be null.
  if (factory->proxyEnv != NULL)
    {
      printf ("HERE 23: %d, %p\n", liveconnect_window, current_thread());
      result = factory->liveconnect->GetWindow(factory->proxyEnv,
                                               this,
                                               NULL, 0, NULL,
                                               &liveconnect_window);
      PLUGIN_CHECK ("get window", result);
      printf ("HERE 24: %d\n", liveconnect_window);
    }

  printf ("HERE 20: %d\n", liveconnect_window);

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptGetWindow";
  message += " ";
  message.AppendInt (liveconnect_window);
  factory->SendMessageToAppletViewer (message);
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
  printf ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  printf ("AFTER GETTING NAMESTRING\n");

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      printf ("Calling GETMEMBER: %d, %d\n", javascript_identifier, strSize);
      result = liveconnect->GetMember(proxyEnv,
                                      javascript_identifier,
                                      nameString, strSize,
                                      NULL, 0, NULL,
                                      &liveconnect_member);
      PLUGIN_CHECK ("get member", result);
    }

  printf ("GOT MEMBER: %d\n", ID (liveconnect_member));
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
      result = liveconnect->GetSlot(proxyEnv,
                                      javascript_identifier,
                                      slot_index,
                                      NULL, 0, NULL,
                                      &liveconnect_member);
      PLUGIN_CHECK ("get slot", result);
    }

  printf ("GOT SLOT: %d\n", ID (liveconnect_member));
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
  printf ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  printf ("AFTER GETTING NAMESTRING\n");

  jobject value = references.ReferenceObject (value_identifier);
  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      printf ("Calling SETMEMBER: %d, %d\n", javascript_identifier, strSize);
      result = liveconnect->SetMember(proxyEnv,
                                      javascript_identifier,
                                      nameString, strSize,
                                      value,
                                      NULL, 0, NULL);
      PLUGIN_CHECK ("set member", result);
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
      result = liveconnect->SetSlot(proxyEnv,
                                    javascript_identifier,
                                    slot_index,
                                    value,
                                    NULL, 0, NULL);
      PLUGIN_CHECK ("set slot", result);
    }

  nsCString message ("context ");
  message.AppendInt (0);
  message += " ";
  message += "JavaScriptSetSlot";
  SendMessageToAppletViewer (message);
}

void
IcedTeaPluginFactory::Eval ()
{
  nsresult result;
  printf ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  // FIXME: unreference after SendMessageToAppletViewer call.
  jstring name = static_cast<jstring> (references.ReferenceObject (string_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  printf ("AFTER GETTING NAMESTRING\n");

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      printf ("Calling Eval: %d, %d\n", javascript_identifier, strSize);
      result = liveconnect->Eval(proxyEnv,
                                 javascript_identifier,
                                 nameString, strSize,
                                 NULL, 0, NULL,
                                 &liveconnect_member);
      PLUGIN_CHECK ("eval", result);
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
  printf ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  printf ("AFTER GETTING NAMESTRING\n");

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      printf ("Calling RemoveMember: %d, %d\n", javascript_identifier, strSize);
      result = liveconnect->RemoveMember(proxyEnv,
                                         javascript_identifier,
                                         nameString, strSize,
                                         NULL, 0, NULL);
      PLUGIN_CHECK ("RemoveMember", result);
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
  printf ("BEFORE GETTING NAMESTRING\n");
  jsize strSize = 0;
  jchar const* nameString;
  jstring name = static_cast<jstring> (
    references.ReferenceObject (name_identifier));
  ((IcedTeaJNIEnv*) secureEnv)->GetStringLength (name, &strSize);
  ((IcedTeaJNIEnv*) secureEnv)->GetStringChars (name, NULL, &nameString);
  printf ("AFTER GETTING NAMESTRING\n");
  jobjectArray args = static_cast<jobjectArray> (
    references.ReferenceObject (args_identifier));

  jobject liveconnect_member;
  if (proxyEnv != NULL)
    {
      printf ("CALL: %d, %d\n", javascript_identifier, strSize);
      result = liveconnect->Call(proxyEnv,
                                 javascript_identifier,
                                 nameString, strSize,
                                 args,
                                 NULL, 0, NULL,
                                 &liveconnect_member);
      PLUGIN_CHECK ("call", result);
    }

  printf ("GOT RETURN FROM CALL : %d\n", ID (liveconnect_member));
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
      printf ("FINALIZE: %d\n", javascript_identifier);
      result = liveconnect->FinalizeJSObject(proxyEnv,
                                             javascript_identifier);
      PLUGIN_CHECK ("finalize", result);
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
      printf ("Calling ToString: %d\n", javascript_identifier);
      result = liveconnect->ToString(proxyEnv,
                                     javascript_identifier,
                                     &liveconnect_member);
      PLUGIN_CHECK ("ToString", result);
    }

  printf ("ToString: %d\n", ID (liveconnect_member));
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
  PLUGIN_TRACE_INSTANCE ();
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

  return NS_OK;
}

// FIXME: handle appletviewer crash and shutdown scenarios.
NS_IMETHODIMP
IcedTeaSocketListener::OnStopListening (nsIServerSocket *aServ,
                                        nsresult aStatus)
{
  PLUGIN_TRACE_LISTENER ();

  nsresult result = NS_OK;

  switch (aStatus)
  {
  case NS_ERROR_ABORT:
    PLUGIN_DEBUG ("appletviewer stopped");
    // FIXME: privatize?
    result = factory->async->AsyncWait (nsnull, 0, 0, factory->current);
    PLUGIN_CHECK_RETURN ("clear async wait", result);
    break;
  default:
    printf ("ERROR %x\n", aStatus);
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
#include <nsServiceManagerUtils.h>

IcedTeaJNIEnv::IcedTeaJNIEnv (IcedTeaPluginFactory* factory)
: factory (factory)
{
  PLUGIN_TRACE_JNIENV ();
}

IcedTeaJNIEnv::~IcedTeaJNIEnv ()
{
  PLUGIN_TRACE_JNIENV ();
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewObject (jclass clazz,
                          jmethodID methodID,
                          jvalue* args,
                          jobject* result,
                          nsISecurityContext* ctx)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jobject, result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (type, result);
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
      retstr.AppendInt (value.j);
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
      printf ("WARNING: didn't handle parse type\n");
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
          retstr.AppendInt (args[arg].j);
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
          printf ("FAILED ID: %d\n", id->identifier);
          break;
        }
      i++;
    }

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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (type, result);
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
  MESSAGE_CREATE ();
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (methodID);
  MESSAGE_ADD_ARGS (methodID, args);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (type, result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_ID (fieldID);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_VALUE (type, result);
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
  MESSAGE_CREATE ();
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_STRING (name);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jclass, clazz);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetSuperclass (jclass sub,
                              jclass* super)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (sub);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jclass, super);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::IsAssignableFrom (jclass sub,
                                 jclass super,
                                 jboolean* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (sub);
  MESSAGE_ADD_REFERENCE (super);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_BOOLEAN (result);
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
  MESSAGE_CREATE ();
  MESSAGE_SEND ();
  // FIXME: potential leak here: when is result free'd?
  MESSAGE_RECEIVE_REFERENCE (jthrowable, result);
  printf ("GOT RESUlT: %x\n", *result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (lobj);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE(jobject, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::DeleteGlobalRef (jobject gref)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
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
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_SEND ();
  factory->references.UnreferenceObject (ID (obj));
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jclass, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::IsInstanceOf (jobject obj,
                             jclass clazz,
                             jboolean* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (obj);
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_BOOLEAN (result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetMethodID (jclass clazz,
                            char const* name,
                            char const* sig,
                            jmethodID* id)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  printf ("SIGNATURE: %s %s\n", __func__, sig);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (jmethodID, id, sig);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetFieldID (jclass clazz,
                           char const* name,
                           char const* sig,
                           jfieldID* id)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  printf ("SIGNATURE: %s %s\n", __func__, sig);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (jfieldID, id, sig);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStaticMethodID (jclass clazz,
                                  char const* name,
                                  char const* sig,
                                  jmethodID* id)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  printf ("SIGNATURE: %s %s\n", __func__, sig);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (jmethodID, id, sig);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStaticFieldID (jclass clazz,
                                 char const* name,
                                 char const* sig,
                                 jfieldID* id)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_STRING (name);
  printf ("SIGNATURE: %s %s\n", __func__, sig);
  MESSAGE_ADD_STRING (sig);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_ID (jfieldID, id, sig);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewString (jchar const* unicode,
                          jsize len,
                          jstring* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_SIZE (len);
  MESSAGE_ADD_STRING_UCS (unicode, len);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jstring, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringLength (jstring str,
                                jsize* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_STRING_UCS (result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_STRING_UTF (utf);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jstring, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetStringUTFLength (jstring str,
                                   jsize* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (str);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_STRING (char, result);
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (array);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_SIZE (result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::NewObjectArray (jsize len,
                               jclass clazz,
                               jobject init,
                               jobjectArray* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_SIZE (len);
  MESSAGE_ADD_REFERENCE (clazz);
  MESSAGE_ADD_REFERENCE (init);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jobjectArray, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::GetObjectArrayElement (jobjectArray array,
                                      jsize index,
                                      jobject* result)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
  MESSAGE_ADD_REFERENCE (array);
  MESSAGE_ADD_SIZE (index);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jobject, result);
  return NS_OK;
}

NS_IMETHODIMP
IcedTeaJNIEnv::SetObjectArrayElement (jobjectArray array,
                                      jsize index,
                                      jobject val)
{
  PLUGIN_TRACE_JNIENV ();
  MESSAGE_CREATE ();
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
  MESSAGE_CREATE ();
  MESSAGE_ADD_TYPE (element_type);
  MESSAGE_ADD_SIZE (len);
  MESSAGE_SEND ();
  MESSAGE_RECEIVE_REFERENCE (jarray, result);
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
  static NS_DEFINE_CID (PluginCID, NS_PLUGIN_CID);
  if (!aClass.Equals (PluginCID))
    return NS_ERROR_FACTORY_NOT_LOADED;

  IcedTeaPluginFactory* factory = new IcedTeaPluginFactory ();
  if (!factory)
    return NS_ERROR_OUT_OF_MEMORY;

  NS_ADDREF (factory);
  *aFactory = factory;

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
  nsCString executableString (dirname (filename));
  free (filename);
  filename = NULL;

  executableString += nsCString ("/../../bin/pluginappletviewer");

  // Never freed.
  appletviewer_executable = strdup (executableString.get ());
  if (!appletviewer_executable)
    {
      PLUGIN_ERROR ("Failed to create appletviewer executable name.");
      return NS_ERROR_OUT_OF_MEMORY;
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
