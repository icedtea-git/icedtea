#include "jni.h"
#include "JNITestClass.h"

JNIEXPORT void JNICALL Java_JNITestClass_doNothing
            (JNIEnv * env, jobject this) {
  return;
}
