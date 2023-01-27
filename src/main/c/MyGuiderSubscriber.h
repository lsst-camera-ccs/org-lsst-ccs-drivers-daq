#ifndef CCS_GUIDER_SUBSCRIBER
#define CCS_GUIDER_SUBSCRIBER

#include <jni.h>

#include "gds/Decoder.hh"
#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"
#include "gds/Stamp.hh"
#include "gds/Series.hh"
#include "gds/Status.hh"

extern jobject createGuiderConfig(JNIEnv* env, const GDS::Status& status, const GDS::Series& series, const GDS::RoiCommon& common, const GDS::RoiLocation* location, int nLocations);
extern jobject createGuiderSeries(JNIEnv* env, const GDS::Status& status, const GDS::Series& series);
extern jobject createGuiderStatus(JNIEnv* env, const GDS::Status& status);

void Guider_OnLoad(JNIEnv*);

class MyGuiderSubscriber : public GDS::Decoder {
public:
    MyGuiderSubscriber(const char* partition, const GDS::LocationSet& locs);
public:
    ~MyGuiderSubscriber() {}
public:
  void start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series);
  void resume(const GDS::StateMetadata& state);
  void pause(const GDS::StateMetadata& state);
  void stop(const GDS::StateMetadata& state);
  void raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp);
  void wait(JNIEnv *env, jobject callback);
  void stamp(const GDS::StateMetadata& state, const GDS::Stamp& stamp);
  uint8_t* allocate(unsigned size);
private:
    JNIEnv *env;
    jobject callback;
    uint8_t _stamp_buf[400*400*4];
    //jobject createRoiCommon(JNIEnv* env, const GDS::RoiCommon& location);
    //jobject createRoiLocation(JNIEnv* env, const GDS::RoiLocation& location);
    //jobject createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state);
    //jobject createSeriesMetadata(JNIEnv* env, const GDS::SeriesMetadata& series);
    jobject createByteBuffer(JNIEnv* env, const GDS::RawStamp& stamp);
    jobject createByteBuffer(JNIEnv* env, const GDS::Stamp& stamp);
};

#endif