#include <stdio.h>

#include "org_lsst_ccs_daq_ims_DAQSourceChannelNativeImplementation.h"
#include "Channel.h"
#include "ims/Source.hh"

using namespace IMS;

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_DAQSourceChannelNativeImplementation_read
(JNIEnv *env, jobject obj, jlong source_, jobject buffer, jint position, jlong offset, jint length) {
    Source* source = (Source*) source_;
    char *buf = (char*) env->GetDirectBufferAddress(buffer);
    return source->read(buf + position, length, offset);
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_DAQSourceChannelNativeImplementation_write
(JNIEnv *env, jobject obj, jlong source_, jobject buffer, jint position, jint length) {
    Source* source = (Source*) source_;
    char *buf = (char*) env->GetDirectBufferAddress(buffer);
    return source->write(buf + position, length);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_DAQSourceChannelNativeImplementation_close
(JNIEnv *rnv, jobject obj, jlong source_, jboolean write) {
    Source* source = (Source*) source_;
    //if (write) {
    //    source->terminate();
    //}
    delete source;
}