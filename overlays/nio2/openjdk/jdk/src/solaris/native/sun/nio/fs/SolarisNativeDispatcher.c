/*
 * Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

#include "jni.h"
#include "jni_util.h"
#include "jvm.h"
#include "jlong.h"

#include <dlfcn.h>
#include <errno.h>

#include "sun_nio_fs_SolarisNativeDispatcher.h"

typedef int facl_func(int filedes, int cmd, int cnt, void *buf);

static facl_func* my_facl_func = NULL;

static void throwUnixException(JNIEnv* env, int errnum) {
    jobject x = JNU_NewObjectByName(env, "sun/nio/fs/UnixException",
        "(I)V", errnum);
    if (x != NULL) {
        (*env)->Throw(env, x);
    }
}

JNIEXPORT void JNICALL
Java_sun_nio_fs_SolarisNativeDispatcher_init(JNIEnv *env, jclass clazz) {
    my_facl_func = (facl_func*) dlsym(RTLD_DEFAULT, "facl");
}

JNIEXPORT jint JNICALL
Java_sun_nio_fs_SolarisNativeDispatcher_facl(JNIEnv* env, jclass this, jint fd,
    jint cmd, jint nentries, jlong address)
{
    void* aclbufp = jlong_to_ptr(address);
    int n = -1;

    if (my_facl_func == NULL) {
        JNU_ThrowInternalError(env, "should not reach here");
        return -1;
    }

    n = (*my_facl_func)((int)fd, (int)cmd, (int)nentries, aclbufp);
    if (n == -1) {
        throwUnixException(env, errno);
    }
    return (jint)n;
}
