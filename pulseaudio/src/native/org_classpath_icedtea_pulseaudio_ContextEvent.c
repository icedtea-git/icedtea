/* org_classpath_icedtea_pulseaudio_EventLoop.c
 Copyright (C) 2011 Red Hat, Inc.

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

#include "org_classpath_icedtea_pulseaudio_ContextEvent.h"
#include "jni-common.h"

// we don't prefix the java names with anything, so we leave the third argument
// empty
#define SET_CONTEXT_ENUM(env, clz, name) \
    SET_JAVA_STATIC_LONG_FIELD_TO_PA_ENUM(env, clz, , CONTEXT, name)

/*
 * Class:     org_classpath_icedtea_pulseaudio_ContextEvent
 * Method:    init_constants
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_ContextEvent_init_1constants
  (JNIEnv *env, jclass clz) {
    SET_CONTEXT_ENUM(env, clz, UNCONNECTED);
    SET_CONTEXT_ENUM(env, clz, CONNECTING);
    SET_CONTEXT_ENUM(env, clz, AUTHORIZING);
    SET_CONTEXT_ENUM(env, clz, SETTING_NAME);
    SET_CONTEXT_ENUM(env, clz, READY);
    SET_CONTEXT_ENUM(env, clz, FAILED);
    SET_CONTEXT_ENUM(env, clz, TERMINATED);
}

