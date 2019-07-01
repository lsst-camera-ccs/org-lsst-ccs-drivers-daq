#include <stdio.h>

#include "MyFolders.h"

using namespace IMS;

MyFolders::MyFolders(Catalog& catalog, JNIEnv *env, jobject result) :
IMS::Folders(catalog), env(env), result(result) {
    jclass cls = env->GetObjectClass(result);
    mid = env->GetMethodID(cls, "add", "(Ljava/lang/Object;)Z");
    if (env->ExceptionCheck()) {
        return;
    }
}

void MyFolders::process(const char* name) {
    env->CallVoidMethod(result, mid, env->NewStringUTF(name));
}

