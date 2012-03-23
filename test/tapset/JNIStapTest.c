#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "jni.h"
#include "JNITestClass.h"

/* Longer than any known good argument. */
#define MAX_ARG_LEN 128
/* Arbitrary length.  Some arrays are assigned with literals, so do not change
 * this without examining the source carefully.
 */
#define TEST_ARRAY_LEN 5

/* Arguments for run_attach_current_thread. */
#define DAEMON 1
#define NOT_DAEMON 2

/* Avoiding repeated inline string constants. */
char* const ALLOC_OBJECT = "AllocObject";
char* const ATTACH_CURRENT_THREAD_AS_DAEMON = "AttachCurrentThreadAsDaemon";
char* const ATTACH_CURRENT_THREAD = "AttachCurrentThread";
char* const CALL = "Call";
char* const CREATE_JAVA_VM = "CreateJavaVM";
char* const DEFINE_CLASS = "DefineClass";
char* const DELETE_GLOBAL_REF = "DeleteGlobalRef";
char* const DELETE_LOCAL_REF = "DeleteLocalRef";
char* const DELETE_WEAK_GLOBAL_REF = "DeleteWeakGlobalRef";
char* const DESTROY_JAVA_VM = "DestroyJavaVM";
char* const DETACH_CURRENT_THREAD = "DetachCurrentThread";
char* const ENSURE_LOCAL_CAPACITY = "EnsureLocalCapacity";
char* const EXCEPTION_CHECK = "ExceptionCheck";
char* const EXCEPTION_CLEAR = "ExceptionClear";
char* const EXCEPTION_DESCRIBE = "ExceptionDescribe";
char* const EXCEPTION_OCCURRED = "ExceptionOccurred";
char* const FATAL_ERROR = "FatalError";
char* const FIND_CLASS = "FindClass";
char* const FROM_REFLECTED_FIELD = "FromReflectedField";
char* const FROM_REFLECTED_METHOD = "FromReflectedMethod";
char* const GET_ARRAY_LENGTH = "GetArrayLength";
char* const GET_CREATED_JAVA_VMS = "GetCreatedJavaVMs";
char* const GET_DEFAULT_JAVA_VM_INIT_ARGS = "GetDefaultJavaVMInitArgs";
char* const GET_DIRECT_BUFFER_ADDRESS = "GetDirectBufferAddress";
char* const GET_DIRECT_BUFFER_CAPACITY = "GetDirectBufferCapacity";
char* const GET = "Get";
char* const GET_ENV = "GetEnv";
char* const GET_FIELD_ID = "GetFieldID";
char* const GET_JAVA_VM = "GetJavaVM";
char* const GET_METHOD_ID = "GetMethodID";
char* const GET_OBJECT_CLASS = "GetObjectClass";
char* const GET_OBJECT_REF_TYPE = "GetObjectRefType";
char* const GET_PRIMITIVE_ARRAY_CRITICAL = "GetPrimitiveArrayCritical";
char* const GET_STATIC_FIELD_ID = "GetStaticFieldID";
char* const GET_STATIC_METHOD_ID = "GetStaticMethodID";
char* const GET_STRING = "GetString";
char* const GET_STRING_UTF = "GetStringUTF";
char* const GET_SUPERCLASS = "GetSuperclass";
char* const GET_VERSION = "GetVersion";
char* const IS_ASSIGNABLE_FROM = "IsAssignableFrom";
char* const IS_INSTANCE_OF = "IsInstanceOf";
char* const IS_SAME_OBJECT = "IsSameObject";
char* const MONITOR = "Monitor";
char* const NEW = "New";
char* const NEW_DIRECT_BYTE_BUFFER = "NewDirectByteBuffer";
char* const NEW_GLOBAL_REF = "NewGlobalRef";
char* const NEW_LOCAL_REF = "NewLocalRef";
char* const NEW_OBJECT_A = "NewObjectA";
char* const NEW_OBJECT = "NewObject";
char* const NEW_OBJECT_V = "NewObjectV";
char* const NEW_STRING = "NewString";
char* const NEW_STRING_UTF = "NewStringUTF";
char* const NEW_WEAK_GLOBAL_REF = "NewWeakGlobalRef";
char* const POP_LOCAL_FRAME = "PopLocalFrame";
char* const PUSH_LOCAL_FRAME = "PushLocalFrame";
char* const REGISTER_NATIVES = "RegisterNatives";
char* const RELEASE = "Release";
char* const RELEASE_PRIMITIVE_ARRAY_CRITICAL = "ReleasePrimitiveArrayCritical";
char* const RELEASE_STRING_CHARS = "ReleaseStringChars";
char* const RELEASE_STRING_CRITICAL = "ReleaseStringCritical";
char* const RELEASE_STRING_UTF_CHARS = "ReleaseStringUTFChars";
char* const SET = "Set";
char* const THROW = "Throw";
char* const THROW_NEW = "ThrowNew";
char* const TO_REFLECTED_FIELD = "ToReflectedField";
char* const TO_REFLECTED_METHOD = "ToReflectedMethod";
char* const UNREGISTER_NATIVES = "UnregisterNatives";

char* const FIELD = "Field";
char* const ARRAY = "Array";
char* const STATIC = "Static";
char* const NONVIRTUAL = "Nonvirtual";
char* const DIRECT = "Direct";
char* const BOOLEAN = "Boolean";
char* const BYTE = "Byte";
char* const CHAR = "Char";
char* const DOUBLE = "Double";
char* const FLOAT = "Float";
char* const INT = "Int";
char* const LONG = "Long";
char* const OBJECT = "Object";
char* const SHORT = "Short";
char* const VOID = "Void";

char* const TEST_CLASS = "staptest/JNITestClass";

/* These functions defined below. */
void bad_usage();
void* run_attach_current_thread(void* style);

int get_type_and_modifier(char* arg, char** type_p, char** mod_p);

jboolean is_field_op(char* arg);
jboolean is_array_op(char* arg);

jint create_java_vm_wrap(JavaVMInitArgs* args);

jobject new_object_v_wrap(JNIEnv* env_, jclass* class_p,
                          jmethodID* construct, ...);

jboolean boolean_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jbyte byte_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jchar char_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jdouble double_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jfloat float_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jint int_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jlong long_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jobject object_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
jshort short_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);
void void_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...);

jboolean nonvirtual_boolean_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jbyte nonvirtual_byte_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jchar nonvirtual_char_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jdouble nonvirtual_double_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jfloat nonvirtual_float_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jint nonvirtual_int_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jlong nonvirtual_long_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jobject nonvirtual_object_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
jshort nonvirtual_short_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);
void nonvirtual_void_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...);

jboolean static_boolean_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jbyte static_byte_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jchar static_char_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jdouble static_double_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jfloat static_float_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jint static_int_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jlong static_long_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jobject static_object_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
jshort static_short_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);
void static_void_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...);

/* We don't want to have to pass these around all the time, so make them global.
 * This would probably be a Very Bad Idea in real JNI code.
 */
JavaVM* jvm;
JNIEnv *env;

int main(int argc, char *argv[]) {

  pthread_t attach_thread;

  if (argc != 2) {
    bad_usage("No argument specified.");
    exit(1);
  }
  int arglen = (int) strlen(argv[1]);
  if (arglen > MAX_ARG_LEN) {
    bad_usage("Argument too long.");
    exit(1);
  }
  char* safe_arg = (char*) malloc(sizeof(char)*(MAX_ARG_LEN + 1));
  strncpy(safe_arg, argv[1], MAX_ARG_LEN);
  /* Copy into a larger array so we can strncmp to our string constants
   * without the risk of running past the end of the string.
   */
/* This is a gigantic if-else if-else block.  Each JNI function (or, where
 * appropriate, groups of related JNI functions) are called independently, as
 * determined by the argument to this program, in order to trigger the
 * corresponding systemtap/dtrace probes.
 */
  if ((strncmp(safe_arg, CREATE_JAVA_VM,
                         strlen(CREATE_JAVA_VM)) == 0) ||
      (strncmp(safe_arg, DESTROY_JAVA_VM,
                         strlen(DESTROY_JAVA_VM)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    /* No further action needed. */
    goto end_vm;
  } else if (strncmp(safe_arg, FATAL_ERROR, strlen(FATAL_ERROR)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    (*env)->FatalError(env, "Intentional Crash: Ignore.");
    exit(-1);
  } else if (strncmp(safe_arg, GET_VERSION, strlen(GET_VERSION)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    (*env)->GetVersion(env);
  } else if (strncmp(safe_arg, GET_ENV, strlen(GET_ENV)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    JNIEnv *tmp_env;
    (*jvm)->GetEnv(jvm, (void**)&tmp_env, JNI_VERSION_1_6);
  } else if (strncmp(safe_arg, GET_DEFAULT_JAVA_VM_INIT_ARGS,
                         strlen(GET_DEFAULT_JAVA_VM_INIT_ARGS)) == 0) {
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_6;
    JNI_GetDefaultJavaVMInitArgs(&vm_args);
    if (safe_arg != NULL) {
      free(safe_arg);
      safe_arg = NULL;
    }
    exit(0);
  } else if (strncmp(safe_arg, GET_JAVA_VM,
                         strlen(GET_JAVA_VM)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    JavaVM* vm_pointer;
    (*env)->GetJavaVM(env, &vm_pointer);
  } else if (strncmp(safe_arg, DEFINE_CLASS, strlen(DEFINE_CLASS)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass loaderClass = (*env)->FindClass(env, "staptest/StapJNIClassLoader");
    if (loaderClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID loadConstructor = (*env)->GetMethodID(env, loaderClass,
                                                "<init>", "()V");
    if (loadConstructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jobject loader = (*env)->NewObject(env, loaderClass, loadConstructor);
    if (loader == NULL) {
      /* Bail out, cannot create loader. */
      goto end_vm;
    }
    int class_fildes = open("./staptest/JNITestClass.class", O_RDONLY|O_NONBLOCK);
    if (class_fildes == -1) {
      /* Bail out, cannot open class file. */
      goto end_vm;
    }
    struct stat classfile_stats;
    int r_val = fstat(class_fildes, &classfile_stats);
    if (r_val != 0) {
      /* Bail out, cannot determine classfile size. */
      goto end_vm;
    }
    ssize_t classfile_size = (ssize_t) classfile_stats.st_size;
    jbyte* classfile_buf = malloc(sizeof(jbyte)*classfile_size);
    ssize_t total_bytes_read = 0;
    /* Ensure entire file is read in case of interrupts. */
    while (total_bytes_read < classfile_size) {
      size_t bytes_needed = (size_t) classfile_size - (size_t) total_bytes_read;
      void* buf_start = (void*) classfile_buf + total_bytes_read;
      ssize_t bytes_read = pread(class_fildes, buf_start,
                                 bytes_needed, (off_t) total_bytes_read);
      if (bytes_read <= 0) {
        /* Bail out, reading from file blocked or unexpected EOF */
        if (classfile_buf != NULL) {
          free(classfile_buf);
          classfile_buf = NULL;
        }
        goto end_vm;
      }
      total_bytes_read += bytes_read;
    }
    jclass testClass = (*env)->DefineClass(env, TEST_CLASS, loader,
                                     classfile_buf, (jsize) classfile_size);
    if (classfile_buf != NULL) {
      free(classfile_buf);
      classfile_buf = NULL;
    }
  } else if ((strncmp(safe_arg, ENSURE_LOCAL_CAPACITY,
                         strlen(ENSURE_LOCAL_CAPACITY)) == 0) ||
      (strncmp(safe_arg, PUSH_LOCAL_FRAME,
                         strlen(PUSH_LOCAL_FRAME)) == 0) ||
      (strncmp(safe_arg, POP_LOCAL_FRAME,
                         strlen(POP_LOCAL_FRAME)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jint rval = (*env)->PushLocalFrame(env, (jint) 10);
    if (rval == JNI_OK) {
      (*env)->EnsureLocalCapacity(env, (jint) 10);
      (*env)->PopLocalFrame(env, NULL);
    }
  } else if ((strncmp(safe_arg, NEW_OBJECT, strlen(NEW_OBJECT)) == 0) &&
                         (strlen(safe_arg) == strlen(NEW_OBJECT))) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                "<init>", "()V");
    if (constructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jobject testInstance = (*env)->NewObject(env, testClass, constructor);
  } else if ((strncmp(safe_arg, NEW_OBJECT_A, strlen(NEW_OBJECT_A)) == 0) &&
                         (strlen(safe_arg) == strlen(NEW_OBJECT_A))) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                "<init>", "()V");
    if (constructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jobject testInstance = (*env)->NewObjectA(env, testClass,
                                              constructor, NULL);
  } else if (strncmp(safe_arg, NEW_OBJECT_V, strlen(NEW_OBJECT_V)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                "<init>", "()V");
    if (constructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jobject testInstance = new_object_v_wrap(env, &testClass, &constructor);
  } else if ((strncmp(safe_arg, GET_ARRAY_LENGTH,
                         strlen(GET_ARRAY_LENGTH)) == 0) ||
      (strncmp(safe_arg, GET_PRIMITIVE_ARRAY_CRITICAL,
                         strlen(GET_PRIMITIVE_ARRAY_CRITICAL)) == 0) ||
      (strncmp(safe_arg, RELEASE_PRIMITIVE_ARRAY_CRITICAL,
                         strlen(RELEASE_PRIMITIVE_ARRAY_CRITICAL)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jbyteArray byte_array = (*env)->NewByteArray(env, (jsize) TEST_ARRAY_LEN);
    jsize byte_array_len = (*env)->GetArrayLength(env, (jarray) byte_array);
    void* primitive_array = (*env)->GetPrimitiveArrayCritical(env,
                                        (jarray) byte_array, NULL);
    if (primitive_array == NULL) {
      /* JVM out of memory, don't try to release. */
      goto end_vm;
    }
    (*env)->ReleasePrimitiveArrayCritical(env, (jarray) byte_array,
                                          primitive_array, JNI_ABORT);
  } else if (strncmp(safe_arg+3, DIRECT, strlen(DIRECT)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    void* some_space = malloc((size_t) MAX_ARG_LEN);
    jobject byte_buffer = (*env)->NewDirectByteBuffer(env, some_space,
                                                      (jlong) MAX_ARG_LEN);
    void* pointer_to_some_space = (*env)->GetDirectBufferAddress(env,
                                                      byte_buffer);
    jlong some_space_capacity = (*env)->GetDirectBufferCapacity(env,
                                                      byte_buffer);
    if (some_space != NULL) {
      free(some_space);
      some_space = NULL;
    }
  } else if ((strncmp(safe_arg, THROW, strlen(THROW)) == 0) ||
      (strncmp(safe_arg, EXCEPTION_CHECK,
                         strlen(EXCEPTION_CHECK)) == 0) ||
      (strncmp(safe_arg, EXCEPTION_CLEAR,
                         strlen(EXCEPTION_CLEAR)) == 0) ||
      (strncmp(safe_arg, EXCEPTION_DESCRIBE,
                         strlen(EXCEPTION_DESCRIBE)) == 0) ||
      (strncmp(safe_arg, EXCEPTION_OCCURRED,
                         strlen(EXCEPTION_OCCURRED)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass exceptionClass = (*env)->FindClass(env, "java/lang/Exception");
    if (exceptionClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID constructor = (*env)->GetMethodID(env, exceptionClass,
                                                "<init>", "()V");
    if (constructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jthrowable anException = (jthrowable) (*env)->NewObject(env,
                                                  exceptionClass, constructor);
    if (anException == NULL) {
      /* Bail out, cannot create exception. */
      goto end_vm;
    }
    jint r_val = (*env)->Throw(env, anException);
    if (r_val != 0) {
      /* Bail out, couldn't throw exception. */
      goto end_vm;
    }
    jboolean exception_thrown = (*env)->ExceptionCheck(env);
    if (exception_thrown == JNI_FALSE) {
      /* Huh? Should be true. */
      goto end_vm;
    }
    (*env)->ExceptionClear(env);
    r_val = (*env)->ThrowNew(env, exceptionClass, "This exception is for testing purposes only.");
    anException = (*env)->ExceptionOccurred(env);
    if (anException == NULL) {
      goto end_vm;
    }
    (*env)->ExceptionClear(env);
    /* We don't actually want to see output, so we clear exception first. */
    (*env)->ExceptionDescribe(env);
  } else if ((strncmp(safe_arg, REGISTER_NATIVES,
                         strlen(REGISTER_NATIVES)) == 0) ||
      (strncmp(safe_arg, UNREGISTER_NATIVES,
                         strlen(UNREGISTER_NATIVES)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    JNINativeMethod jni_method;
    jni_method.name = "doNothing";
    jni_method.signature = "()V";
    jni_method.fnPtr = &Java_JNITestClass_doNothing;
    jint r_val = (*env)->RegisterNatives(env, testClass, &jni_method, (jint) 1);
    if (r_val != 0) {
      goto end_vm;
    }
    r_val = (*env)->UnregisterNatives(env, testClass);
  } else if ((strncmp(safe_arg, ATTACH_CURRENT_THREAD_AS_DAEMON,
                         strlen(ATTACH_CURRENT_THREAD_AS_DAEMON)) == 0) ||
      (strncmp(safe_arg, GET_CREATED_JAVA_VMS,
                         strlen(GET_CREATED_JAVA_VMS)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    int rval;
    int style = (int) DAEMON;
    rval = pthread_create(&attach_thread, NULL, 
                          run_attach_current_thread, &style);
    if (rval == 0) {
      pthread_join(attach_thread, NULL);
    }
  } else if ((strncmp(safe_arg, ATTACH_CURRENT_THREAD,
                         strlen(ATTACH_CURRENT_THREAD)) == 0) ||
      (strncmp(safe_arg, DETACH_CURRENT_THREAD,
                         strlen(DETACH_CURRENT_THREAD)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    int rval;
    int style = (int) NOT_DAEMON;
    rval = pthread_create(&attach_thread, NULL, 
                          run_attach_current_thread, &style);
    if (rval == 0) {
      pthread_join(attach_thread, NULL);
    }
  } else if ((strncmp(safe_arg, ALLOC_OBJECT,
                         strlen(ALLOC_OBJECT)) == 0) ||
/* A bunch of jni functions having to do with classes and references. */
      (strncmp(safe_arg, DELETE_GLOBAL_REF,
                         strlen(DELETE_GLOBAL_REF)) == 0) ||
      (strncmp(safe_arg, DELETE_LOCAL_REF,
                         strlen(DELETE_LOCAL_REF)) == 0) ||
      (strncmp(safe_arg, DELETE_WEAK_GLOBAL_REF,
                         strlen(DELETE_WEAK_GLOBAL_REF)) == 0) ||
      (strncmp(safe_arg, FIND_CLASS,
                         strlen(FIND_CLASS)) == 0) ||
      (strncmp(safe_arg, GET_FIELD_ID,
                         strlen(GET_FIELD_ID)) == 0) ||
      (strncmp(safe_arg, GET_METHOD_ID,
                         strlen(GET_METHOD_ID)) == 0) ||
      (strncmp(safe_arg, GET_OBJECT_CLASS,
                         strlen(GET_OBJECT_CLASS)) == 0) ||
      (strncmp(safe_arg, GET_OBJECT_REF_TYPE,
                         strlen(GET_OBJECT_REF_TYPE)) == 0) ||
      (strncmp(safe_arg, GET_STATIC_FIELD_ID,
                         strlen(GET_STATIC_FIELD_ID)) == 0) ||
      (strncmp(safe_arg, GET_STATIC_METHOD_ID,
                         strlen(GET_STATIC_METHOD_ID)) == 0) ||
      (strncmp(safe_arg, GET_SUPERCLASS,
                         strlen(GET_SUPERCLASS)) == 0) ||
      (strncmp(safe_arg, IS_ASSIGNABLE_FROM,
                         strlen(IS_ASSIGNABLE_FROM)) == 0) ||
      (strncmp(safe_arg, IS_INSTANCE_OF,
                         strlen(IS_INSTANCE_OF)) == 0) ||
      (strncmp(safe_arg, IS_SAME_OBJECT,
                         strlen(IS_SAME_OBJECT)) == 0) ||
      (strncmp(safe_arg, NEW_GLOBAL_REF,
                         strlen(NEW_GLOBAL_REF)) == 0) ||
      (strncmp(safe_arg, NEW_LOCAL_REF,
                         strlen(NEW_LOCAL_REF)) == 0) ||
      (strncmp(safe_arg, NEW_WEAK_GLOBAL_REF,
                         strlen(NEW_WEAK_GLOBAL_REF)) == 0) ||
      (strncmp(safe_arg, TO_REFLECTED_FIELD,
                         strlen(TO_REFLECTED_FIELD)) == 0) ||
      (strncmp(safe_arg, FROM_REFLECTED_FIELD,
                         strlen(FROM_REFLECTED_FIELD)) == 0) ||
      (strncmp(safe_arg, TO_REFLECTED_METHOD,
                         strlen(TO_REFLECTED_METHOD)) == 0) ||
      (strncmp(safe_arg, FROM_REFLECTED_METHOD,
                         strlen(FROM_REFLECTED_METHOD)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jobject testClassInstance = (*env)->AllocObject(env, testClass);
    jclass aClass = (*env)->GetObjectClass(env, testClassInstance);
    jclass superClass = (*env)->GetSuperclass(env, aClass);
    jboolean jboolrval = (*env)->IsAssignableFrom(env, testClass, aClass);
    jboolrval = (*env)->IsInstanceOf(env, testClassInstance, testClass);
    jfieldID myBoolean = (*env)->GetFieldID(env, testClass, "myBoolean", "Z");
    jobject myBooleanRef = (*env)->ToReflectedField(env, testClass, myBoolean,
                                                    (jboolean) JNI_FALSE);
    jfieldID myBoolean2 = (*env)->FromReflectedField(env, myBooleanRef);
    jmethodID getBoolean = (*env)->GetMethodID(env, testClass,
                                                "getBoolean", "()Z");
    jobject getBooleanRef = (*env)->ToReflectedMethod(env, testClass,
                                              getBoolean, (jboolean) JNI_FALSE);
    jmethodID getBoolean2 = (*env)->FromReflectedMethod(env, getBooleanRef);
    jfieldID myStaticBoolean = (*env)->GetStaticFieldID(env, testClass, 
                                                "myStaticBoolean", "Z");
    jmethodID getStaticBoolean = (*env)->GetStaticMethodID(env, testClass,
                                                "getStaticBoolean", "()Z");
    jobject testClassLocal = (*env)->NewLocalRef(env, testClassInstance);
    jobject testClassGlobal = (*env)->NewGlobalRef(env, testClassInstance);
    jobject testClassWeakGlobal = (*env)->NewWeakGlobalRef(env,
                                                       testClassInstance);
    jboolrval = (*env)->IsSameObject(env, testClassGlobal, testClassWeakGlobal);
    jobjectRefType refType = (*env)->GetObjectRefType(env, testClassGlobal);
    (*env)->DeleteGlobalRef(env, testClassGlobal);
    (*env)->DeleteWeakGlobalRef(env, testClassWeakGlobal);
    (*env)->DeleteLocalRef(env, testClassLocal);
  } else if (strncmp(safe_arg, MONITOR, strlen(MONITOR)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto end_vm;
    }
    jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                "<init>", "()V");
    if (constructor == NULL) {
      /* Bail out, cannot find constructor. */
      goto end_vm;
    }
    jobject testInstance = (*env)->NewObject(env, testClass, constructor);
    (*env)->MonitorEnter(env, testInstance);
    (*env)->MonitorExit(env, testInstance);
  } else if ((strncmp(safe_arg, GET_STRING_UTF,
                         strlen(GET_STRING_UTF)) == 0) ||
      (strncmp(safe_arg, NEW_STRING_UTF,
                         strlen(NEW_STRING_UTF)) == 0) ||
      (strncmp(safe_arg, RELEASE_STRING_UTF_CHARS,
                         strlen(RELEASE_STRING_UTF_CHARS)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    char string_chars[6] = {0x57,0x4F,0x52,0x44,0xC0,0x80};
    jstring aString = (*env)->NewStringUTF(env, string_chars);
    jsize str_len =  (*env)->GetStringUTFLength(env, aString);
    char string_buf[2];
    (*env)->GetStringUTFRegion(env, aString, (jsize) 1, (jsize) 2, string_buf);
    const jbyte* str_pointer = (*env)->GetStringUTFChars(env, aString, NULL);
    (*env)->ReleaseStringUTFChars(env, aString, (char*) str_pointer);
  } else if ((strncmp(safe_arg, GET_STRING,
                         strlen(GET_STRING)) == 0) ||
      (strncmp(safe_arg, NEW_STRING,
                         strlen(NEW_STRING)) == 0) ||
      (strncmp(safe_arg, RELEASE_STRING_CHARS,
                         strlen(RELEASE_STRING_CHARS)) == 0) ||
      (strncmp(safe_arg, RELEASE_STRING_CRITICAL,
                         strlen(RELEASE_STRING_CRITICAL)) == 0)) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    /* Long way to say "WORD" in Unicode: */
    jchar string_chars[4] = {(jchar)0x57,(jchar)0x4F,(jchar)0x52,(jchar)0x44};
    jstring aString = (*env)->NewString(env, string_chars, (jsize) 4);
    jsize str_len =  (*env)->GetStringLength(env, aString);
    jchar string_buf[2];
    (*env)->GetStringRegion(env, aString, (jsize) 1, (jsize) 2, string_buf);
    const jchar* str_pointer = (*env)->GetStringChars(env, aString, NULL);
    (*env)->ReleaseStringChars(env, aString, str_pointer);
    str_pointer = (*env)->GetStringCritical(env, aString, NULL);
    (*env)->ReleaseStringCritical(env, aString, str_pointer);
    
  } else if (strncmp(safe_arg, CALL, strlen(CALL)) == 0) {
    jint created = create_java_vm_wrap(NULL);
    if (created != 0) {
      exit(-1);
    }
    char* type = NULL;
    char* modifier = NULL;
    char* method_sig = malloc(MAX_ARG_LEN);
    char* method_name = malloc(MAX_ARG_LEN);
    if ((method_sig == NULL) || (method_name == NULL)) {
      goto cannot_call;
    }
    int rval = get_type_and_modifier(safe_arg+4, &type, &modifier);
    if (rval == -1) {
      goto cannot_call;
    }
    int arg_len = strlen(safe_arg);
    jclass testClass = (*env)->FindClass(env, TEST_CLASS);
    if (testClass == NULL) {
      /* Bail out, cannot find class. */
      goto cannot_call;
    }
    if ((modifier != NULL) && (strncmp(modifier, STATIC, 
                                       strlen(STATIC)) == 0)) {
      snprintf(method_name, MAX_ARG_LEN, "get%s%s", modifier, type);
      if (strncmp(type, BOOLEAN, strlen(type)) == 0) {
        strncpy(method_sig, "()Z", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_boolean_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticBooleanMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticBooleanMethod(env, testClass, method);
        }
      } else if (strncmp(type, BYTE, strlen(type)) == 0) {
        strncpy(method_sig, "()B", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_byte_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticByteMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticByteMethod(env, testClass, method);
        }
      } else if (strncmp(type, CHAR, strlen(type)) == 0) {
        strncpy(method_sig, "()C", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_char_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticCharMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticCharMethod(env, testClass, method);
        }
      } else if (strncmp(type, DOUBLE, strlen(type)) == 0) {
        strncpy(method_sig, "()D", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_double_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticDoubleMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticDoubleMethod(env, testClass, method);
        }
      } else if (strncmp(type, FLOAT, strlen(type)) == 0) {
        strncpy(method_sig, "()F", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_float_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticFloatMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticFloatMethod(env, testClass, method);
        }
      } else if (strncmp(type, INT, strlen(type)) == 0) {
        strncpy(method_sig, "()I", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_int_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticIntMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticIntMethod(env, testClass, method);
        }
      } else if (strncmp(type, LONG, strlen(type)) == 0) {
        strncpy(method_sig, "()J", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_long_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticLongMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticLongMethod(env, testClass, method);
        }
      } else if (strncmp(type, OBJECT, strlen(type)) == 0) {
        strncpy(method_sig, "()Ljava/lang/Object;", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_object_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticObjectMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticObjectMethod(env, testClass, method);
        }
      } else if (strncmp(type, SHORT, strlen(type)) == 0) {
        strncpy(method_sig, "()S", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_short_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticShortMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticShortMethod(env, testClass, method);
        }
      } else if (strncmp(type, VOID, strlen(type)) == 0) {
        strncpy(method_sig, "()V", MAX_ARG_LEN-1);
        jmethodID method = (*env)->GetStaticMethodID(env, testClass,
                                               method_name, method_sig);
        if (method == NULL) {
          /* Bail out, cannot find method. */
          goto cannot_call;
        }
        if (safe_arg[arg_len - 1] == 'V') {
          static_void_v_wrap(&env, &testClass, &method);
        } else if (safe_arg[arg_len - 1] == 'A') {
          (*env)->CallStaticVoidMethodA(env, testClass, method, NULL);
        } else {
          (*env)->CallStaticVoidMethod(env, testClass, method);
        }
      } else {
        /* Type not recognized. */
        goto cannot_call;
      }
    } else {
      jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                  "<init>", "()V");
      if (constructor == NULL) {
        /* Bail out, cannot find constructor. */
        goto cannot_call;
      }
      jobject testInstance = (*env)->NewObject(env, testClass, constructor);
      if (testInstance == NULL) {
        /* Bail out, cannot create object. */
        goto cannot_call;
      }
      snprintf(method_name, MAX_ARG_LEN, "get%s", type);
      if ((modifier != NULL) && (strncmp(modifier, NONVIRTUAL, 
                                       strlen(NONVIRTUAL)) == 0)) {
        if (strncmp(type, BOOLEAN, strlen(type)) == 0) {
          strncpy(method_sig, "()Z", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_boolean_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualBooleanMethodA(env, testInstance, testClass,
                                                 method, NULL);
          } else {
            (*env)->CallNonvirtualBooleanMethod(env, testInstance, testClass,
                                                method);
          }
        } else if (strncmp(type, BYTE, strlen(type)) == 0) {
          strncpy(method_sig, "()B", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_byte_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualByteMethodA(env, testInstance, testClass,
                                              method, NULL);
          } else {
            (*env)->CallNonvirtualByteMethod(env, testInstance, testClass,
                                             method);
          }
        } else if (strncmp(type, CHAR, strlen(type)) == 0) {
          strncpy(method_sig, "()C", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_char_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualCharMethodA(env, testInstance, testClass,
                                              method, NULL);
          } else {
            (*env)->CallNonvirtualCharMethod(env, testInstance, testClass,
                                             method);
          }
        } else if (strncmp(type, DOUBLE, strlen(type)) == 0) {
          strncpy(method_sig, "()D", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_double_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualDoubleMethodA(env, testInstance, testClass,
                                                method, NULL);
          } else {
            (*env)->CallNonvirtualDoubleMethod(env, testInstance, testClass,
                                               method);
          }
        } else if (strncmp(type, FLOAT, strlen(type)) == 0) {
          strncpy(method_sig, "()F", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_float_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualFloatMethodA(env, testInstance, testClass,
                                               method, NULL);
          } else {
            (*env)->CallNonvirtualFloatMethod(env, testInstance, testClass,
                                              method);
          }
        } else if (strncmp(type, INT, strlen(type)) == 0) {
          strncpy(method_sig, "()I", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_int_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualIntMethodA(env, testInstance, testClass,
                                             method, NULL);
          } else {
            (*env)->CallNonvirtualIntMethod(env, testInstance, testClass,
                                            method);
          }
        } else if (strncmp(type, LONG, strlen(type)) == 0) {
          strncpy(method_sig, "()J", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_long_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualLongMethodA(env, testInstance, testClass,
                                              method, NULL);
          } else {
            (*env)->CallNonvirtualLongMethod(env, testInstance, testClass,
                                             method);
          }
        } else if (strncmp(type, OBJECT, strlen(type)) == 0) {
          strncpy(method_sig, "()Ljava/lang/Object;", 21);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_object_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualObjectMethodA(env, testInstance, testClass,
                                                method, NULL);
          } else {
            (*env)->CallNonvirtualObjectMethod(env, testInstance, testClass,
                                               method);
          }
        } else if (strncmp(type, SHORT, strlen(type)) == 0) {
          strncpy(method_sig, "()S", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_short_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualShortMethodA(env, testInstance, testClass,
                                               method, NULL);
          } else {
            (*env)->CallNonvirtualShortMethod(env, testInstance, testClass,
                                              method);
          }
        } else if (strncmp(type, VOID, strlen(type)) == 0) {
          strncpy(method_sig, "()V", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            nonvirtual_void_v_wrap(&env, &testInstance, &testClass, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallNonvirtualVoidMethodA(env, testInstance, testClass,
                                              method, NULL);
          } else {
            (*env)->CallNonvirtualVoidMethod(env, testInstance, testClass,
                                             method);
          }
        } else {
          /* Type not recognized. */
          goto cannot_call;
        }
      } else {
        if (strncmp(type, BOOLEAN, strlen(type)) == 0) {
          strncpy(method_sig, "()Z", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            boolean_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallBooleanMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallBooleanMethod(env, testInstance, method);
          }
        } else if (strncmp(type, BYTE, strlen(type)) == 0) {
          strncpy(method_sig, "()B", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            byte_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallByteMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallByteMethod(env, testInstance, method);
          }
        } else if (strncmp(type, CHAR, strlen(type)) == 0) {
          strncpy(method_sig, "()C", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            char_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallCharMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallCharMethod(env, testInstance, method);
          }
        } else if (strncmp(type, DOUBLE, strlen(type)) == 0) {
          strncpy(method_sig, "()D", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            double_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallDoubleMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallDoubleMethod(env, testInstance, method);
          }
        } else if (strncmp(type, FLOAT, strlen(type)) == 0) {
          strncpy(method_sig, "()F", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            float_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallFloatMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallFloatMethod(env, testInstance, method);
          }
        } else if (strncmp(type, INT, strlen(type)) == 0) {
          strncpy(method_sig, "()I", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            int_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallIntMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallIntMethod(env, testInstance, method);
          }
        } else if (strncmp(type, LONG, strlen(type)) == 0) {
          strncpy(method_sig, "()J", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            long_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallLongMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallLongMethod(env, testInstance, method);
          }
        } else if (strncmp(type, OBJECT, strlen(type)) == 0) {
          strncpy(method_sig, "()Ljava/lang/Object;", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            object_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallObjectMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallObjectMethod(env, testInstance, method);
          }
        } else if (strncmp(type, SHORT, strlen(type)) == 0) {
          strncpy(method_sig, "()S", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            short_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallShortMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallShortMethod(env, testInstance, method);
          }
        } else if (strncmp(type, VOID, strlen(type)) == 0) {
          strncpy(method_sig, "()V", MAX_ARG_LEN-1);
          jmethodID method = (*env)->GetMethodID(env, testClass,
                                               method_name, method_sig);
          if (method == NULL) {
            /* Bail out, cannot find method. */
            goto cannot_call;
          }
          if (safe_arg[arg_len - 1] == 'V') {
            void_v_wrap(&env, &testInstance, &method);
          } else if (safe_arg[arg_len - 1] == 'A') {
            (*env)->CallVoidMethodA(env, testInstance, method, NULL);
          } else {
            (*env)->CallVoidMethod(env, testInstance, method);
          }
        } else {
          /* Type not recognized. */
          goto cannot_call;
        }
      }
    }
cannot_call:
    if (type != NULL) {
      free(type);
      type = NULL;
    }
    if (modifier != NULL) {
      free(modifier);
      modifier = NULL;
    }
    if (method_sig != NULL) {
      free(method_sig);
      method_sig = NULL;
    }
    if (method_name != NULL) {
      free(method_name);
      method_name = NULL;
    }
  } else if ((strncmp(safe_arg, GET, strlen(GET)) == 0) ||
             (strncmp(safe_arg, NEW, strlen(NEW)) == 0) ||
             (strncmp(safe_arg, SET, strlen(SET)) == 0) ||
             (strncmp(safe_arg, RELEASE, strlen(RELEASE)) == 0)) {
    char* type;
    char* modifier;
    char* field_name = malloc(MAX_ARG_LEN);
    char* field_sig = malloc(MAX_ARG_LEN);
    if ((field_name == NULL) || (field_sig == NULL)) {
      goto done_or_error;
    }
    int rval;
    if (strncmp(safe_arg, RELEASE, strlen(RELEASE)) == 0) {
      rval = get_type_and_modifier(safe_arg+7, &type, &modifier);
    } else {
      rval = get_type_and_modifier(safe_arg+3, &type, &modifier);
    }
    if (rval == -1) {
      goto done_or_error;
    }
    jboolean check_static = JNI_FALSE;
    jboolean check_boolean = JNI_FALSE;
    jboolean check_byte = JNI_FALSE;
    jboolean check_char = JNI_FALSE;
    jboolean check_double = JNI_FALSE;
    jboolean check_float = JNI_FALSE;
    jboolean check_int = JNI_FALSE;
    jboolean check_long = JNI_FALSE;
    jboolean check_object = JNI_FALSE;
    jboolean check_short = JNI_FALSE;
    jboolean check_field = is_field_op(safe_arg);
    jboolean check_array = is_array_op(safe_arg);
    if (strncmp(type, BOOLEAN, strlen(type)) == 0) {
      check_boolean = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "Z");
    } else if (strncmp(type, BYTE, strlen(type)) == 0) {
      check_byte = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "B");
    } else if (strncmp(type, CHAR, strlen(type)) == 0) {
      check_char = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "C");
    } else if (strncmp(type, DOUBLE, strlen(type)) == 0) {
      check_double = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "D");
    } else if (strncmp(type, FLOAT, strlen(type)) == 0) {
      check_float = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "F");
    } else if (strncmp(type, INT, strlen(type)) == 0) {
      check_int = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "I");
    } else if (strncmp(type, LONG, strlen(type)) == 0) {
      check_long = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "J");
    } else if (strncmp(type, OBJECT, strlen(type)) == 0) {
      check_object = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "Ljava/lang/String;");
    } else if (strncmp(type, SHORT, strlen(type)) == 0) {
      check_short = JNI_TRUE;
      snprintf(field_sig, MAX_ARG_LEN-1, "S");
    } else {
      goto done_or_error;
    }
    if (modifier == NULL) {
      snprintf(field_name, MAX_ARG_LEN-1, "my%s", type);
    } else {
      if (strncmp(modifier, STATIC, strlen(modifier)) == 0) {
        check_static = JNI_TRUE;
        snprintf(field_name, MAX_ARG_LEN-1, "my%s%s", modifier, type);
      } else {
        goto done_or_error;
      }
    }
    if ((check_static == JNI_FALSE) && 
        (strncmp(safe_arg, GET, strlen(GET)) == 0)) {
      /* Get<PrimitiveType>Field() functions are optimized by default,
       * bypassing the probe locations.
       */
      JavaVMOption options[1];
      options[0].optionString = "-XX:-UseFastJNIAccessors";
      JavaVMInitArgs vm_args;
      vm_args.version = JNI_VERSION_1_6;
      vm_args.nOptions = 1;
      vm_args.options = options;
      jint created = create_java_vm_wrap(&vm_args);
      if (created != 0) {
        exit(-1);
      }
    } else {
      jint created = create_java_vm_wrap(NULL);
      if (created != 0) {
        exit(-1);
      }
    }
    if (check_field == JNI_TRUE) {
    /* Field */
      jclass testClass = (*env)->FindClass(env, TEST_CLASS);
      if (testClass == NULL) {
        /* Bail out, cannot find class. */
        goto done_or_error;
      } 
      jobject testInstance = NULL;
      jfieldID theFieldID = NULL;
      if (check_static == JNI_TRUE) {
        theFieldID = (*env)->GetStaticFieldID(env, testClass, field_name,
                                              field_sig);
      } else {
        jmethodID constructor = (*env)->GetMethodID(env, testClass,
                                                    "<init>", "()V");
        if (constructor == NULL) {
          /* Bail out, cannot find constructor. */
          goto done_or_error;
        }
        testInstance = (*env)->NewObject(env, testClass, constructor);
        if (testInstance == NULL) {
          /* Bail out, cannot create object. */
          goto done_or_error;
        }
        theFieldID = (*env)->GetFieldID(env, testClass, field_name, field_sig);
      }
      if (theFieldID == NULL) {
        /* Bail out, cannot access field. */
        goto done_or_error;
      }
      if (check_boolean == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jboolean aBool = (*env)->GetStaticBooleanField(env, testClass,
                                                   theFieldID);
          } else {
            jboolean aBool = (*env)->GetBooleanField(env, testInstance,
                                                   theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jboolean aBool = JNI_TRUE;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticBooleanField(env, testClass, theFieldID, aBool);
          } else {
            (*env)->SetBooleanField(env, testInstance, theFieldID, aBool);
          }
        }
      } else if (check_byte == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jbyte aByte = (*env)->GetStaticByteField(env, testClass,
                                                     theFieldID);
          } else {
            jbyte aByte = (*env)->GetByteField(env, testInstance, theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jbyte aByte = (jbyte) 2;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticByteField(env, testClass, theFieldID, aByte);
          } else {
            (*env)->SetByteField(env, testInstance, theFieldID, aByte);
          }
        }
      } else if (check_char == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jchar aChar = (*env)->GetStaticCharField(env, testClass,
                                                     theFieldID);
          } else {
            jchar aChar = (*env)->GetCharField(env, testInstance, theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jchar aChar = 'A';
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticCharField(env, testClass, theFieldID, aChar);
          } else {
            (*env)->SetCharField(env, testInstance, theFieldID, aChar);
          }
        }
      } else if (check_double == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jdouble aDouble = (*env)->GetStaticDoubleField(env, testClass,
                                                           theFieldID);
          } else {
            jdouble aDouble = (*env)->GetDoubleField(env, testInstance,
                                                   theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jdouble aDouble = (jdouble) 2.5;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticDoubleField(env, testClass, theFieldID, aDouble);
          } else {
            (*env)->SetDoubleField(env, testInstance, theFieldID, aDouble);
          }
        }
      } else if (check_float == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jfloat aFloat = (*env)->GetStaticFloatField(env, testClass,
                                                theFieldID);
          } else {
            jfloat aFloat = (*env)->GetFloatField(env, testInstance,
                                                theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jfloat aFloat = (jfloat) 3.5;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticFloatField(env, testClass, theFieldID, aFloat);
          } else {
            (*env)->SetFloatField(env, testInstance, theFieldID, aFloat);
          }
        }
      } else if (check_int == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jint aInt = (*env)->GetStaticIntField(env, testClass, theFieldID);
          } else {
            jint aInt = (*env)->GetIntField(env, testInstance, theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jint aInt = (jint) 7;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticIntField(env, testClass, theFieldID, aInt);
          } else {
            (*env)->SetIntField(env, testInstance, theFieldID, aInt);
          }
        }
      } else if (check_long == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jlong aLong = (*env)->GetStaticLongField(env, testClass,
                                                     theFieldID);
          } else {
            jlong aLong = (*env)->GetLongField(env, testInstance, theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jlong aLong = (jlong) 13;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticLongField(env, testClass, theFieldID, aLong);
          } else {
            (*env)->SetLongField(env, testInstance, theFieldID, aLong);
          }
        }
      } else if (check_object == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jobject aObject = (*env)->GetStaticObjectField(env, testClass,
                                                           theFieldID);
          } else {
            jobject aObject = (*env)->GetObjectField(env, testInstance,
                                                   theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jchar string_chars[4] = 
                   { (jchar) 0x57, (jchar) 0x4F, (jchar) 0x52, (jchar) 0x44 };
          jobject aObject = (jobject) (*env)->NewString(env, string_chars, 
                                                        (jsize) 4);
          if (aObject == NULL) {
            goto done_or_error;
          }
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticObjectField(env, testClass, theFieldID, aObject);
          } else {
            (*env)->SetObjectField(env, testInstance, theFieldID, aObject);
          }
        }
      } else if (check_short == JNI_TRUE) {
        if (strncmp(safe_arg, GET, strlen(GET)) == 0) {
          if (check_static == JNI_TRUE) {
            jshort aShort = (*env)->GetStaticShortField(env, testClass,
                                                        theFieldID);
          } else {
            jshort aShort = (*env)->GetShortField(env, testInstance,
                                                  theFieldID);
          }
        } else if (strncmp(safe_arg, SET, strlen(SET)) == 0) {
          jshort aShort = (jshort) 11;
          if (check_static == JNI_TRUE) {
            (*env)->SetStaticShortField(env, testClass, theFieldID, aShort);
          } else {
            (*env)->SetShortField(env, testInstance, theFieldID, aShort);
          }
        }
      }
    } else if (check_array == JNI_TRUE) {
    /* Array */
      if (check_boolean == JNI_TRUE) {
        jbooleanArray jboolean_array = (*env)->NewBooleanArray(env,
                                                              TEST_ARRAY_LEN);
        if (jboolean_array == NULL) {
          goto done_or_error;
        }
        jboolean boolean_array[TEST_ARRAY_LEN] = { JNI_TRUE, JNI_TRUE, JNI_TRUE,
                                              JNI_TRUE, JNI_TRUE };
        (*env)->SetBooleanArrayRegion(env, jboolean_array, 0, TEST_ARRAY_LEN,
                                      boolean_array);
        jboolean* boolean_buf = malloc(sizeof(jboolean) * TEST_ARRAY_LEN);
        if (boolean_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetBooleanArrayRegion(env, jboolean_array, 0, TEST_ARRAY_LEN,
                                      boolean_buf);
        jboolean* jbool_p = (*env)->GetBooleanArrayElements(env,
                                             jboolean_array, NULL);
        if (jbool_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseBooleanArrayElements(env, jboolean_array, jbool_p,
                                            JNI_ABORT);
      } else if (check_byte == JNI_TRUE) {
        jbyteArray jbyte_array = (*env)->NewByteArray(env, TEST_ARRAY_LEN);
        if (jbyte_array == NULL) {
          goto done_or_error;
        }
        jbyte byte_array[TEST_ARRAY_LEN] = { 0, 0, 0, 0, 0 };
        (*env)->SetByteArrayRegion(env, jbyte_array, 0, TEST_ARRAY_LEN,
                                   byte_array);
        jbyte* byte_buf = malloc(sizeof(jbyte) * TEST_ARRAY_LEN);
        if (byte_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetByteArrayRegion(env, jbyte_array, 0, TEST_ARRAY_LEN,
                                   byte_buf);
        jbyte* jbyte_p = (*env)->GetByteArrayElements(env,
                                             jbyte_array, NULL);
        if (jbyte_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseByteArrayElements(env, jbyte_array, jbyte_p, JNI_ABORT);
      } else if (check_char == JNI_TRUE) {
        jcharArray jchar_array = (*env)->NewCharArray(env, TEST_ARRAY_LEN);
        if (jchar_array == NULL) {
          goto done_or_error;
        }
        jchar char_array[TEST_ARRAY_LEN] = { (jchar) 'a', (jchar) 'b',
                                     (jchar) 'c', (jchar) 'd', (jchar) 'e' };
        (*env)->SetCharArrayRegion(env, jchar_array, 0, TEST_ARRAY_LEN,
                                   char_array);
        jchar* char_buf = malloc(sizeof(jchar) * TEST_ARRAY_LEN);
        if (char_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetCharArrayRegion(env, jchar_array, 0, TEST_ARRAY_LEN,
                                   char_buf);
        jchar* jchar_p = (*env)->GetCharArrayElements(env, jchar_array, NULL);
        if (jchar_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseCharArrayElements(env, jchar_array, jchar_p, JNI_ABORT);
      } else if (check_double == JNI_TRUE) {
        jdoubleArray jdouble_array = (*env)->NewDoubleArray(env, TEST_ARRAY_LEN);
        if (jdouble_array == NULL) {
          goto done_or_error;
        }
        jdouble double_array[TEST_ARRAY_LEN] = { (jdouble) 1, (jdouble) 2,
                                    (jdouble) 3, (jdouble) 4, (jdouble) 5 };
        (*env)->SetDoubleArrayRegion(env, jdouble_array, 0, TEST_ARRAY_LEN,
                                      double_array);
        jdouble* double_buf = malloc(sizeof(jdouble) * TEST_ARRAY_LEN);
        if (double_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetDoubleArrayRegion(env, jdouble_array, 0, TEST_ARRAY_LEN,
                                      double_buf);
        jdouble* jdouble_p = (*env)->GetDoubleArrayElements(env,
                                             jdouble_array, NULL);
        if (jdouble_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseDoubleArrayElements(env, jdouble_array, jdouble_p,
                                            JNI_ABORT);
      } else if (check_float == JNI_TRUE) {
        jfloatArray jfloat_array = (*env)->NewFloatArray(env, TEST_ARRAY_LEN);
        if (jfloat_array == NULL) {
          goto done_or_error;
        }
        jfloat float_array[TEST_ARRAY_LEN] = { (jfloat) 2.5, (jfloat) 6.7,
                                (jfloat) 11.2, (jfloat) 4.8, (jfloat) 8.4 };
        (*env)->SetFloatArrayRegion(env, jfloat_array, 0, TEST_ARRAY_LEN,
                                   float_array);
        jfloat* float_buf = malloc(sizeof(jfloat) * TEST_ARRAY_LEN);
        if (float_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetFloatArrayRegion(env, jfloat_array, 0, TEST_ARRAY_LEN,
                                    float_buf);
        jfloat* jfloat_p = (*env)->GetFloatArrayElements(env, jfloat_array,
                                                         NULL);
        if (jfloat_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseFloatArrayElements(env, jfloat_array, jfloat_p,
                                          JNI_ABORT);
      } else if (check_int == JNI_TRUE) {
        jintArray jint_array = (*env)->NewIntArray(env, TEST_ARRAY_LEN);
        if (jint_array == NULL) {
          goto done_or_error;
        }
        jint int_array[TEST_ARRAY_LEN] = { (jint) 1, (jint) 2, (jint) 3,
                                        (jint) 4, (jint) 5 };
        (*env)->SetIntArrayRegion(env, jint_array, 0, TEST_ARRAY_LEN,
                                  int_array);
        jint* int_buf = malloc(sizeof(jint) * TEST_ARRAY_LEN);
        if (int_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetIntArrayRegion(env, jint_array, 0, TEST_ARRAY_LEN, int_buf);
        jint* jint_p = (*env)->GetIntArrayElements(env, jint_array, NULL);
        if (jint_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseIntArrayElements(env, jint_array, jint_p, JNI_ABORT);
      } else if (check_long == JNI_TRUE) {
        jlongArray jlong_array = (*env)->NewLongArray(env, TEST_ARRAY_LEN);
        if (jlong_array == NULL) {
          goto done_or_error;
        }
        jlong long_array[TEST_ARRAY_LEN] = { (jlong) 1, (jlong) 2, (jlong) 3,
                                        (jlong) 4, (jlong) 5 };
        (*env)->SetLongArrayRegion(env, jlong_array, 0, TEST_ARRAY_LEN,
                                   long_array);
        jlong* long_buf = malloc(sizeof(jlong) * TEST_ARRAY_LEN);
        if (long_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetLongArrayRegion(env, jlong_array, 0, TEST_ARRAY_LEN,
                                   long_buf);
        jlong* jlong_p = (*env)->GetLongArrayElements(env, jlong_array, NULL);
        if (jlong_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseLongArrayElements(env, jlong_array, jlong_p, JNI_ABORT);
      } else if (check_object == JNI_TRUE) {
        jchar string_chars[4] = 
                   { (jchar) 0x57, (jchar) 0x4F, (jchar) 0x52, (jchar) 0x44 };
        jstring aString = (*env)->NewString(env, string_chars, (jsize) 4);
        if (aString == NULL) {
          goto done_or_error;
        }
        jclass stringClass = (*env)->GetObjectClass(env, (jobject) aString);
        if (stringClass == NULL) {
          goto done_or_error;
        }
        jobjectArray oArray = (*env)->NewObjectArray(env, 5, stringClass, NULL);
        if (oArray == NULL) {
          goto done_or_error;
        }
        (*env)->SetObjectArrayElement(env, oArray, 1, (jobject) aString);
        if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
          goto done_or_error;
        }
        jobject bString = (*env)->GetObjectArrayElement(env, oArray, 1);
      } else if (check_short == JNI_TRUE) {
        jshortArray jshort_array = (*env)->NewShortArray(env, TEST_ARRAY_LEN);
        if (jshort_array == NULL) {
          goto done_or_error;
        }
        jshort short_array[TEST_ARRAY_LEN] = { (jshort) 1, (jshort) 2,
                                   (jshort) 3, (jshort) 4, (jshort) 5 };
        (*env)->SetShortArrayRegion(env, jshort_array, 0, TEST_ARRAY_LEN,
                                  short_array);
        jshort* short_buf = malloc(sizeof(jshort) * TEST_ARRAY_LEN);
        if (short_buf == NULL) {
          goto done_or_error;
        }
        (*env)->GetShortArrayRegion(env, jshort_array, 0, TEST_ARRAY_LEN,
                                    short_buf);
        jshort* jshort_p = (*env)->GetShortArrayElements(env, jshort_array,
                                                         NULL);
        if (jshort_p == NULL) {
          goto done_or_error;
        }
        (*env)->ReleaseShortArrayElements(env, jshort_array, jshort_p,
                                          JNI_ABORT);
      }
    }
done_or_error:
    if (type != NULL) {
      free(type);
      type = NULL;
    }
    if (modifier != NULL) {
      free(modifier);
      modifier = NULL;
    }
    if (field_name != NULL) {
      free(field_name);
      field_name = NULL;
    }
    if (field_sig != NULL) {
      free(field_sig);
      field_sig = NULL;
    }
  } else {
/*    bad_usage("Probe specified not found.");*/
    exit(-1);
  }

/* Goto labels are ugly, but we use one anyways to provide a way to
 * jump out of a series of JNI operations if an unsafe operation is reached,
 * while still ensuring the JVM gets shut down.
 */
end_vm:
  if (safe_arg != NULL) {
    free(safe_arg);
    safe_arg = NULL;
  }
  (*jvm)->DestroyJavaVM(jvm);
}

void bad_usage(char* error_msg) {
  fprintf(stderr, "Error: %s\nThis program requires a single argument.\nThe argument should specify a probe alias in the hotspot_jni tapset.\nThese probes aliases are of the form \"hotspot.jni.<name>\".\nThe argument should consist of the <name>.\n", error_msg);
}

void* run_attach_current_thread(void* style) {
  jint r_val;
  JavaVM* javavm = NULL;
  jsize returned_count = 0;
  r_val = JNI_GetCreatedJavaVMs(&javavm, (jsize) 1, &returned_count);
  if ((r_val != JNI_OK) || (returned_count < 1)) {
    pthread_exit(NULL);
  }
  struct JavaVMAttachArgs attach_args;
  attach_args.version = (jint) JNI_VERSION_1_6;
  attach_args.name = "Attached Thread";
  attach_args.group = (jobject) NULL;
  JNIEnv *attach_env;
  if (*(int*)style == (int) DAEMON) {
    r_val = (*javavm)->AttachCurrentThreadAsDaemon(javavm,
                               (void **) &attach_env, (void *) &attach_args);
  } else if (*(int*)style == (int) NOT_DAEMON) {
    r_val = (*javavm)->AttachCurrentThread(javavm,
                               (void **) &attach_env, (void *) &attach_args);
  } else {
    pthread_exit(NULL);
  }
  if (r_val != JNI_OK) {
    pthread_exit(NULL);
  }
  (*javavm)->DetachCurrentThread(javavm);
  pthread_exit(NULL);
}

int get_type_and_modifier(char* arg, char** type_p, char** mod_p) {
  char* type = arg;
  if (strncmp(type, STATIC, strlen(STATIC)) == 0) {
    *mod_p = malloc(strlen(STATIC) + 1);
    if (*mod_p == NULL) {
      *type_p = NULL;
      return -1;
    }
    strncpy(*mod_p, type, strlen(STATIC));
    type += strlen(STATIC);
  } else if (strncmp(type, NONVIRTUAL, strlen(NONVIRTUAL)) == 0) {
    *mod_p = malloc(strlen(NONVIRTUAL) + 1);
    if (*mod_p == NULL) {
      *type_p = NULL;
      return -1;
    }
    strncpy(*mod_p, type, strlen(NONVIRTUAL));
    type += strlen(NONVIRTUAL);
  } else {
    *mod_p = NULL;
  }
  char* char_after_type = strchr(type, (int) 'M');
  /* 'M' targets "Method" in argument. */
  if (char_after_type == NULL) {
    char_after_type = strchr(type, (int) 'A');
    /* 'A' targets "Array<Elements|Region>" in argument. */
    if (char_after_type == NULL) {
      /* 'F' targets "Field" in argument. */
      char_after_type = strrchr(type, (int) 'F');
    }
  }
  int type_len = char_after_type - type;
  if (type_len <= 0) {
    /* char_after_type doesn't point at 'M', 'A', or 'F', or the special case
     * where it points at the 'F' in "Float". */
    *type_p = NULL;
    return -1;
  }
  *type_p = malloc(type_len + 1);
  if (*type_p == NULL) {
    return -1;
  }
  strncpy(*type_p, type, (size_t) type_len);
  return 0;
}

jboolean is_field_op(char* arg) {
  char* poss_field = strrchr(arg, (int) 'F');
  if (poss_field != NULL) {
    if (strncmp(poss_field, FIELD, strlen(FIELD)) == 0) {
      return JNI_TRUE;
    }
  }
  return JNI_FALSE;
}

jboolean is_array_op(char* arg) {
  char* poss_array = strrchr(arg, (int) 'A');
  if (poss_array != NULL) {
    if (strncmp(poss_array, ARRAY, strlen(ARRAY)) == 0) {
      return JNI_TRUE;
    }
  }
  return JNI_FALSE;
}

jint create_java_vm_wrap(JavaVMInitArgs* args) {
  JavaVMInitArgs def_vm_args;
  JavaVMInitArgs* vm_args_p;
  if (args == NULL) {
    def_vm_args.version = JNI_VERSION_1_6;
    def_vm_args.nOptions = 0;
    def_vm_args.options = NULL;
    vm_args_p = &def_vm_args;
  } else {
    vm_args_p = args;
  }

  return JNI_CreateJavaVM(&jvm, (void **) &env, vm_args_p);
}

jobject new_object_v_wrap(JNIEnv* env_, jclass* class_p,
                          jmethodID* construct, ...) {
  va_list var_args;
  va_start(var_args, construct);
  jobject instance = (*env_)->NewObjectV(env_, *class_p, *construct, var_args);
  va_end(var_args);
  return instance;
}

jboolean boolean_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jboolean to_return = (**env_p)->CallBooleanMethodV(*env_p, *obj_p,
                                                     *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jbyte byte_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jbyte to_return = (**env_p)->CallByteMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jchar char_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jchar to_return = (**env_p)->CallCharMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jdouble double_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jdouble to_return = (**env_p)->CallDoubleMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jfloat float_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jfloat to_return = (**env_p)->CallFloatMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jint int_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jint to_return = (**env_p)->CallIntMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jlong long_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jlong to_return = (**env_p)->CallLongMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jobject object_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jobject to_return = (**env_p)->CallObjectMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jshort short_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jshort to_return = (**env_p)->CallShortMethodV(*env_p, *obj_p,
                                               *method_p, var_args);
  va_end(var_args);
  return to_return;
}
void void_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  (**env_p)->CallVoidMethodV(*env_p, *obj_p, *method_p, var_args);
  va_end(var_args);
  return;
}

jboolean nonvirtual_boolean_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jboolean to_return = (**env_p)->CallNonvirtualBooleanMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jbyte nonvirtual_byte_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jbyte to_return = (**env_p)->CallNonvirtualByteMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jchar nonvirtual_char_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jchar to_return = (**env_p)->CallNonvirtualCharMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jdouble nonvirtual_double_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jdouble to_return = (**env_p)->CallNonvirtualDoubleMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jfloat nonvirtual_float_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jfloat to_return = (**env_p)->CallNonvirtualFloatMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jint nonvirtual_int_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jint to_return = (**env_p)->CallNonvirtualIntMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jlong nonvirtual_long_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jlong to_return = (**env_p)->CallNonvirtualLongMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jobject nonvirtual_object_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jobject to_return = (**env_p)->CallNonvirtualObjectMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jshort nonvirtual_short_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jshort to_return = (**env_p)->CallNonvirtualShortMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return to_return;
}
void nonvirtual_void_v_wrap(JNIEnv** env_p, jobject* obj_p,
                     jclass* jclass_p, jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  (**env_p)->CallNonvirtualVoidMethodV(*env_p, *obj_p,
                                    *jclass_p, *method_p, var_args);
  va_end(var_args);
  return;
}

jboolean static_boolean_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jboolean to_return = (**env_p)->CallStaticBooleanMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jbyte static_byte_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jbyte to_return = (**env_p)->CallStaticByteMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jchar static_char_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jchar to_return = (**env_p)->CallStaticCharMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jdouble static_double_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jdouble to_return = (**env_p)->CallStaticDoubleMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jfloat static_float_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jfloat to_return = (**env_p)->CallStaticFloatMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jint static_int_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jint to_return = (**env_p)->CallStaticIntMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jlong static_long_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jlong to_return = (**env_p)->CallStaticLongMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jobject static_object_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jobject to_return = (**env_p)->CallStaticObjectMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
jshort static_short_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  jshort to_return = (**env_p)->CallStaticShortMethodV(*env_p, *jclass_p,
                                    *method_p, var_args);
  va_end(var_args);
  return to_return;
}
void static_void_v_wrap(JNIEnv** env_p, jclass* jclass_p,
                     jmethodID* method_p, ...) {
  va_list var_args;
  va_start(var_args, method_p);
  (**env_p)->CallStaticVoidMethodV(*env_p, *jclass_p, *method_p, var_args);
  va_end(var_args);
  return;
}

