#ifndef CCS_PROCESSOR
#define CCS_PROCESSOR

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Processor.hh"

#include "Store.h"
class MyProcessor : public IMS::Processor {
public:
    MyProcessor(IMS::Store&, JNIEnv *env, jobject result);
public:

    ~MyProcessor() {}
public:
    void process(const IMS::Id&);

private:
    IMS::Store& store;
    jobject result;
    JNIEnv *env;
    jmethodID imageMetaDataConstructor;
    jclass imageMetaDataClass;
};

#endif