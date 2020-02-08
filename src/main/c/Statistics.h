#ifndef CCS_STATISTICS
#define CCS_STATISTICS

#include <jni.h>

#include "dvi/Version.hh"
#include "daq/LocationSet.hh"

void JNI_Stats_OnLoad(JNIEnv*);

/**************************************************************
 * Static Global Variables to cache Java Class and Method IDs
 **************************************************************/
extern jobject createVersion(JNIEnv*, DVI::Version);

#endif
