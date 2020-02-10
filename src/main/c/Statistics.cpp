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

/*  Statistics.cpp: a collection of functions o interface between Java code
 *  and DAQ statistics information.  The four sets of information accessed
 *  are Rms::Stats, Rds::Stats, Daq::InterfaceDriverStats and
 *  Daq::InterfaceFirmwareStats.
 */

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

/*  Methods to create the four Java classes which hold the statisics data 
 *  Three of the sets of data are accessed with RMS::Client, the other
 *  with RDS::Client.  The argument "clear" determines whether the
 *  statistics are cleared after reading.
 */

jobject createDAQRmsStats(JNIEnv* env, const DAQ::Location& location, 
                          RMS::Client* client, bool clear) {
    RMS::Stats rmsStats;
    int32_t error;
    if (client->stats(location, clear, rmsStats, error)) {

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
      return NULL;
    }
}

jobject createDAQRdsStats(JNIEnv* env, const DAQ::Location& location, 
                          RDS::Client* client, bool clear) {
    RDS::Stats rdsStats;
    int32_t error;
    if (client->stats(location, clear, rdsStats, error)) {

        jbyte bay            = (jbyte) location.bay();
        jbyte board          = (jbyte) location.board();        
        jlong wakes	     = (jlong) rdsStats.wakes();	      
        jlong discards       = (jlong) rdsStats.discards();      
        jlong bdi_waits      = (jlong) rdsStats.bdi_waits();     
        jlong bdi_nulls      = (jlong) rdsStats.bdi_nulls();     
        jlong bdi_seq_errors = (jlong) rdsStats.bdi_seq_errors();
        jlong bdi_overflows  = (jlong) rdsStats.bdi_overflows(); 
        // jlong no_event       = (jlong) rdsStats.no_event();      
        jlong bdi_firsts     = (jlong) rdsStats.bdi_firsts();    
        jlong bdi_lasts      = (jlong) rdsStats.bdi_lasts();     
        jlong bdi_frees      = (jlong) rdsStats.bdi_frees();     
        
        return env->NewObject(JCdaqRdsStatsClass, JCdaqRdsStatsConstructor, bay, board, wakes, discards, bdi_waits, bdi_nulls, bdi_seq_errors, bdi_overflows, bdi_firsts, bdi_lasts, bdi_frees);

    } else {
      char x[MESSAGE_LENGTH];
      snprintf(x, MESSAGE_LENGTH, "createDAQRdsStats failed");
      throwDAQStatsException(env, x, error);
      return NULL;
    }
}

jobject createDAQDriverStats(JNIEnv* env, const DAQ::Location& location, 
                             RMS::Client* client, bool clear) {
    DAQ::InterfaceDriverStats driverStats;
    int32_t error;
    if (client->stats(location, clear, driverStats, error)) {

        jbyte bay        = (jbyte) location.bay();
        jbyte board      = (jbyte) location.board();        
        jlong received   = (jlong) driverStats.received()       ;
        jlong errors     = (jlong) driverStats.errors()     ;
        jlong rxVC0      = (jlong) driverStats.rx(0);
        jlong rxVC1      = (jlong) driverStats.rx(1);
        jlong rxVC2      = (jlong) driverStats.rx(2);
        jlong rxVC3      = (jlong) driverStats.rx(3);
        
        return env->NewObject(JCdaqDriverStatsClass, JCdaqDriverStatsConstructor, bay, board, received, errors, rxVC0, rxVC1, rxVC2, rxVC3);

    } else {
      char x[MESSAGE_LENGTH];
      snprintf(x, MESSAGE_LENGTH, "createDAQDriverStats failed");
      throwDAQStatsException(env, x, error);
      return NULL;      
    }
}

jobject createDAQFirmwareStats(JNIEnv* env, const DAQ::Location& location, 
                               RMS::Client* client, bool clear) {
    DAQ::InterfaceFirmwareStats firmwareStats;
    int32_t error;
    if (client->stats(location, clear, firmwareStats, error)) {

        jbyte bay        = (jbyte) location.bay();
        jbyte board      = (jbyte) location.board();        
        jlong link_status      = (jlong) firmwareStats.link_status()     ;
        jlong rlink_data       = (jlong) firmwareStats.rlink_data()      ;
        jlong cell_errors      = (jlong) firmwareStats.cell_errors()     ;
        jlong link_down        = (jlong) firmwareStats.link_down()       ;
        jlong link_errors      = (jlong) firmwareStats.link_errors()     ;
        jlong overflow_remote0 = (jlong) firmwareStats.overflow_remote(0);
        jlong overflow_remote1 = (jlong) firmwareStats.overflow_remote(1);
        jlong overflow_remote2 = (jlong) firmwareStats.overflow_remote(2);
        jlong overflow_remote3 = (jlong) firmwareStats.overflow_remote(3);
        jlong rx_errors        = (jlong) firmwareStats.rx_errors()       ;
        jlong rx_frames        = (jlong) firmwareStats.rx_frames()       ;
        jlong overflow_local0  = (jlong) firmwareStats.overflow_local(0) ;
        jlong overflow_local1  = (jlong) firmwareStats.overflow_local(1) ;
        jlong overflow_local2  = (jlong) firmwareStats.overflow_local(2) ;
        jlong overflow_local3  = (jlong) firmwareStats.overflow_local(3) ;
        jlong tx_errors        = (jlong) firmwareStats.tx_errors()       ;
        jlong tx_frames        = (jlong) firmwareStats.tx_frames()       ;
        jlong rx_clock         = (jlong) firmwareStats.rx_clock()        ;
        jlong tx_clock         = (jlong) firmwareStats.tx_clock()        ;
        jlong last_tx          = (jlong) firmwareStats.last_tx()         ;
        jlong last_rx          = (jlong) firmwareStats.last_rx()         ;
        jlong tx_op            = (jlong) firmwareStats.tx_op()           ;
        jlong rx_op            = (jlong) firmwareStats.rx_op()           ;

        return env->NewObject(JCdaqFirmwareStatsClass, JCdaqFirmwareStatsConstructor, bay, board, link_status, rlink_data, cell_errors, link_down, link_errors, overflow_remote0, overflow_remote1, overflow_remote2, overflow_remote3, rx_errors, rx_frames, overflow_local0, overflow_local1, overflow_local2, overflow_local3, tx_errors, tx_frames, rx_clock, tx_clock, last_tx, last_rx, tx_op, rx_op);

    } else {
      char x[MESSAGE_LENGTH];
      snprintf(x, MESSAGE_LENGTH, "createDAQFirmwareStats failed");
      throwDAQStatsException(env, x, error);
      return NULL;
    }
}

//static jint JNIVERSION = JNI_VERSION_1_8;

/*  This function is meant to be called from a top-level (or global) 
 *  OnLoad function, which in turn checks and returns JNI_VERSION.
 */

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

    JCdaqFirmwareStatsConstructor = env->GetMethodID(JCdaqFirmwareStatsClass, "<init>", "(BBJJJJJJJJJJJJJJJJJJJJJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    jclass daqDriverStatsClass = env->FindClass("org/lsst/ccs/daq/ims/DAQDriverStats");
    if (env->ExceptionCheck()) {
         return;
    }
    JCdaqDriverStatsClass = (jclass) env->NewGlobalRef(daqDriverStatsClass);

    JCdaqDriverStatsConstructor = env->GetMethodID(JCdaqDriverStatsClass, "<init>", "(BBJJJJJJ)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}

/*  JNI methods invoked in Java code to create Java statistics classes */

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Stats_getRmsStats
(JNIEnv *env, jobject obj, jlong client, jint elementIndex, jboolean clear) {
    RMS::Client* client_ = (RMS::Client*)client;
    DAQ::Location element(elementIndex);
    return createDAQRmsStats(env, element, client_, (bool)(clear!=JNI_FALSE));
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Stats_getRdsStats
(JNIEnv *env, jobject obj, jlong client, jint elementIndex, jboolean clear) {
    RDS::Client* client_ = (RDS::Client*)client;
    DAQ::Location element(elementIndex);
    return createDAQRdsStats(env, element, client_, (bool)(clear!=JNI_FALSE));
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Stats_getDriverStats
(JNIEnv *env, jobject obj, jlong client, jint elementIndex, jboolean clear) {
    RMS::Client* client_ = (RMS::Client*)client;
    DAQ::Location element(elementIndex);
    return createDAQDriverStats(env, element, client_, (bool)(clear!=JNI_FALSE));
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Stats_getFirmwareStats
(JNIEnv *env, jobject obj, jlong client, jint elementIndex, jboolean clear) {
    RMS::Client* client_ = (RMS::Client*)client;
    DAQ::Location element(elementIndex);
    return createDAQFirmwareStats(env, element, client_, (bool)(clear!=JNI_FALSE));
}


/*  JNI methods to attach or detach the two clients.  It is up to the
 *  Java code to save the client pointers, and use the proper client
 *  in JNI methods beyond attach.
 */

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_Stats_attachRmsClient
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

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_Stats_attachRdsClient
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        RDS::Client* client = new RDS::Client(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) client;
    } catch (DSM::Exception& x) {
        env->ReleaseStringUTFChars(partition, partition_name);
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Stats_detachRmsClient
(JNIEnv* env, jobject obj, jlong client) {
  delete ((RMS::Client*) client);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Stats_detachRdsClient
(JNIEnv* env, jobject obj, jlong client) {
  delete ((RDS::Client*) client);
}
