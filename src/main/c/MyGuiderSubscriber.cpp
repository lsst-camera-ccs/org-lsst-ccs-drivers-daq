#include <stdio.h>

#include "MyGuiderSubscriber.h"

using namespace GDS;

static jclass JCguiderClass;
static jmethodID JCguiderStartCallbackMethod;
static jmethodID JCguiderStopCallbackMethod;
static jmethodID JCguiderPauseCallbackMethod;
static jmethodID JCguiderResumeCallbackMethod;
static jmethodID JCguiderStampCallbackMethod;
static jclass JCguiderStateMetadataClass;
static jmethodID JCguiderStateMetadataConstructor;
static jclass JCguiderSeriesMetadataClass;
static jmethodID JCguiderSeriesMetadataConstructor;

MyGuiderSubscriber::MyGuiderSubscriber(JNIEnv *env, jobject callback, const char* partition, const GDS::LocationSet& locs) :
    env(env), callback(callback), GDS::Subscriber(partition, locs) {
}

jobject MyGuiderSubscriber::createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state) {
    jint type = state.type();
    jint status = state.status();
    jint sequence = state.sequence();
    jlong timestamp = state.timestamp();
    return env->NewObject(JCguiderStateMetadataClass, JCguiderStateMetadataConstructor, type, status, sequence, timestamp);
}

jobject MyGuiderSubscriber::createSeriesMetadata(JNIEnv* env, const GDS::SeriesMetadata& series) {
    const RoiCommon& common = series.common();
    const RoiLocation& location = series.location();
    const DVI::Version version =  series.software();
    jint firmware = series.firmware();
    jlong serialNumber = series.serial_number();

    return env->NewObject(JCguiderSeriesMetadataClass, JCguiderSeriesMetadataConstructor, firmware, serialNumber);
}


jobject MyGuiderSubscriber::createByteBuffer(JNIEnv* env, const GDS::RawStamp& stamp) {
    return env->NewDirectByteBuffer(const_cast<GDS::RawStamp&>(stamp).content(), const_cast<GDS::RawStamp&>(stamp).size());
}

void MyGuiderSubscriber::start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series) {
    env->CallVoidMethod(callback, JCguiderStartCallbackMethod, createStateMetadata(env, state), createSeriesMetadata(env, series));
}

void MyGuiderSubscriber::resume(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCguiderResumeCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::pause(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCguiderPauseCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::stop(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCguiderStopCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp) {
    env->CallVoidMethod(callback, JCguiderStampCallbackMethod, createStateMetadata(env, state), createByteBuffer(env, stamp));
}

void MyGuider_OnLoad(JNIEnv* env) {

    jclass guiderClass = env->FindClass("org/lsst/ccs/daq/ims/Guider");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderClass = (jclass) env->NewGlobalRef(guiderClass);

    JCguiderStartCallbackMethod = env->GetMethodID(JCguiderClass, "startCallback", "(Lorg/lsst/ccs/daq/ims/StateMetaData;Lorg/lsst/ccs/daq/ims/SeriesMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderStopCallbackMethod = env->GetMethodID(JCguiderClass, "stopCallback", "(Lorg/lsst/ccs/daq/ims/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderPauseCallbackMethod = env->GetMethodID(JCguiderClass, "pauseCallback", "(Lorg/lsst/ccs/daq/ims/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderResumeCallbackMethod = env->GetMethodID(JCguiderClass, "resumeCallback", "(Lorg/lsst/ccs/daq/ims/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderStampCallbackMethod = env->GetMethodID(JCguiderClass, "stampCallback", "(Lorg/lsst/ccs/daq/ims/StateMetaData;java.nio.ByteBuffer;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderStateMetadataClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$StateMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderStateMetadataClass = (jclass) env->NewGlobalRef(guiderStateMetadataClass);

    JCguiderStateMetadataConstructor = env->GetMethodID(JCguiderStateMetadataClass, "<init>", "(IIIL)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSeriesMetadataClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$SeriesMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSeriesMetadataClass = (jclass) env->NewGlobalRef(guiderSeriesMetadataClass);

    JCguiderSeriesMetadataConstructor = env->GetMethodID(JCguiderSeriesMetadataClass, "<init>", "(IL)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}


