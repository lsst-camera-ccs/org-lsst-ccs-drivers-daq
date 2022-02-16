#include <stdio.h>

#include "MyGuiderSubscriber.h"
#include "Store.h"

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
static jclass JCguiderRoiCommonClass;
static jmethodID JCguiderRoiCommonConstructor;
static jclass JCguiderRoiLocationClass;
static jmethodID JCguiderRoiLocationConstructor;

MyGuiderSubscriber::MyGuiderSubscriber(const char* partition, const GDS::LocationSet& locs) :
    GDS::Subscriber(partition, locs) {
}

jobject MyGuiderSubscriber::createRoiCommon(JNIEnv* env, const GDS::RoiCommon& location) {
    jint nrows = location.nrows();
    jint ncols = location.ncols();
    jint integration = location.integration();
    jint binning = location.binning();

    return env->NewObject(JCguiderRoiCommonClass, JCguiderRoiCommonConstructor, nrows, ncols, integration, binning);
}

jobject MyGuiderSubscriber::createRoiLocation(JNIEnv* env, const GDS::RoiLocation& location) {
    GDS::Location loc = location.location();
    const DAQ::Location source = loc.source();
    jint sensor = loc.sensor();
    jint segment = location.segment();
    jint startRow = location.startrow();
    jint startCol = location.startcol();

    return env->NewObject(JCguiderRoiLocationClass, JCguiderRoiLocationConstructor, source.bay(), source.board(), sensor, segment, startRow, startCol);
}

jobject MyGuiderSubscriber::createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state) {
    jint type = state.type();
    jint status = state.status();
    jint sequence = state.sequence();
    jlong timestamp = state.timestamp();
    const GDS::Location location = state.sensor();
    const DAQ::Location source = location.source();
    jint sensor = location.sensor();
    return env->NewObject(JCguiderStateMetadataClass, JCguiderStateMetadataConstructor, type, status, sequence, timestamp, location.bay(), source.board(), sensor);
}

jobject MyGuiderSubscriber::createSeriesMetadata(JNIEnv* env, const GDS::SeriesMetadata& series) {
    const RoiCommon& common = series.common();
    const RoiLocation& location = series.location();
    const DVI::Version version =  series.software();
    jint firmware = series.firmware();
    jlong serialNumber = series.serial_number();

    return env->NewObject(JCguiderSeriesMetadataClass, JCguiderSeriesMetadataConstructor, firmware, serialNumber, createRoiCommon(env, common), createRoiLocation(env, location), createVersion(env, version));
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

void MyGuiderSubscriber::wait(JNIEnv *env, jobject callback) {
    this->env = env;
    this->callback = callback;
    GDS::Subscriber::wait();
}
void Guider_OnLoad(JNIEnv* env) {

    jclass guiderClass = env->FindClass("org/lsst/ccs/daq/ims/Guider");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderClass = (jclass) env->NewGlobalRef(guiderClass);

    JCguiderStartCallbackMethod = env->GetMethodID(JCguiderClass, "startCallback", "(Lorg/lsst/ccs/daq/ims/Guider$StateMetaData;Lorg/lsst/ccs/daq/ims/Guider$SeriesMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderStopCallbackMethod = env->GetMethodID(JCguiderClass, "stopCallback", "(Lorg/lsst/ccs/daq/ims/Guider$StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderPauseCallbackMethod = env->GetMethodID(JCguiderClass, "pauseCallback", "(Lorg/lsst/ccs/daq/ims/Guider$StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderResumeCallbackMethod = env->GetMethodID(JCguiderClass, "resumeCallback", "(Lorg/lsst/ccs/daq/ims/Guider$StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCguiderStampCallbackMethod = env->GetMethodID(JCguiderClass, "stampCallback", "(Lorg/lsst/ccs/daq/ims/Guider$StateMetaData;Ljava/nio/ByteBuffer;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderStateMetadataClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$StateMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderStateMetadataClass = (jclass) env->NewGlobalRef(guiderStateMetadataClass);

    JCguiderStateMetadataConstructor = env->GetMethodID(JCguiderStateMetadataClass, "<init>", "(IIIJBBI)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSeriesMetadataClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$SeriesMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSeriesMetadataClass = (jclass) env->NewGlobalRef(guiderSeriesMetadataClass);

    JCguiderSeriesMetadataConstructor = env->GetMethodID(JCguiderSeriesMetadataClass, "<init>", "(IJLorg/lsst/ccs/daq/ims/Guider$ROICommon;Lorg/lsst/ccs/daq/ims/Guider$ROILocation;Lorg/lsst/ccs/daq/ims/Version;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderRoiCommonClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$ROICommon");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderRoiCommonClass = (jclass) env->NewGlobalRef(guiderRoiCommonClass);

    JCguiderRoiCommonConstructor = env->GetMethodID(JCguiderRoiCommonClass, "<init>", "(IIII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderRoiLocationClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$ROILocation");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderRoiLocationClass = (jclass) env->NewGlobalRef(guiderRoiLocationClass);

    JCguiderRoiLocationConstructor = env->GetMethodID(JCguiderRoiLocationClass, "<init>", "(BBIIII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}


