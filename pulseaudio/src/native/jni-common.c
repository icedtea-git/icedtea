/* jni-common.c
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

#include "jni-common.h"

#include <assert.h>
#include <string.h>

/*
 * Throw an exception by name
 */
void throwByName(JNIEnv* env, const char* name, const char* msg) {
	jclass cls = (*env)->FindClass(env, name);
	if (cls != NULL) {
		(*env)->ThrowNew(env, cls, msg);
		return;
	}
}

jint getJavaIntField(JNIEnv* env, jobject obj, char* fieldName) {
	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid = (*env)->GetFieldID(env, cls, fieldName, "I");
	assert(fid);
	jint value = (*env)->GetIntField(env, obj, fid);
	return value;
}

void setJavaIntField(JNIEnv *env, jobject obj, char *fieldName, jint value) {
	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid =(*env)->GetFieldID(env, cls, fieldName, "I");
	assert(fid);
	(*env)->SetIntField(env, obj, fid, value);
}

jlong getJavaLongField(JNIEnv* env, jobject obj, char* name) {
	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid = (*env)->GetFieldID(env, cls, name, "J");
	assert(fid);
	jint value = (*env)->GetLongField(env, obj, fid);
	return value;

}

void setJavaLongField(JNIEnv* env, jobject obj, char* name, jlong value) {
	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid =(*env)->GetFieldID(env, cls, name, "J");
	assert(fid);
	(*env)->SetLongField(env, obj, fid, value);
}

jbyteArray getJavaByteArrayField(JNIEnv* env, jobject obj, char* name) {

	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid = (*env)->GetFieldID(env, cls, name, "[B");
	assert(fid);
	jbyteArray array = (*env)->GetObjectField(env, obj, fid);
	assert(array);
	return array;

}

void setJavaByteArrayField(JNIEnv* env, jobject obj, char* name,
		jbyteArray array) {

	jclass cls = (*env)->GetObjectClass(env, obj);
	assert(cls);
	jfieldID fid = (*env)->GetFieldID(env, cls, name, "[B");
	assert(fid);

	(*env)->SetObjectField(env, obj, fid, array);
	return;
}

void callJavaVoidMethod(JNIEnv* env, jobject obj, const char* method_name) {

	jclass cls = (*env)->GetObjectClass(env, obj);
	if (cls == NULL) {
		printf("unable to get class of object");
		return;
	}
	jmethodID mid = (*env)->GetMethodID(env, cls, method_name, "()V");
	if (mid == NULL) {
		printf("unable to get method %s\n", method_name);
		return;

	}
	(*env)->CallVoidMethod(env, obj, mid);

	return;

}

jobject getLockObject(JNIEnv* env) {

	const char* eventLoopClassName =
			"org/classpath/icedtea/pulseaudio/EventLoop";

	jclass eventLoopClass = (*env)->FindClass(env, eventLoopClassName);
	assert(eventLoopClass);

	const char* getEventLoopIDSignature =
			"()Lorg/classpath/icedtea/pulseaudio/EventLoop;";
	jmethodID getEventLoopID = (*env)->GetStaticMethodID(env, eventLoopClass, "getEventLoop",
			getEventLoopIDSignature);
	assert(getEventLoopID);

	jobject eventLoop = (*env)->CallStaticObjectMethod(env, eventLoopClass, getEventLoopID);
	assert(eventLoop);

	jfieldID lockID = (*env)->GetFieldID(env, eventLoopClass, "threadLock",
			"Ljava/lang/Object;");
	assert(lockID);

	jobject lockObject = (*env)->GetObjectField(env, eventLoop, lockID);
	assert(lockObject);
	return lockObject;

}

void notifyWaitingOperations(JNIEnv* env) {
	jobject lockObject = getLockObject(env);

	(*env)->MonitorEnter(env, lockObject);

	jclass objectClass = (*env)->FindClass(env, "java/lang/Object");
	assert(objectClass);
	jmethodID notifyAllID = (*env)->GetMethodID(env, objectClass, "notifyAll", "()V");
	assert(notifyAllID);

	(*env)->CallObjectMethod(env, lockObject, notifyAllID);

	(*env)->MonitorExit(env, lockObject);

}

void* getJavaPointer(JNIEnv* env, jobject obj, char* name) {

	jbyteArray array = getJavaByteArrayField(env, obj, name);
	assert(array);
	void* value = convertJavaPointerToNative(env, array);
	// allow returning NULL values
	return value;
}

void setJavaPointer(JNIEnv* env, jobject obj, char* name, void* value) {

	// allow NULL for value
	jbyteArray array = convertNativePointerToJava(env, value);
	assert(array);
	setJavaByteArrayField(env, obj, name, array);
	return;
}

void* convertJavaPointerToNative(JNIEnv* env, jbyteArray pointer) {
	//	printf("convertJavaPointerToNative(): entering method\n");

	void* returnPointer = NULL;

	// this is not the pointer, but the container of the pointer
	assert(pointer);

	jsize len = (*env)->GetArrayLength(env, pointer);
	assert(len);
	assert(len == sizeof(returnPointer));

	jbyte* data = (*env)->GetByteArrayElements(env, pointer, NULL);
	if (data == NULL) {
		return NULL; // oome;
	}
	memcpy(&returnPointer, data, sizeof(returnPointer));
	(*env)->ReleaseByteArrayElements(env, pointer, data, 0);

	//	printf("convertJavaPointerToNative(): leaving method\n");
	return returnPointer;
}

jbyteArray convertNativePointerToJava(JNIEnv* env, void* pointer) {
	//	printf("convertNativePointerToJava(): entering method\n");

	jbyteArray array = (*env)->NewByteArray(env, sizeof(pointer));
	if (array == NULL) {
		return 0; // oome?
	}

	jbyte* data = (*env)->GetByteArrayElements(env, array, NULL);
	if (data == NULL) {
		return 0; // oome
	}

	memcpy(data, &pointer, sizeof(pointer));
	(*env)->ReleaseByteArrayElements(env, array, data, 0);

	//	printf("convertNativePointerToJava(): leaving method\n");

	return array;
}

