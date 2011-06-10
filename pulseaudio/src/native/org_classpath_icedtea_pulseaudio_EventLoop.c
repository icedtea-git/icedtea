/* org_classpath_icedtea_pulseaudio_EventLoop.c
 Copyright (C) 2008 Red Hat, Inc.

 This file is part of IcedTea.

 IcedTea is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 2.

 IcedTea is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with IcedTea; see the file COPYING.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
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
 exception statement from your version.
 */

#include <pulse/pulseaudio.h>

#include "org_classpath_icedtea_pulseaudio_EventLoop.h"
#include "jni-common.h"

#include <poll.h>

const int PA_ITERATE_BLOCK = 1;
const int PA_ITERATE_NOBLOCK = 0;

static java_context_t* java_context = NULL;

JNIEnv* pulse_thread_env = NULL;

void sink_list_success_cb(pa_context *context, const pa_sink_info *i, int eol,
        void *userdata) {

    if (eol == 0) {
        jclass cls = (*pulse_thread_env)->GetObjectClass(pulse_thread_env,
                java_context->obj);
        assert(cls);
        jstring name = (*pulse_thread_env)->NewStringUTF(pulse_thread_env, i->name);
        assert(name);
        jmethodID mid1 = (*pulse_thread_env)->GetMethodID(pulse_thread_env, cls,
                "sink_callback", "(Ljava/lang/String;)V");
        assert(mid1);
        (*pulse_thread_env)->CallVoidMethod(pulse_thread_env,
                java_context->obj, mid1, name) ;
    } else {
        assert(pulse_thread_env);
        notifyWaitingOperations(pulse_thread_env);
    }

}

void source_list_success_cb(pa_context *context, const pa_source_info *i,
        int eol, void *userdata) {

    if (eol == 0) {
        jclass cls = (*pulse_thread_env)->GetObjectClass(pulse_thread_env,
                java_context->obj);
        assert(cls);
        jstring name = (*pulse_thread_env)->NewStringUTF(pulse_thread_env, i->name);
        assert(name);
        jmethodID mid1 = (*pulse_thread_env)->GetMethodID(pulse_thread_env, cls,
                "source_callback", "(Ljava/lang/String;)V");
        assert(mid1);
        (*pulse_thread_env)->CallVoidMethod(pulse_thread_env,
                java_context->obj, mid1, name) ;
    } else {
        assert(pulse_thread_env);
        notifyWaitingOperations(pulse_thread_env);
    }

}

static void context_change_callback(pa_context* context, void* userdata) {
    assert(context);
    assert(userdata == NULL);

    //java_context_t* java_context = (java_context_t*)userdata;
    JNIEnv* env = java_context->env;
    jobject obj = java_context->obj;

    //    printf("context state changed to %d\n", pa_context_get_state(context));

    /* Call the EventLoop.update method in java
     * to handle all java-side events
     */
    jclass cls = (*env)->GetObjectClass(env, obj);
    assert(cls);
    jmethodID mid = (*env)->GetMethodID(env, cls, "update", "(I)V");
    assert(mid);
    (*env)->CallVoidMethod(env, obj, mid, pa_context_get_state(context));
    return;

}

static int poll_function(struct pollfd *ufds, unsigned long nfds, int timeout,
        void *userdata) {

    JNIEnv* env = pulse_thread_env;
    assert(env);
    jobject lockObject = getLockObject(env);

    (*env)->MonitorExit(env, lockObject);

    int value = poll(ufds, nfds, timeout);

    (*env)->MonitorEnter(env, lockObject);
    return value;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_EventLoop
 * Method:    native_setup
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_EventLoop_native_1setup
(JNIEnv* env, jobject obj, jstring appName, jstring server) {

    assert(appName != NULL);

    //    printf("native_setup() called\n");
    pa_mainloop *mainloop = pa_mainloop_new();
    assert(mainloop != NULL);
    pa_mainloop_api *mainloop_api = pa_mainloop_get_api(mainloop);
    assert(mainloop != NULL);

    pa_context *context = NULL;

    const char* string_appName;
    string_appName = (*env)->GetStringUTFChars(env, appName, NULL);
    if (string_appName == NULL) {
        return; /* a OutOfMemoryError thrown by vm */
    }
    //    printf("using appName : %s\n", string_appName);
    context = pa_context_new(mainloop_api, string_appName);
    assert(mainloop != NULL);
    (*env)->ReleaseStringUTFChars(env, appName, string_appName);

    obj = (*env)->NewGlobalRef(env, obj);

    java_context = malloc(sizeof(java_context_t));
    java_context->env = env;
    pulse_thread_env = env;
    java_context->obj = obj;

    pa_context_set_state_callback(context, context_change_callback, NULL);

    if (server != NULL) {
        /* obtain the server from the caller */
        const char* string_server = NULL;
        string_server = (*env)->GetStringUTFChars(env, server, NULL);
        if (string_server == NULL) {
            /* error, so clean up */
            (*env)->DeleteGlobalRef(env, java_context->obj);
            pa_context_disconnect(context);
            pa_mainloop_free(mainloop);
            free(java_context);
            return; /* OutOfMemoryError */
        }
        //        printf("About to connect to server: %s\n", string_server);
        pa_context_connect(context, string_server, 0, NULL);
        (*env)->ReleaseStringUTFChars(env, appName, string_server);
    } else {
        //        printf("using default server\n");
        pa_context_connect(context, NULL, 0, NULL);
    }

    // set polling function
    pa_mainloop_set_poll_func(mainloop, poll_function, NULL);

    setJavaPointer(env, obj, "mainloopPointer", mainloop);
    setJavaPointer(env, obj, "contextPointer", context);
    //    printf("native_setup() returning\n");
    return;

}
/*
 * Class:     org_classpath_icedtea_pulseaudio_EventLoop
 * Method:    native_iterate
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_EventLoop_native_1iterate
(JNIEnv* env, jobject obj, jint timeout) {

    pa_mainloop* mainloop = (pa_mainloop*) getJavaPointer(env, obj, "mainloopPointer");
    assert(mainloop);

    int returnval;

    returnval = pa_mainloop_prepare(mainloop, timeout);
    if ( returnval < 0) {
        return -1;
    }
    returnval = pa_mainloop_poll(mainloop);
    if ( returnval < 0) {
        return -1;
    }
    returnval = pa_mainloop_dispatch(mainloop);
    if ( returnval < 0) {
        return -1;
    }
    return returnval;

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_EventLoop
 * Method:    nativeUpdateTargetPortNameList
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_EventLoop_nativeUpdateTargetPortNameList
(JNIEnv* env, jobject obj) {

    pa_context* context = (pa_context*) getJavaPointer(env, obj, "contextPointer");
    assert(context);
    pa_operation *o = pa_context_get_sink_info_list(context, sink_list_success_cb, NULL);
    assert(o);
    return convertNativePointerToJava(env, o);
}


/*
 * Class:     org_classpath_icedtea_pulseaudio_EventLoop
 * Method:    nativeUpdateSourcePortNameList
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_EventLoop_nativeUpdateSourcePortNameList
(JNIEnv * env, jobject obj) {
    pa_context* context = (pa_context*) getJavaPointer(env, obj, "contextPointer");
    assert(context);
    pa_operation *o = pa_context_get_source_info_list(context, source_list_success_cb, NULL);
    assert(o);
    return convertNativePointerToJava(env, o);
}

static void context_drain_complete_callback(pa_context* context, void* userdata) {
    pa_context_disconnect(context);

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_EventLoop
 * Method:    native_shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_EventLoop_native_1shutdown
(JNIEnv *env, jobject obj) {

    //    printf("native_shutdown() starting\n");

    pa_mainloop* mainloop = (pa_mainloop*) getJavaPointer(env, obj, "mainloopPointer");
    assert(mainloop != NULL);

    pa_context* context = (pa_context*) getJavaPointer(env, obj, "contextPointer");
    assert(context != NULL);

    pa_operation* o = pa_context_drain(context, context_drain_complete_callback, NULL);
    if ( o == NULL) {
        pa_context_disconnect(context);
        pa_mainloop_free(mainloop);
    } else {
        pa_operation_unref(o);
    }
    
    pa_context_unref(context);
    (*env)->DeleteGlobalRef(env, java_context->obj);

    free(java_context);
    java_context = NULL;

    setJavaPointer(env, obj, "mainloopPointer", NULL);
    setJavaPointer(env, obj, "contextPointer", NULL);

    //    printf("native_shutdown() returning\n");

}

