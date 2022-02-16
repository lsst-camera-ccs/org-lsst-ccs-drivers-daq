#ifndef CCS_GUIDER_SUBSCRIBER
#define CCS_GUIDER_SUBSCRIBER

#include <jni.h>

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"

void Guider_OnLoad(JNIEnv*);

class MyGuiderSubscriber : public GDS::Subscriber {
public:
    MyGuiderSubscriber(const char* partition, const GDS::LocationSet& locs);
public:
    ~MyGuiderSubscriber() {}
public:
  void start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series);
  void resume(const GDS::StateMetadata& state);
  void pause(const GDS::StateMetadata& state);
  void stop(const GDS::StateMetadata& state);
  void stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp);
  void wait(JNIEnv *env, jobject callback);
private:
    JNIEnv *env;
    jobject callback;
    jobject createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state);
    jobject createSeriesMetadata(JNIEnv* env, const GDS::SeriesMetadata& series);
    jobject createByteBuffer(JNIEnv* env, const GDS::RawStamp& stamp);
};

#endif