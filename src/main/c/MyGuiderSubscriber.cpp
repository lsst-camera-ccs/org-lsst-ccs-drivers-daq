#include <stdio.h>

#include "MyGuiderSubscriber.h"

using namespace GDS;

MyGuiderSubscriber::MyGuiderSubscriber(JNIEnv *env, jobject callback, const char* partition, const GDS::LocationSet& locs, 
    jmethodID startCallback, jmethodID resumeCallback, jmethodID pauseCallback, jmethodID stopCallback, jmethodID stampCallback, jclass guiderStateMetadataClass, jmethodID guiderStateMetadataConstructor) :
    env(env), callback(callback), GDS::Subscriber(partition, locs), startCallback(startCallback), resumeCallback(resumeCallback), pauseCallback(pauseCallback), stopCallback(stopCallback), stampCallback(stampCallback),
    guiderStateMetadataClass(guiderStateMetadataClass), guiderStateMetadataConstructor(guiderStateMetadataConstructor)  {
}

jobject MyGuiderSubscriber::createStateMetadata(JNIEnv* env, const GDS::StateMetadata& state) {
    jint type = state.type();
    jint status = state.status();
    jint sequence = state.sequence();
    jlong timestamp = state.timestamp();
    return env->NewObject(guiderStateMetadataClass, guiderStateMetadataConstructor, type, status, sequence, timestamp);
}

void MyGuiderSubscriber::start(const GDS::StateMetadata& state, const GDS::SeriesMetadata& series) {
    env->CallVoidMethod(callback, startCallback, createStateMetadata(env, state));
}

void MyGuiderSubscriber::resume(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, resumeCallback, createStateMetadata(env, state));
}

void MyGuiderSubscriber::pause(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, pauseCallback, createStateMetadata(env, state));
}

void MyGuiderSubscriber::stop(const GDS::StateMetadata& state) {
    env->CallVoidMethod(callback, stopCallback, createStateMetadata(env, state));
}

void MyGuiderSubscriber::stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp) {
    env->CallVoidMethod(callback, stampCallback, createStateMetadata(env, state));
}

