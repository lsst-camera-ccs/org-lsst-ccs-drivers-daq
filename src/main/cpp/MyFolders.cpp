#include <stdio.h>

#include "MyFolders.h"
#include "Store.h"

using namespace IMS;

MyFolders::MyFolders(Catalog& catalog, JNIEnv *env, jobject result) :
IMS::Folders(catalog), env(env), result(result) {

}

void MyFolders::process(const char* name) {
    addObjectToList(env, result, env->NewStringUTF(name));
}

