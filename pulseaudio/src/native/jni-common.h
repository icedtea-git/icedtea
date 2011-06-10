/* jni-common.h
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

#ifndef _JNI_COMMON_H
#define _JNI_COMMON_H

#include <jni.h>
/*
 * This file contains some commonly used functions 
 * 
 */

typedef struct java_context_t {
    JNIEnv* env;
    jobject obj;
} java_context_t;

/* Exception Handling */

void throwByName(JNIEnv* const env, const char* const name,
        const char* const msg);

#define ILLEGAL_ARGUMENT_EXCEPTION "java/lang/IllegalArgumentException"
#define ILLEGAL_STATE_EXCEPTION "java/lang/IllegalStateException"

/* Threading and Synchronization */

jobject getLockObject(JNIEnv* env);
void notifyWaitingOperations(JNIEnv* env);

/* Storing and Loading Values */

jint getJavaIntField(JNIEnv* env, jobject obj, char* fieldName);
void setJavaIntField(JNIEnv* env, jobject obj, char* fieldName, jint value);

jlong getJavaLongField(JNIEnv* env, jobject obj, char* name);
void setJavaLongField(JNIEnv* env, jobject, char* name, jlong value);

jbyteArray getJavaByteArrayField(JNIEnv* env, jobject obj, char* name);
void setJavaByteArrayField(JNIEnv* env, jobject obj, char* name,
        jbyteArray array);

/* Pointers and Java */

void* getJavaPointer(JNIEnv* env, jobject obj, char* name);
void setJavaPointer(JNIEnv* env, jobject obj, char*name, void* pointer_value);

void* convertJavaPointerToNative(JNIEnv* env, jbyteArray pointer);
jbyteArray convertNativePointerToJava(JNIEnv* env, void* pointer);

/* Calling Java Functions */

void callJavaVoidMethod(JNIEnv* env, jobject obj, const char* method_name);

#endif

