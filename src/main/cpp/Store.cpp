#include <jni.h>

#include "org_lsst_ccs_daq_imageapi_Store.h"
#include "ims/Store.hh"
#include "ims/Catalog.hh"
#include "ims/Folder.hh"
#include "ims/Image.hh"
#include "dsi/LocationSet.hh"
#include "dsm/Exception.hh"
#include "xds/Page.hh"

#include "MyFolders.h"
#include "MyProcessor.h"
#include "MyInspector.h"

using namespace IMS;

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_imageapi_Store_attachStore
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        Store* store = new Store(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) store;
    } catch (DSM::Exception& x) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        return env->ThrowNew(exClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_detachStore
(JNIEnv* env, jobject obj, jlong store) {
    delete ((Store*) store);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_imageapi_Store_capacity
(JNIEnv * env, jobject obj, jlong store) {

    DSI::LocationSet missing;
    return ((Store *) store)->capacity(missing) << XDS::Page::SIZE2;
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_imageapi_Store_remaining
(JNIEnv * env, jobject obj, jlong store) {

    DSI::LocationSet missing;
    return ((Store *) store)->remaining(missing) << XDS::Page::SIZE2;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_listFolders
(JNIEnv *env, jobject obj, jlong store, jobject result) {

    Catalog& catalog = ((Store *) store)->catalog;
    MyFolders folders(catalog, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    folders.traverse();
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_imageapi_Store_insertFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    jint rc = catalog.insert(folder_name);
    env->ReleaseStringUTFChars(name, folder_name);
    return rc;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_imageapi_Store_removeFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    jint rc = catalog.remove(folder_name);
    env->ReleaseStringUTFChars(name, folder_name);
    return rc;
}

JNIEXPORT jboolean JNICALL Java_org_lsst_ccs_daq_imageapi_Store_findFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    //TODO: Implementation?
    return 1;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_listImages
(JNIEnv *env, jobject obj, jlong store, jstring folderName, jobject result) {
    Store* store_ = (Store*) store;
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    Folder folder(folder_name, store_->catalog);
    MyProcessor processor(*store_, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    folder.traverse(processor);
    env->ReleaseStringUTFChars(folderName, folder_name);
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_imageapi_Store_moveImageToFolder
(JNIEnv *env, jobject obj, jlong store, jlong id, jstring folderName) {
    Store* store_ = (Store*) store;
    Id id_(id);
    Image image_(id_, *store_);
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    int rc = image_.moveTo(folder_name);
    env->ReleaseStringUTFChars(folderName, folder_name);
    return rc;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_imageapi_Store_deleteImage
(JNIEnv *env, jobject obj, jlong store, jlong id) {
    Store* store_ = (Store*) store;
    Id id_(id);
    Image image_(id_, *store_);
    return image_.remove();
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_listSources
  (JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jobject result) {
    Store* store_ = (Store*) store;
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    const char *image_name = env->GetStringUTFChars(imageName, 0);
    Id id_ =  store_->catalog.lookup(image_name, folder_name);
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(folderName, image_name);
    Image image_(id_, *store_);
    if (!image_) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        env->ThrowNew(exClass, "Invalid image");        
    }
    MyInspector inspector(image_, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    image_.synopsis(0);
    printf("Calling run %s\n",image_.metadata().name());
    inspector.run();
}

