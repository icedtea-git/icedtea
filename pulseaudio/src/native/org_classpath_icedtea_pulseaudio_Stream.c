#include "org_classpath_icedtea_pulseaudio_Stream.h"

#include "jni-common.h"
#include <pulse/pulseaudio.h>
#include <string.h>

#define STREAM_POINTER "streamPointer"
#define CONTEXT_POINTER "contextPointer"

typedef struct java_context {
    JNIEnv* env;
    jobject obj;
} java_context;

extern JNIEnv* pulse_thread_env;

static void set_sink_input_volume_callback(pa_context* context, int success,
        void* userdata) {
    notifyWaitingOperations(pulse_thread_env);

}

const char* getStringFromFormat(pa_sample_format_t format) {

    const char* value;

    if (format == PA_SAMPLE_U8) {
        value = "PA_SAMPLE_U8";
    } else if (format == PA_SAMPLE_ALAW) {
        value = "PA_SAMPLE_ALAW";
    } else if (format == PA_SAMPLE_ULAW) {
        value = "PA_SAMPLE_ULAW";
    } else if (format == PA_SAMPLE_S16BE) {
        value = "PA_SAMPLE_S16BE";
    } else if (format == PA_SAMPLE_S16LE) {
        value = "PA_SAMPLE_S16LE";
    } else if (format == PA_SAMPLE_S32BE) {
        value = "PA_SAMPLE_S32BE";
    } else if (format == PA_SAMPLE_S32LE) {
        value = "PA_SAMPLE_S32LE";
    } else {
        value = "PA_SAMPLE_INVALID";
    }

    return value;
}

pa_sample_format_t getFormatFromString(const char* encoding) {

    pa_sample_format_t format;

    if (strcmp(encoding, "PA_SAMPLE_U8") == 0) {
        format = PA_SAMPLE_U8;
    } else if (strcmp(encoding, "PA_SAMPLE_ALAW") == 0) {
        format = PA_SAMPLE_ALAW;
    } else if (strcmp(encoding, "PA_SAMPLE_ULAW;") == 0) {
        format = PA_SAMPLE_ULAW;
    } else if (strcmp(encoding, "PA_SAMPLE_S16BE") == 0) {
        format = PA_SAMPLE_S16BE;
    } else if (strcmp(encoding, "PA_SAMPLE_S16LE") == 0) {
        format = PA_SAMPLE_S16LE;
    } else if (strcmp(encoding, "PA_SAMPLE_S32BE") == 0) {
        format = PA_SAMPLE_S32BE;
    } else if (strcmp(encoding, "PA_SAMPLE_S32LE") == 0) {
        format = PA_SAMPLE_S32LE;
    } else {
        format = PA_SAMPLE_INVALID;
    }

    return format;
}

static void stream_state_callback(pa_stream* stream, void *userdata) {
    //printf("stream_state_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "stateCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "stateCallback");
    }

}

static void stream_write_callback(pa_stream *stream, size_t length,
        void *userdata) {
    //    printf("stream_write_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "writeCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "writeCallback");
    }
}

static void stream_read_callback(pa_stream *stream, size_t length,
        void *userdata) {
    //    printf("stream_read_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "readCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "readCallback");
    }

}

static void stream_overflow_callback(pa_stream *stream, void *userdata) {
    //printf("stream_overflow_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "overflowCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "overflowCallback");
    }
}

static void stream_underflow_callback(pa_stream *stream, void *userdata) {
    //    printf("stream_underflow_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "underflowCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "underflowCallback");
    }
}


static void update_timing_info_callback(pa_stream* stream, int success, void* userdata) {

    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);

    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "drain failed");
    }

}

// requires pulseaudio 0.9.11 :(
static void stream_started_callback(pa_stream *stream, void *userdata) {
    // printf("stream_started_callback called\n");
    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj,
                "playbackStartedCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj,
                "playbackStartedCallback");
    }

}

static void stream_latency_update_callback(pa_stream *stream, void *userdata) {
    //    printf("stream_latency_update_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "latencyUpdateCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj,
                "latencyUpdateCallback");
    }
}

static void stream_moved_callback(pa_stream *stream, void *userdata) {
    //    printf("stream_moved_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "movedCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "movedCallback");
    }

}

static void stream_suspended_callback(pa_stream *stream, void *userdata) {
    //    printf("stream_suspended_callback called\n");

    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "suspendedCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "suspendedCallback");
    }

}

static void buf_attr_changed_callback(pa_stream *stream, void *userdata) {
    java_context* context = userdata;
    assert(stream);
    assert(context);
    assert(context->env);
    assert(context->obj);

    if (pa_stream_get_state(stream) == PA_STREAM_CREATING) {
        callJavaVoidMethod(context->env, context->obj, "bufferAttrCallback");
    } else {
        callJavaVoidMethod(pulse_thread_env, context->obj, "bufferAttrCallback");
    }
}

// used to set stream flags and states.
// The names in Stream.java have a {STATE_,FLAG_} prefix, but we don't want to
// add the underscore in every line in init_constants, so we add it here. If
// constants with no prefix are ever introduced (i.e. java_prefix is "",
// it's important to remove the ##_ )
#define SET_STREAM_ENUM(env, clz, java_prefix, state_name) \
    SET_JAVA_STATIC_LONG_FIELD_TO_PA_ENUM(env, clz, java_prefix##_, STREAM, state_name)

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    init_constants
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_init_1constants
  (JNIEnv *env, jclass clz) {
    // set states.
    SET_STREAM_ENUM(env, clz, STATE, UNCONNECTED);
    SET_STREAM_ENUM(env, clz, STATE, CREATING);
    SET_STREAM_ENUM(env, clz, STATE, READY);
    SET_STREAM_ENUM(env, clz, STATE, FAILED);
    SET_STREAM_ENUM(env, clz, STATE, TERMINATED);

    // set flags.
    SET_STREAM_ENUM(env, clz, FLAG, NOFLAGS);
    SET_STREAM_ENUM(env, clz, FLAG, START_CORKED);
    SET_STREAM_ENUM(env, clz, FLAG, INTERPOLATE_TIMING);
    SET_STREAM_ENUM(env, clz, FLAG, NOT_MONOTONIC);
    SET_STREAM_ENUM(env, clz, FLAG, AUTO_TIMING_UPDATE);
    SET_STREAM_ENUM(env, clz, FLAG, NO_REMAP_CHANNELS);
    SET_STREAM_ENUM(env, clz, FLAG, NO_REMIX_CHANNELS);
    SET_STREAM_ENUM(env, clz, FLAG, FIX_FORMAT);
    SET_STREAM_ENUM(env, clz, FLAG, FIX_RATE);
    SET_STREAM_ENUM(env, clz, FLAG, FIX_CHANNELS);
    SET_STREAM_ENUM(env, clz, FLAG, DONT_MOVE);
    SET_STREAM_ENUM(env, clz, FLAG, VARIABLE_RATE);
    SET_STREAM_ENUM(env, clz, FLAG, PEAK_DETECT);
    SET_STREAM_ENUM(env, clz, FLAG, START_MUTED);
    SET_STREAM_ENUM(env, clz, FLAG, ADJUST_LATENCY);
    SET_STREAM_ENUM(env, clz, FLAG, EARLY_REQUESTS);
    SET_STREAM_ENUM(env, clz, FLAG, DONT_INHIBIT_AUTO_SUSPEND);
    SET_STREAM_ENUM(env, clz, FLAG, START_UNMUTED);
    SET_STREAM_ENUM(env, clz, FLAG, FAIL_ON_SUSPEND);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_new
 * Signature: ([BLjava/lang/String;Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1new
(JNIEnv* env, jobject obj, jbyteArray contextPointer, jstring nameString,
        jstring encodingString, jint sampleRate, jint channels) {

    //    printf("creating a new PulseAudio stream\n");

    java_context* j_context = malloc(sizeof(java_context));
    assert(j_context);
    j_context->env = env;
    j_context->obj = (*env)->NewGlobalRef(env, obj);

    setJavaPointer(env, obj, CONTEXT_POINTER, j_context);

    pa_context* context = convertJavaPointerToNative(env, contextPointer);
    assert(context);

    const char* name = NULL;
    if (nameString) {
        name = (*env)->GetStringUTFChars(env,nameString, NULL);
        if (name == NULL) {
            (*env)->DeleteGlobalRef(env, obj);
            free(j_context);
            return; // oome thrown
        }
    }

    const char *encoding = (*env)->GetStringUTFChars(env, encodingString, NULL);
    if( encoding == NULL) {
        return; //oome thrown
    }

    pa_sample_spec sample_spec;

    sample_spec.format = getFormatFromString(encoding);
    sample_spec.rate = sampleRate;
    sample_spec.channels = channels;

    if ( !pa_sample_spec_valid(&sample_spec)) {
        throwByName(env, "java/lang/IllegalArgumentException", "Invalid format");
        (*env)->ReleaseStringUTFChars(env, encodingString, encoding);
        if (name) {
            (*env)->ReleaseStringUTFChars(env, nameString,name);
        }
        return;
    }

    pa_stream* stream = pa_stream_new(context, name, &sample_spec, NULL);
    assert(stream);
    if (name) {
        (*env)->ReleaseStringUTFChars(env, nameString,name);
    }

    setJavaPointer(env, obj, STREAM_POINTER, stream);

    /*
     *
     * The stream has been created; now setup the callbacks
     * so we can do somethig about them
     *
     */

    pa_stream_set_state_callback (stream, stream_state_callback, j_context);
    pa_stream_set_write_callback (stream, stream_write_callback, j_context);
    pa_stream_set_read_callback (stream, stream_read_callback, j_context);
    pa_stream_set_overflow_callback (stream, stream_overflow_callback, j_context);
    pa_stream_set_underflow_callback (stream, stream_underflow_callback, j_context);
    pa_stream_set_started_callback (stream, stream_started_callback, j_context);
    pa_stream_set_latency_update_callback (stream, stream_latency_update_callback, j_context);
    pa_stream_set_moved_callback (stream, stream_moved_callback, j_context);
    pa_stream_set_suspended_callback (stream, stream_suspended_callback, j_context);
    pa_stream_set_buffer_attr_callback(stream, buf_attr_changed_callback, j_context);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_unref
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1unref
(JNIEnv* env, jobject obj) {

    java_context* j_context = getJavaPointer(env, obj, CONTEXT_POINTER);
    assert(j_context);
    (*env)->DeleteGlobalRef(env, j_context->obj);
    free(j_context);
    setJavaPointer(env, obj, CONTEXT_POINTER, NULL);

    pa_stream* stream = getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_stream_unref(stream);
    setJavaPointer(env, obj, STREAM_POINTER, NULL);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_state
 * Signature: ()I
 */
JNIEXPORT jlong JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1state
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_get_state(stream);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_context
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1context
(JNIEnv* env, jobject obj) {

    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_context* context = pa_stream_get_context(stream);
    assert(context);
    return convertNativePointerToJava(env, context);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_index
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1index
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_get_index(stream);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_device_index
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1device_1index
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_get_device_index(stream);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_device_name
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1device_1name
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    const char* name = pa_stream_get_device_name(stream);
    assert(name);
    return (*env)->NewStringUTF(env, name);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_is_suspended
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1is_1suspended
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_is_suspended(stream);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_connect_playback
 * Signature: (Ljava/lang/String;IIIIII[B[B)I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1connect_1playback
(JNIEnv* env, jobject obj, jstring device, jint bufferMaxLength,
        jint bufferTargetLength, jint bufferPreBuffering,
        jint bufferMinimumRequest, jint bufferFragmentSize, jlong flags,
        jbyteArray volumePointer, jbyteArray sync_streamPointer) {
    pa_stream *sync_stream;
    if(sync_streamPointer != NULL) {
        sync_stream = convertJavaPointerToNative(env, sync_streamPointer);
        printf("Master stream is %p\n", sync_stream);
    } else {
        sync_stream = NULL;
    }

    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);

    pa_buffer_attr buffer_attr;

    memset(&buffer_attr, 0, sizeof(buffer_attr));

    buffer_attr.maxlength = (uint32_t) bufferMaxLength;
    buffer_attr.tlength = (uint32_t) bufferTargetLength;
    buffer_attr.prebuf = (uint32_t) bufferPreBuffering;
    buffer_attr.minreq = (uint32_t) bufferMinimumRequest;

    const char* dev = NULL;
    if (device != NULL) {
        dev = (*env)->GetStringUTFChars(env, device, NULL);
        if (dev == NULL) {
            return -1; // oome thrown
        }
    }

    int value = pa_stream_connect_playback(stream, dev, &buffer_attr,
            (pa_stream_flags_t) flags, NULL, sync_stream);

    if (dev != NULL) {
        (*env)->ReleaseStringUTFChars(env, device, dev);
        dev = NULL;
    }
    return value;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_connect_record
 * Signature: (Ljava/lang/String;IIIIII[B[B)I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1connect_1record
(JNIEnv* env, jobject obj, jstring device, jint bufferMaxLength,
        jint bufferTargetLength, jint bufferPreBuffereing,
        jint bufferMinimumRequest, jint bufferFragmentSize, jlong flags,
        jbyteArray volumePointer, jbyteArray sync_streamPointer) {

    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    pa_buffer_attr buffer_attr;
    memset(&buffer_attr, 0 , sizeof(buffer_attr));
    buffer_attr.maxlength = (uint32_t) bufferMaxLength;
    buffer_attr.fragsize = (uint32_t) bufferFragmentSize;

    const char* dev = NULL;
    if (device != NULL) {
        dev = (*env)->GetStringUTFChars(env, device, NULL);
        if (dev == NULL) {
            return -1; // oome thrown
        }
    }

    int value = pa_stream_connect_record(stream, dev, &buffer_attr,
                                         (pa_stream_flags_t) flags);

    if (dev != NULL) {
        (*env)->ReleaseStringUTFChars(env, device, dev);
        dev = NULL;
    }
    return value;

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_disconnect
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1disconnect
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    int return_value = pa_stream_disconnect(stream);

    return return_value;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_write
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1write
(JNIEnv* env, jobject obj, jbyteArray data, jint offset, jint data_length) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    jbyte* data_buffer = (*env)->GetByteArrayElements(env, data, NULL);
    if (data_buffer == NULL) {
        return -1; // oome thrown
    }
    jbyte* buffer_start = data_buffer + offset;
    int value = pa_stream_write(stream, buffer_start, data_length, NULL, 0, PA_SEEK_RELATIVE);
    (*env)->ReleaseByteArrayElements(env, data, data_buffer, 0);
    return value;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_peek
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1peek
(JNIEnv* env, jobject obj) {

    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    const void* startLocation;
    size_t count;

    if ( pa_stream_peek(stream, &startLocation, &count) < 0 ) {
        return NULL;
    }

    /* no data available */
    if (startLocation == NULL) {
        return NULL;
    }

    jsize length = count;
    jbyteArray data = (*env)->NewByteArray(env, length);

    if ( data == NULL) {
        return NULL; // oome thrown
    }

    (*env)->SetByteArrayRegion(env, data, 0, count, startLocation);
    return data;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_drop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1drop
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_drop(stream);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_writable_size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1writable_1size
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    if(!stream) {
        return 0;
    }
    size_t size = pa_stream_writable_size(stream);
    return size;

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_readable_size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1readable_1size
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_readable_size(stream);
}

static void drain_callback(pa_stream* stream, int success, void* userdata) {

    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);

    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "drain failed");
    }

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_drain
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1drain
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* operation = pa_stream_drain(stream, drain_callback, NULL);
    assert(operation);
    return convertNativePointerToJava(env, operation);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_is_corked
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1is_1corked
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    return pa_stream_is_corked(stream);
}

static void cork_callback(pa_stream* stream, int success, void* userdata) {

    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "cork failed");
    }

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_cork
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1cork
(JNIEnv* env, jobject obj, jint yes) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* operation = pa_stream_cork(stream, yes, cork_callback, NULL);
    assert(operation);
    return convertNativePointerToJava(env, operation);
}

static void flush_callback(pa_stream* stream, int success, void* userdata) {
    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "flush failed");
    }

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_flush
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1flush
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*) getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* operation = pa_stream_flush(stream, flush_callback, NULL);
    assert(operation);
    return convertNativePointerToJava(env, operation);
}

static void trigger_callback(pa_stream* stream, int success, void* userdata) {
    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "trigger failed");
    }

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_trigger
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1trigger
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* operation = pa_stream_trigger(stream, trigger_callback, NULL);
    assert(operation);
    return convertNativePointerToJava(env, operation);
}

static void set_name_callback(pa_stream* stream, int success, void* userdata) {
    assert(stream);
    JNIEnv* env = pulse_thread_env;
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "set_name failed");
    }
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_set_name
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1set_1name
(JNIEnv* env, jobject obj, jstring newName) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    const char* name;
    name = (*env)->GetStringUTFChars(env, newName, NULL);
    if (name == NULL) {
        return 0; // OutOfMemoryError already thrown
    }

    pa_operation* operation = pa_stream_set_name(stream, name, set_name_callback, NULL);
    assert(operation);
    (*env)->ReleaseStringUTFChars(env, newName, name);

    return convertNativePointerToJava(env, operation);
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_time
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1time
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    pa_usec_t time = 0;
    int result = pa_stream_get_time (stream,&time);
    assert(result == 0);

    return time;

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_latency
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1latency
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_usec_t returnValue = 0;
    int negative = 0;
    int result = pa_stream_get_latency ( stream, &returnValue, &negative);
    assert(result == 0);
    assert(negative == 0);
    return returnValue;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_sample_spec
 * Signature: ()Lorg/classpath/icedtea/pulseaudio/StreamSampleSpecification;
 */
JNIEXPORT jobject JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1sample_1spec
(JNIEnv* env, jobject obj) {
    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    const pa_sample_spec* sample_spec = pa_stream_get_sample_spec(stream);
    assert(sample_spec);

    char* name = "Lorg/classpath/icedtea/pulseaudio/StreamSampleSpecification;";
    jclass cls = (*env)->FindClass(env, name);
    assert(cls);
    jmethodID constructor_mid = (*env)->GetMethodID(env, cls, "<init>", "V");
    assert(constructor_mid);

    const char* formatString = getStringFromFormat(sample_spec->format);
    assert(formatString);
    int rate = sample_spec->rate;
    int channels = sample_spec->channels;

    jstring format = (*env)->NewStringUTF(env, formatString);
    if ( format == NULL) {
        return NULL; // oome
    }
    jobject return_object = (*env)->NewObject(env, cls, constructor_mid, format, rate, channels);

    return return_object;
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_get_buffer_attr
 * Signature: ()Lorg/classpath/icedtea/pulseaudio/StreamBufferAttributes;
 */
JNIEXPORT jobject JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1get_1buffer_1attr
(JNIEnv* env, jobject obj) {

    //    printf("in native_pa_stream_get_buffer_attributes");

    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    const pa_buffer_attr* buffer = pa_stream_get_buffer_attr(stream);
    assert(buffer);

    const char* class_name = "org/classpath/icedtea/pulseaudio/StreamBufferAttributes";
    jclass cls = (*env)->FindClass(env, class_name);
    assert(cls);
    jmethodID constructor_mid = (*env)->GetMethodID(env, cls, "<init>", "(IIIII)V");
    assert(constructor_mid);
    jint maxLength = buffer->maxlength;
    jint targetLength = buffer->tlength;
    jint preBuffering = buffer->prebuf;
    jint minimumRequest = buffer->minreq;
    jint fragmentSize = buffer->fragsize;

    jobject return_object = (*env)->NewObject(env, cls, constructor_mid, maxLength, targetLength,
            preBuffering, minimumRequest, fragmentSize);

    return return_object;
}

static void set_buffer_attr_callback(pa_stream* stream, int success,
        void* userdata) {

    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "set_buffer_attr failed");
    }
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_set_buffer_attr
 * Signature: (Lorg/classpath/icedtea/pulseaudio/StreamBufferAttributes;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1set_1buffer_1attr
(JNIEnv* env, jobject obj, jobject bufferAttributeObject) {

    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    jclass cls = (*env)->GetObjectClass(env, bufferAttributeObject);
    assert(cls);

    pa_buffer_attr buffer;

    jmethodID getMaxLengthID = (*env)->GetMethodID(env,cls,"getMaxLength","()I");
    assert(getMaxLengthID);
    buffer.maxlength = (uint32_t) (*env)->CallIntMethod(env, bufferAttributeObject, getMaxLengthID);

    jmethodID getTargetLengthID = (*env)->GetMethodID(env,cls,"getTargetLength","()I");
    assert(getTargetLengthID);
    buffer.tlength = (uint32_t) (*env)->CallIntMethod(env, bufferAttributeObject, getTargetLengthID);

    jmethodID getPreBufferingID = (*env)->GetMethodID(env,cls,"getPreBuffering","()I");
    assert(getPreBufferingID);
    buffer.prebuf = (uint32_t) (*env)->CallIntMethod(env, bufferAttributeObject, getPreBufferingID);

    jmethodID getMinimumRequestID = (*env)->GetMethodID(env, cls, "getMinimumRequest", "()I");
    assert(getMinimumRequestID);
    buffer.minreq = (uint32_t) (*env)->CallIntMethod(env, bufferAttributeObject, getMinimumRequestID );

    jmethodID getFragmentSizeID = (*env)->GetMethodID(env,cls,"getFragmentSize","()I");
    assert(getFragmentSizeID);
    buffer.fragsize = (uint32_t) (*env)->CallIntMethod(env, bufferAttributeObject, getFragmentSizeID );

    /*
     const pa_buffer_attr* old_buffer = pa_stream_get_buffer_attr(stream);

     printf("old buffer values: %u %u %u %u %u\n", old_buffer->maxlength, old_buffer->tlength, old_buffer->prebuf, old_buffer->minreq, old_buffer->fragsize);

     printf("want these values: %u %u %u %u %u\n", buffer.maxlength, buffer.tlength, buffer.prebuf, buffer.minreq, buffer.fragsize);
     */

    pa_operation* operation = pa_stream_set_buffer_attr(stream, &buffer, set_buffer_attr_callback, NULL);

    assert(operation);
    return convertNativePointerToJava(env,operation);
}

static void update_sample_rate_callback(pa_stream* stream, int success,
        void* userdata) {
    assert(stream);
    JNIEnv* env = pulse_thread_env;
    assert(env);
    notifyWaitingOperations(env);

    if (success == 0) {
        throwByName(env, ILLEGAL_STATE_EXCEPTION, "update_sampl_rate failed");
    }

}
/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_pa_stream_update_sample_rate
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1update_1sample_1rate
(JNIEnv* env, jobject obj, jint newRate) {

    uint32_t rate = (uint32_t) newRate;

    pa_stream* stream = (pa_stream*)getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* operation = pa_stream_update_sample_rate(stream,rate, update_sample_rate_callback, NULL);
    assert(operation);
    return convertNativePointerToJava(env, operation);

}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_set_volume
 * Signature: (F)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1set_1volume
(JNIEnv *env, jobject obj, jfloat new_volume) {

    pa_stream *stream = getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_context *context = pa_stream_get_context(stream);
    assert(context);

    int stream_id = pa_stream_get_index(stream);
    int channels = pa_stream_get_sample_spec(stream)->channels;
    pa_cvolume cv;

    pa_operation* o = pa_context_set_sink_input_volume(context, stream_id, pa_cvolume_set(&cv, channels, new_volume), set_sink_input_volume_callback, NULL);
    assert(o);

    return convertNativePointerToJava(env, o);

}


static void get_sink_input_volume_callback(pa_context *context, const pa_sink_input_info *i,
        int eol, void *userdata) {

    JNIEnv* env = pulse_thread_env;

    assert(context);
    assert(env);
    jobject obj = (jobject) userdata;
    assert(obj);

    if (eol == 0) {
        jclass cls = (*pulse_thread_env)->GetObjectClass(pulse_thread_env, obj);
        assert(cls);
        jmethodID mid1 = (*pulse_thread_env)->GetMethodID(pulse_thread_env, cls,
                "update_channels_and_volume", "(IF)V");
        assert(mid1);
        (*pulse_thread_env)->CallVoidMethod(pulse_thread_env, obj, mid1,
                (int) (i->volume).channels, (float) (i->volume).values[0]) ;
    } else {
        notifyWaitingOperations(pulse_thread_env);
        (*env)->DeleteGlobalRef(env, obj);
    }
}

/*
 * Class:     org_classpath_icedtea_pulseaudio_Stream
 * Method:    native_update_volume
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1update_1volume
(JNIEnv* env, jobject obj) {

    pa_stream* stream = getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);

    int sink_input_index = pa_stream_get_index(stream);

    pa_context* context = pa_stream_get_context(stream);
    assert(context);

    obj = (*env)->NewGlobalRef(env, obj);
    pa_operation *o = pa_context_get_sink_input_info(context, sink_input_index , get_sink_input_volume_callback, obj);
    assert(o);
    return convertNativePointerToJava(env, o);


}

JNIEXPORT jint JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_bytesInBuffer(JNIEnv *env, jobject obj) {
    pa_stream *stream = getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    const pa_timing_info *timing_info = pa_stream_get_timing_info(stream);
    int write_index = timing_info->write_index;
    int read_index = timing_info->read_index;
    return write_index - read_index;
}

JNIEXPORT jbyteArray JNICALL Java_org_classpath_icedtea_pulseaudio_Stream_native_1pa_1stream_1updateTimingInfo(JNIEnv* env, jobject obj) {
    pa_stream *stream = getJavaPointer(env, obj, STREAM_POINTER);
    assert(stream);
    pa_operation* o = pa_stream_update_timing_info(stream, update_timing_info_callback, NULL);
    assert(o);
    return convertNativePointerToJava(env, o);

}



