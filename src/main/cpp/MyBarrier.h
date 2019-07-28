#ifndef CCS_BARRIER
#define CCS_BARRIER

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Barrier.hh"

#include "Store.h"

class MyBarrier : public IMS::Decoder {
public:
    MyBarrier(IMS::Store&, JNIEnv *env, IMS::Image& image, jobject obj, jmethodID callback);
public:

    ~MyBarrier() {}
public:
  void process(IMS::Science::Source&,   uint64_t length, uint64_t offset);
  void process(IMS::Guiding::Source&,   uint64_t length, uint64_t offset);
  void process(IMS::Wavefront::Source&, uint64_t length, uint64_t offset);
private:
    IMS::Store& store;
    jobject obj;
    JNIEnv *env;
    jmethodID callback;
};

#endif