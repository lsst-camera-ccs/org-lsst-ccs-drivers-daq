#include <stdio.h>

#include "MyProcessor.h"

#include "ims/Image.hh"
#include "ims/ImageMetadata.hh"
#include "daq/LocationSet.hh"

using namespace IMS;

MyProcessor::MyProcessor(Store& store, JNIEnv *env, jobject result) :
IMS::Processor(), store(store), env(env), result(result) {

}

void MyProcessor::process(const Id& id) {
    Image image(id, store);
    if (!image) return;
    addObjectToList(env, result, createImageMetaData(env, image));
}

