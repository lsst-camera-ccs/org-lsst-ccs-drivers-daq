#ifndef CCS_FOLDERS
#define CCS_FOLDERS

#include "ims/Catalog.hh"
#include "ims/Folders.hh"

#include <jni.h>

class MyFolders : public IMS::Folders {
public:
    MyFolders(IMS::Catalog&, JNIEnv *env, jobject result);
public:

    ~MyFolders() {
    }
public:
    void process(const char* name);

private:
    jobject result;
    JNIEnv *env;
    jmethodID mid;
};

#endif