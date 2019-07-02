#ifndef CCS_INSPECTOR
#define CCS_INSPECTOR

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Decoder.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"
#include "ims/SourceMetadata.hh"

#include <jni.h>

class MyInspector : public IMS::Decoder {

public:
  MyInspector(IMS::Image&, JNIEnv *env, jobject result);
public:
 ~MyInspector() {} 
public:
  void process(IMS::Science::Source&,   uint64_t length, uint64_t base);
  void process(IMS::Guiding::Source&,   uint64_t length, uint64_t base);
  void process(IMS::Wavefront::Source&,   uint64_t length, uint64_t base);
  
private:
  void process(const IMS::Source&,   uint64_t length);
  
private:
  jobject result;
  JNIEnv *env;
  jmethodID sourceMetaDataConstructor;
  jclass sourceMetaDataClass;
};

#endif