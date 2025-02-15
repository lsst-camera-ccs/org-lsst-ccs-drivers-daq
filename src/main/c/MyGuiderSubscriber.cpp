#include <stdio.h>

#include "MyGuiderSubscriber.h"
#include "Store.h"

using namespace GDS;

static jclass JCsubscriberClass;
static jmethodID JCsubscriberStartCallbackMethod;
static jmethodID JCsubscriberStopCallbackMethod;
static jmethodID JCsubscriberPauseCallbackMethod;
static jmethodID JCsubscriberResumeCallbackMethod;
static jmethodID JCsubscriberRawStampCallbackMethod;
static jmethodID JCsubscriberStampCallbackMethod;
static jclass JCguiderStateMetadataClass;
static jmethodID JCguiderStateMetadataConstructor;
static jclass JCguiderSeriesMetadataClass;
static jmethodID JCguiderSeriesMetadataConstructor;
static jclass JCguiderRoiCommonClass;
static jmethodID JCguiderRoiCommonConstructor;
static jclass JCguiderRoiLocationClass;
static jmethodID JCguiderRoiLocationConstructor;
static jclass JCguiderConfigClass;
static jmethodID JCguiderConfigConstructor;
static jclass JCguiderSeriesClass;
static jmethodID JCguiderSeriesConstructor;
static jclass JCguiderSeriesStatusClass;
static jmethodID JCguiderSeriesStatusConstructor;
static jclass JCguiderSensorLocationClass;
static jmethodID JCguiderSensorLocationConstructor;
static jclass JCguiderStatusClass;
static jmethodID JCguiderStatusConstructor;
static jclass JCguiderClearParametersClass;
static jmethodID JCguiderClearParametersConstructor;

MyGuiderSubscriber::MyGuiderSubscriber(const char* partition, bool bigEndian, const GDS::LocationSet& locs) :
    GDS::Decoder(partition, locs, bigEndian) {
}

jobject createGuiderStatus(JNIEnv* env, const GDS::Status& status) {
   jlong time = (uint64_t) status.timestamp();
   jint state = status.status();
   jint sequence = status.sequence();
   jint in = status.state_in();
   jint out = status.state_out();
   return env->NewObject(JCguiderStatusClass, JCguiderStatusConstructor, time, state, sequence, in, out);
}

jobject createRoiCommon(JNIEnv* env, const GDS::RoiCommon& location) {
    jint nrows = location.nrows();
    jint ncols = location.ncols();
    jint integration = location.integration();

    return env->NewObject(JCguiderRoiCommonClass, JCguiderRoiCommonConstructor, nrows, ncols, integration);
}

jobject createRoiLocation(JNIEnv* env, const GDS::RoiLocation& location) {
    GDS::Location loc = location.location();
    const DAQ::Location source = loc.source();
    jint sensor = loc.sensor();
    jint segment = location.segment();
    jint startRow = location.startrow();
    jint startCol = location.startcol();
    jbyte bay = source.bay();
    jbyte board = source.board();

    return env->NewObject(JCguiderRoiLocationClass, JCguiderRoiLocationConstructor, bay, board, sensor, segment, startRow, startCol);
}

jobject createGuiderConfig(JNIEnv* env, const GDS::Status& status, const GDS::ClearParameters& clearParamaters, const GDS::RoiCommon& common, const GDS::RoiLocation* location, int nLocations) {
    jobject list = createList(env);
    for (int i=0; i<nLocations; i++) {
        addObjectToList(env, list, createRoiLocation(env, location[i]));
    }
    return env->NewObject(JCguiderConfigClass, JCguiderConfigConstructor, createGuiderStatus(env, status), createGuiderClearParameters(env, clearParamaters), createRoiCommon(env, common), list);
}

jobject createGuiderClearParameters(JNIEnv* env, const GDS::ClearParameters& clearParameters) {
    jint delay =  clearParameters.delay();
    jint preRows = clearParameters.preRows();
    jint flushCount = clearParameters.flushCount();
    jint readRows = clearParameters.readRows();
    return env->NewObject(JCguiderClearParametersClass, JCguiderClearParametersConstructor, delay, preRows, flushCount, readRows);
}

jobject createSensorLocation(JNIEnv* env, const GDS::Location& location) {
    const DAQ::Location source = location.source();
    jint sensor = location.sensor();
    jbyte bay = source.bay();
    jbyte board = source.board();

    return env->NewObject(JCguiderSensorLocationClass, JCguiderSensorLocationConstructor, bay, board, sensor);
}

jobject createSensorLocations(JNIEnv* env, const GDS::LocationSet& locations) {
    jobject list = createList(env);
    GDS::LocationSet remaining = locations;
    GDS::Location loc;
    while(remaining.remove(loc)) {
       addObjectToList(env, list, createSensorLocation(env, loc));
    }
    return list;
}

jobject createGuiderSeriesStatus(JNIEnv* env, const GDS::Status& status, const GDS::Series& series, const GDS::Series& idle) {
    jobject jStatus = createGuiderStatus(env, status);
    jobject jSeries = createGuiderSeries(env, series);
    jobject jIdle = createGuiderSeries(env, idle);

    return env->NewObject(JCguiderSeriesStatusClass, JCguiderSeriesStatusConstructor, jStatus, jSeries, jIdle);
}

jobject createGuiderSeries(JNIEnv* env, const GDS::Series& series) {
    jlong timestamp = (uint64_t) series.begin();
    jint sequence = series.sequence();
    jint stamps = series.stamps();
    jobject configured = createSensorLocations(env, series.configured());
    jobject remaining = createSensorLocations(env, series.remaining());

    return env->NewObject(JCguiderSeriesClass, JCguiderSeriesConstructor, timestamp, sequence, stamps, configured, remaining);
}

jobject createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state) {
    jint type = state.type();
    jint status = state.status();
    jint sequence = state.sequence();
    jlong timestamp = (uint64_t) state.timestamp();
    jstring comment = env->NewStringUTF(state.comment());
    const GDS::Location location = state.sensor();
    const DAQ::Location source = location.source();
    jint sensor = location.sensor();
    return env->NewObject(JCguiderStateMetadataClass, JCguiderStateMetadataConstructor, type, status, sequence, timestamp, location.bay(), source.board(), sensor, comment);
}

jobject createSeriesMetadata(JNIEnv* env, const GDS::SeriesMetadata& series) {
    const RoiCommon& common = series.common();
    const RoiLocation& location = series.location();
    const DVI::Version version =  series.software();
    jint firmware = series.firmware();
    jlong serialNumber = series.serial_number();
    jstring id = env->NewStringUTF(series.id());
    jstring platform = env->NewStringUTF(series.platform());
    jboolean splitroi = series.splitroi();
    jint undercols = 0; // FIXME
    jint ccdType = series.vendor();

    return env->NewObject(JCguiderSeriesMetadataClass, JCguiderSeriesMetadataConstructor, firmware, serialNumber, id, platform, createRoiCommon(env, common), createRoiLocation(env, location), createVersion(env, version), splitroi, undercols, ccdType);
}

jobject MyGuiderSubscriber::createByteBuffer(JNIEnv* env, const GDS::RawStamp& stamp) {
    // I know I am not going to modify the mapped buffer, but the compiler does not know that
    return env->NewDirectByteBuffer(const_cast<uint8_t *>(stamp.content()), stamp.size());
}

jobject MyGuiderSubscriber::createByteBuffer(JNIEnv* env, const GDS::Stamp& stamp) {
    // I know I am not going to modify the mapped buffer, but the compiler does not know that
    return env->NewDirectByteBuffer(const_cast<uint8_t *>(stamp.content()), stamp.size());
}

void MyGuiderSubscriber::start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series) {
    env->CallVoidMethod(callback, JCsubscriberStartCallbackMethod, createStateMetadata(env, state), createSeriesMetadata(env, series));
}

void MyGuiderSubscriber::resume(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCsubscriberResumeCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::pause(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCsubscriberPauseCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::stop(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, JCsubscriberStopCallbackMethod, createStateMetadata(env, state));
}

void MyGuiderSubscriber::raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp) {
    env->CallVoidMethod(callback, JCsubscriberRawStampCallbackMethod, createStateMetadata(env, state), createByteBuffer(env, stamp));
}

void MyGuiderSubscriber::stamp(const GDS::StateMetadata& state, const GDS::Stamp& stamp) {
    env->CallVoidMethod(callback, JCsubscriberStampCallbackMethod, createStateMetadata(env, state), createByteBuffer(env, stamp));
}

uint8_t* MyGuiderSubscriber::allocate(unsigned size) {
    //TODO: needs to be dynamically allocated?
    return _stamp_buf;
}

void MyGuiderSubscriber::wait(JNIEnv *env, jobject callback) {
    this->env = env;
    this->callback = callback;
    GDS::Decoder::wait();
}

void MyGuiderSubscriber::abort(JNIEnv *env) {
    this->env = env;
    GDS::Decoder::abort();
}

void Guider_OnLoad(JNIEnv* env) {

    jclass subscriberClass = env->FindClass("org/lsst/ccs/daq/ims/Guider$Subscriber");
    if (env->ExceptionCheck()) {
        return;
    }
    JCsubscriberClass = (jclass) env->NewGlobalRef(subscriberClass);

    JCsubscriberStartCallbackMethod = env->GetMethodID(JCsubscriberClass, "startCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;Lorg/lsst/ccs/daq/guider/SeriesMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCsubscriberStopCallbackMethod = env->GetMethodID(JCsubscriberClass, "stopCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCsubscriberPauseCallbackMethod = env->GetMethodID(JCsubscriberClass, "pauseCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCsubscriberResumeCallbackMethod = env->GetMethodID(JCsubscriberClass, "resumeCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCsubscriberRawStampCallbackMethod = env->GetMethodID(JCsubscriberClass, "rawStampCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;Ljava/nio/ByteBuffer;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    JCsubscriberStampCallbackMethod = env->GetMethodID(JCsubscriberClass, "stampCallback", "(Lorg/lsst/ccs/daq/guider/StateMetaData;Ljava/nio/ByteBuffer;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderStateMetadataClass = env->FindClass("org/lsst/ccs/daq/guider/StateMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderStateMetadataClass = (jclass) env->NewGlobalRef(guiderStateMetadataClass);

    JCguiderStateMetadataConstructor = env->GetMethodID(JCguiderStateMetadataClass, "<init>", "(IIIJBBILjava/lang/String;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSeriesMetadataClass = env->FindClass("org/lsst/ccs/daq/guider/SeriesMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSeriesMetadataClass = (jclass) env->NewGlobalRef(guiderSeriesMetadataClass);

    JCguiderSeriesMetadataConstructor = env->GetMethodID(JCguiderSeriesMetadataClass, "<init>", "(IJLjava/lang/String;Ljava/lang/String;Lorg/lsst/ccs/daq/guider/ROICommon;Lorg/lsst/ccs/daq/guider/ROILocation;Lorg/lsst/ccs/daq/ims/Version;ZII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderRoiCommonClass = env->FindClass("org/lsst/ccs/daq/guider/ROICommon");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderRoiCommonClass = (jclass) env->NewGlobalRef(guiderRoiCommonClass);

    JCguiderRoiCommonConstructor = env->GetMethodID(JCguiderRoiCommonClass, "<init>", "(III)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderRoiLocationClass = env->FindClass("org/lsst/ccs/daq/guider/ROILocation");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderRoiLocationClass = (jclass) env->NewGlobalRef(guiderRoiLocationClass);

    JCguiderRoiLocationConstructor = env->GetMethodID(JCguiderRoiLocationClass, "<init>", "(BBIIII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderConfigClass = env->FindClass("org/lsst/ccs/daq/guider/Config");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderConfigClass = (jclass) env->NewGlobalRef(guiderConfigClass);

    JCguiderConfigConstructor = env->GetMethodID(JCguiderConfigClass, "<init>", "(Lorg/lsst/ccs/daq/guider/Series;Lorg/lsst/ccs/daq/guider/ROICommon;Ljava/util/List;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderClearParametersClass = env->FindClass("org/lsst/ccs/daq/guider/ClearParameters");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderClearParametersClass = (jclass) env->NewGlobalRef(guiderClearParametersClass);

    JCguiderClearParametersConstructor = env->GetMethodID(JCguiderClearParametersClass, "<init>", "(IIII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSeriesStatusClass = env->FindClass("org/lsst/ccs/daq/guider/SeriesStatus");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSeriesStatusClass = (jclass) env->NewGlobalRef(guiderSeriesStatusClass);

    JCguiderSeriesConstructor = env->GetMethodID(guiderSeriesStatusClass, "<init>", "(Lorg/lsst/ccs/daq/guider/Status;Lorg/lsst/ccs/daq/guider/Series;Lorg/lsst/ccs/daq/guider/Series)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSeriesClass = env->FindClass("org/lsst/ccs/daq/guider/Series");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSeriesClass = (jclass) env->NewGlobalRef(guiderSeriesClass);

    JCguiderSeriesConstructor = env->GetMethodID(guiderSeriesClass, "<init>", "(JIILjava/util/List;Ljava/util/List;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderSensorLocationClass = env->FindClass("org/lsst/ccs/utilities/location/SensorLocation");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderSensorLocationClass = (jclass) env->NewGlobalRef(guiderSensorLocationClass);

    JCguiderSensorLocationConstructor = env->GetMethodID(guiderSensorLocationClass, "<init>", "(BBI)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass guiderStatusClass = env->FindClass("org/lsst/ccs/daq/guider/Status");
    if (env->ExceptionCheck()) {
        return;
    }
    JCguiderStatusClass = (jclass) env->NewGlobalRef(guiderStatusClass);

    JCguiderStatusConstructor = env->GetMethodID(guiderStatusClass, "<init>", "(JIIII)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}
