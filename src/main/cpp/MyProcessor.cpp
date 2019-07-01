#include <stdio.h>

#include "MyProcessor.h"

#include "ims/Image.hh"
#include "ims/ImageMetadata.hh"


using namespace IMS;

MyProcessor::MyProcessor(Store& store, JNIEnv *env, jobject result) :
IMS::Processor(), store(store), env(env), result(result) {
    jclass cls = env->GetObjectClass(result);
    if (env->ExceptionCheck()) {
        return;
    }
    mid = env->GetMethodID(cls, "add", "(Ljava/lang/Object;)Z");
    if (env->ExceptionCheck()) {
        return;
    }
    imageMetaDataClass = env->FindClass("org/lsst/ccs/daq/imageapi/ImageMetaData");
    if (env->ExceptionCheck()) {
        return;
    }
    imageMetaDataConstructor = env->GetMethodID(imageMetaDataClass, "<init>", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;IJ)V");
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
    //TODO: Deal with location and other release info

    jobject jimage = env->NewObject(imageMetaDataClass, imageMetaDataConstructor, id_, name, annotation, release, opcode, timestamp);
    env->CallVoidMethod(result, mid, jimage);
}

