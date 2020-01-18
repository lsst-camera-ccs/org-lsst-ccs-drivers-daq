#include "Statistics.h"

#include "org_lsst_ccs_daq_ims_Stats.h"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"
#include "rms/Client.hh"
#include "rms/Stats.hh"
#include "rds/Client.hh"
#include "rds/Stats.hh"
#include "daq/InterfaceDriverStats.hh"
#include "daq/InterfaceFirmwareStats.hh"
#include "rms/Errors.hh"
#include "dsm/Exception.hh"
#include "xds/Page.hh"

#define MESSAGE_LENGTH 256

//static jclass JCversionClass;
//static jmethodID JCversionConstructor;
static jclass JCdaqRmsStatsClass;
static jmethodID JCdaqRmsStatsConstructor;
static jclass JCdaqRdsStatsClass;
static jmethodID JCdaqRdsStatsConstructor;
static jclass JCdaqDriverStatsClass;
static jmethodID JCdaqDriverStatsConstructor;
static jclass JCdaqFirmwareStatsClass;
static jmethodID JCdaqFirmwareStatsConstructor;
static jclass JCexClass;
static jmethodID JCexConstructor;

jstring decodeRms(JNIEnv* env, jint error) {
   const char* decoded = RMS::Errors::decode(error);
   // TODO: Check for other exceptions
   return env->NewStringUTF(decoded);
}

void throwDAQStatsException(JNIEnv* env, char* message, jint error) {
    jstring message_ = env->NewStringUTF(message);
    jstring decoded = decodeRms(env, error);
    jthrowable exception = (jthrowable) env->NewObject(JCexClass, JCexConstructor, message_, error, decoded);
    env->Throw(exception);
}

jobject createDAQRmsStats(JNIEnv* env, const DAQ::Location& location, 
                          RMS::Client* client) {
    RMS::Stats rmsStats;
    int32_t error;
    if (client->stats(location, true, rmsStats, error)) {

        jbyte bay          = (jbyte) location.bay();
        jbyte board        = (jbyte) location.board();        
        jlong probes       = (jlong) rmsStats.probes()       ;
        jlong unopened     = (jlong) rmsStats.unopened()     ;
        jlong in_reset     = (jlong) rmsStats.in_reset()     ;
        jlong link_down    = (jlong) rmsStats.link_down()    ;
        jlong lists_posted = (jlong) rmsStats.lists_posted() ;
        jlong ops_posted   = (jlong) rmsStats.ops_posted()   ;
        jlong waits        = (jlong) rmsStats.waits()        ;
        jlong responses    = (jlong) rmsStats.responses()    ;
        jlong inv_tids     = (jlong) rmsStats.inv_tids()     ;
        jlong orphans      = (jlong) rmsStats.orphans()      ;
        jlong rundowns     = (jlong) rmsStats.rundowns()     ;
        jlong tid_hwm      = (jlong) rmsStats.tid_hwm()      ;
        
        return env->NewObject(JCdaqRmsStatsClass, JCdaqRmsStatsConstructor, bay, board, probes, unopened, in_reset, link_down, lists_posted, ops_posted, waits, responses, inv_tids, orphans, rundowns, tid_hwm);

    } else {
      char x[MESSAGE_LENGTH];
      snprintf(x, MESSAGE_LENGTH, "createDAQRmsStats failed");
      throwDAQStatsException(env, x, error);
    }
}

//static jint JNI_VERSION = JNI_VERSION_1_8;

//This function is meant to be called from a top-level (or global) 
//OnLoad function, which in turn checks and returns JNI_VERSION.

void JNI_Stats_OnLoad(JNIEnv* env) {

    jclass daqRmsStatsClass = env->FindClass("org/lsst/ccs/daq/ims/DAQRmsStats");
    if (env->ExceptionCheck()) {
        return;
    }
    JCdaqRmsStatsClass = (jclass) env->NewGlobalRef(daqRmsStatsClass);

    JCdaqRmsStatsConstructor = env->GetMethodID(JCdaqRmsStatsClass, "<init>", "(BBJJJJJJJJJJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass daqRdsStatsClass = env->FindClass("org/lsst/ccs/daq/ims/DAQRdsStats");
    if (env->ExceptionCheck()) {
        return;
    }
    JCdaqRdsStatsClass = (jclass) env->NewGlobalRef(daqRdsStatsClass);

    JCdaqRdsStatsConstructor = env->GetMethodID(JCdaqRdsStatsClass, "<init>", "(BBJJJJJJJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass daqFirmwareStatsClass = env->FindClass("org/lsst/ccs/daq/ims/DAQFirmwareStats");
    if (env->ExceptionCheck()) {
        return;
    }
    JCdaqFirmwareStatsClass = (jclass) env->NewGlobalRef(daqFirmwareStatsClass);

    JCdaqFirmwareStatsConstructor = env->GetMethodID(JCdaqFirmwareStatsClass, "<init>", "(BBJJJJJJJJJJJJJJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass daqDriverStatsClass = env->FindClass("org/lsst/ccs/daq/ims/DAQDriverStats");
    if (env->ExceptionCheck()) {
        return;
    }
    JCdaqDriverStatsClass = (jclass) env->NewGlobalRef(daqDriverStatsClass);

    JCdaqDriverStatsConstructor = env->GetMethodID(JCdaqDriverStatsClass, "<init>", "(BBJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}


JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Statistics_getRmsStats
(JNIEnv *env, jobject obj, jlong client, jint elementIndex) {
    RMS::Client* client_ = (RMS::Client*)client;
    DAQ::Location element(elementIndex);
    return createDAQRmsStats(env, element, client_);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_rms_Client_attachRmsClient
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        RMS::Client* client = new RMS::Client(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) client;
    } catch (DSM::Exception& x) {
        env->ReleaseStringUTFChars(partition, partition_name);
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_rms_Client_detachRmsClient
(JNIEnv* env, jobject obj, jlong client) {
  delete ((RMS::Client*) client);
}
