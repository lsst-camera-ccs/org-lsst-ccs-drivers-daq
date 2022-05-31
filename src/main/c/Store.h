#ifndef CCS_STORE
#define CCS_STORE

#include <jni.h>

#include "dvi/Version.hh"
#include "daq/LocationSet.hh"
#include "ims/Image.hh"

/**************************************************************
 * Static Global Variables to cache Java Class and Method IDs
 **************************************************************/
extern jobject createVersion(JNIEnv*, DVI::Version);
extern jobject createBitSet(JNIEnv* env, DAQ::LocationSet elements);
extern void addObjectToList(JNIEnv* env, jobject list, jobject item);
extern jobject createImageMetaData(JNIEnv*, IMS::Image&);

static jclass JClistClass;
static jmethodID JClistAddMethodID;
static jclass JCarrayListClass;
static jmethodID JCarrayListConstructor;
#endif