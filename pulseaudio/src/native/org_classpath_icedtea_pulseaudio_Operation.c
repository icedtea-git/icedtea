/* org_classpath_icedtea_pulseaudio_Operation.c
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

#include "org_classpath_icedtea_pulseaudio_Operation.h"

#include "jni-common.h"
#include <pulse/pulseaudio.h>

// we don't prefix the java names with anything, so we leave the third argument
// empty
#define SET_OP_ENUM(env, clz, name) \
    SET_JAVA_STATIC_LONG_FIELD_TO_PA_ENUM(env, clz, , OPERATION, name)

/*
 * Class:     org_classpath_icedtea_pulseaudio_Operation
 * Method:    init_constants
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Operation_init_1constants
  (JNIEnv *env, jclass clz) {
    SET_OP_ENUM(env, clz, RUNNING);
    SET_OP_ENUM(env, clz, DONE);
    SET_OP_ENUM(env, clz, CANCELLED);
}


/*
 * Class:     org_classpath_icedtea_pulseaudio_Operation
 * Method:    native_ref
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Operation_native_1ref
(JNIEnv* env, jobject obj) {

    pa_operation* operation = (pa_operation*) getJavaPointer(env, obj, "operationPointer");
    assert(operation);
    pa_operation_ref(operation);

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Operation
 * Method:    native_unref
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Operation_native_1unref
(JNIEnv* env, jobject obj) {

    pa_operation* operation = (pa_operation*) getJavaPointer(env, obj, "operationPointer");
    assert(operation);
    pa_operation_unref(operation);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Operation
 * Method:    native_get_state
 * Signature: ()I
 */
JNIEXPORT jlong JNICALL Java_org_classpath_icedtea_pulseaudio_Operation_native_1get_1state
(JNIEnv *env, jobject obj) {

    pa_operation* operation = (pa_operation*) getJavaPointer(env, obj, "operationPointer");
    assert(operation);
    jlong state = pa_operation_get_state(operation);
    return state;
}
