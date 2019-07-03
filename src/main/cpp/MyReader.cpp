#include <stdio.h>

#include "MyReader.h"
#include "Store.h"

#include "ims/Image.hh"
#include "dvi/Version.hh"


using namespace IMS;

MyReader::MyReader(Image& image, JNIEnv *env, DAQ::LocationSet& filter, jobject* buffers) :
IMS::Decoder(image, filter), env(env), buffers(buffers) {
}

void MyReader::process(IMS::Science::Source& source, uint64_t length, uint64_t base) {
    this->read(source, length, base);
}

void MyReader::process(IMS::Guiding::Source& source, uint64_t length, uint64_t base) {
    this->read(source, length, base);
}

void MyReader::process(IMS::Wavefront::Source& source, uint64_t length, uint64_t base) {
    this->read(source, length, base);
}

void MyReader::read(IMS::Source& source, uint64_t length, uint64_t base) {
    uint8_t index = source.location().index();
    jobject buffer = buffers[index];
    char *buf = (char*) env->GetDirectBufferAddress(buffer);
    int32_t err = source.read(buf,length, base);
    jclass cls = env->GetObjectClass(buffer);
    jmethodID mid = env->GetMethodID(cls, "limit", "(I)Ljava/nio/Buffer;");
    env->CallObjectMethod(buffer, mid, length);
}