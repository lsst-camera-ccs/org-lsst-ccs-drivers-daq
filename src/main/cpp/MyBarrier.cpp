#include <stdio.h>

#include "MyBarrier.h"

#include "ims/Image.hh"
#include "ims/ImageMetadata.hh"
#include "daq/LocationSet.hh"

using namespace IMS;

MyBarrier::MyBarrier(IMS::Store&, JNIEnv *env, IMS::Image& image, jobject obj, jmethodID callback) :
IMS::Decoder(image), store(store), env(env), obj(obj), callback(callback) {

}

void MyBarrier::process(Science::Source& source, uint64_t length, uint64_t offset) {
    env->CallVoidMethod(obj, callback, _image.id().value(), source.location().index(), offset+length);
}

void MyBarrier::process(Guiding::Source& source, uint64_t length, uint64_t offset) {
    env->CallVoidMethod(obj, callback, _image.id().value(), source.location().index(), offset+length);
}

void MyBarrier::process(Wavefront::Source& source, uint64_t length, uint64_t offset) {
    env->CallVoidMethod(obj, callback, _image.id().value(), source.location().index(), offset+length);
}
