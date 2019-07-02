#include <stdio.h>

#include "MyProcessor.h"

#include "ims/Image.hh"
#include "ims/ImageMetadata.hh"
#include "daq/LocationSet.hh"

using namespace IMS;

MyProcessor::MyProcessor(Store& store, JNIEnv *env, jobject result) :
IMS::Processor(), store(store), env(env), result(result) {
    imageMetaDataClass = env->FindClass("org/lsst/ccs/daq/imageapi/ImageMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    imageMetaDataConstructor = env->GetMethodID(imageMetaDataClass, "<init>", "(JLjava/lang/String;Ljava/lang/String;Lorg/lsst/ccs/daq/imageapi/Version;IJLjava/util/BitSet;)V");
    if (env->ExceptionCheck()) {
        return;
    }
}

void MyProcessor::process(const Id& id) {
    Image image(id, store);
    if (!image) return;
    const ImageMetadata& metaData = image.metadata();
    jstring name = env->NewStringUTF(metaData.name());
    jstring annotation = env->NewStringUTF(metaData.annotation());
    jint opcode = metaData.opcode();
    jlong timestamp = metaData.timestamp();
    jstring release = env->NewStringUTF(metaData.release().tag());
    jlong id_ = id.value();
    jobject bitset = createBitSet(env, metaData.elements());
    jobject version_ = createVersion(env, metaData.release());
    jobject jimage = env->NewObject(imageMetaDataClass, imageMetaDataConstructor, id_, name, annotation, version_, opcode, timestamp, bitset);
    addObjectToList(env, result, jimage);
}

