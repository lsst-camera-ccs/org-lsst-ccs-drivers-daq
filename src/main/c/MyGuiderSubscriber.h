#ifndef CCS_GUIDER_SUBSCRIBER
#define CCS_GUIDER_SUBSCRIBER

#include <jni.h>

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"

class MyGuiderSubscriber : public GDS::Subscriber {
public:
    MyGuiderSubscriber(JNIEnv *env, jobject callback, const char* partition, const GDS::LocationSet& locs, jmethodID startCallback, jmethodID resumeCallback, jmethodID pauseCallback, jmethodID stopCallback, jmethodID stampCallback,
        jclass guiderStateMetadataClass, jmethodID guiderStateMetadataConstructor);
public:
    ~MyGuiderSubscriber() {}
public:
  void start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series);
  void resume(const GDS::StateMetadata& state);
  void pause(const GDS::StateMetadata& state);
  void stop(const GDS::StateMetadata& state);
  void stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp);
private:
    JNIEnv *env;
    jobject callback;
    jmethodID startCallback;
    jmethodID resumeCallback;
    jmethodID pauseCallback;
    jmethodID stopCallback;
    jmethodID stampCallback;
    jclass guiderStateMetadataClass;
    jmethodID guiderStateMetadataConstructor;

    jobject createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state);
};

#endif