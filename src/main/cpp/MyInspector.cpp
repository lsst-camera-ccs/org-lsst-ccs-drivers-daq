#include <stdio.h>

#include "MyInspector.h"
#include "Store.h"

#include "ims/Image.hh"
#include "dvi/Version.hh"


using namespace IMS;

MyInspector::MyInspector(Image& image, JNIEnv *env, jobject result) :
IMS::Decoder(image), env(env), result(result) {
    sourceMetaDataClass = env->FindClass("org/lsst/ccs/daq/imageapi/SourceMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    sourceMetaDataConstructor = env->GetMethodID(sourceMetaDataClass, "<init>", "(BBLjava/lang/String;Lorg/lsst/ccs/daq/imageapi/Version;IJIBB)V");
    if (env->ExceptionCheck()) {
        return;
    }
}

void MyInspector::process(IMS::Science::Source& source, uint64_t length, uint64_t base) {
    source.synopsis(0);
    this->process(source, length);
    char* _data = new char[length];
    uint32_t err = source.read(_data,length);
    printf("bytesRead=%d error=%d\n",length,err);
    delete _data;
}

void MyInspector::process(IMS::Guiding::Source& source, uint64_t length, uint64_t base) {
    source.synopsis(0);
    this->process(source, length);
}

void MyInspector::process(IMS::Wavefront::Source& source, uint64_t length, uint64_t base) {
    source.synopsis(0);
    this->process(source, length);
}

void MyInspector::process(const IMS::Source& source, uint64_t length) {
    
    SourceMetadata metaData = source.metadata();
    jbyte sensor = metaData.sensor();
    jbyte lane = metaData.lane();
    jstring platform = env->NewStringUTF(metaData.platform());
    jobject version_ = createVersion(env, metaData.software());
    jint firmware = metaData.firmware();
    jlong serialNumber = metaData.serial_number();
    jbyte bay = source.location().bay();
    jbyte board = source.location().board();
    jobject metaData_ = env->NewObject(sourceMetaDataClass, sourceMetaDataConstructor, sensor, lane, platform, version_, firmware, serialNumber, length, bay, board);
    addObjectToList(env, result, metaData_);    
}