#ifndef CCS_READER
#define CCS_READER

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Decoder.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"
#include "ims/SourceMetadata.hh"

#include <jni.h>

class MyReader : public IMS::Decoder {

public:
  MyReader(IMS::Image&, JNIEnv *env, DAQ::LocationSet& filter, jobject* buffers);
public:
 ~MyReader() {} 
public:
  void process(IMS::Science::Source&,   uint64_t length, uint64_t base);
  void process(IMS::Guiding::Source&,   uint64_t length, uint64_t base);
  void process(IMS::Wavefront::Source&,   uint64_t length, uint64_t base);
  
private:
  void read(IMS::Source&, uint64_t length, uint64_t base);
  
private:
  jobject result;
  JNIEnv *env;
  jobject* buffers;
};

#endif