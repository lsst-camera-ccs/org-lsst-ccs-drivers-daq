#include "Store.h"

#include "org_lsst_ccs_daq_imageapi_Store.h"
#include "ims/Store.hh"
#include "ims/Catalog.hh"
#include "ims/Folder.hh"
#include "ims/Image.hh"
#include "daq/LocationSet.hh"
#include "dsm/Exception.hh"
#include "xds/Page.hh"

#include "MyFolders.h"
#include "MyProcessor.h"
#include "MyInspector.h"
#include "MyReader.h"

using namespace IMS;

static jclass JCversionClass;
static jmethodID JCversionConstructor;
static jclass JCbitSetClass;
static jmethodID JCbitSetConstructor;
static jmethodID JCbitSetSetMethodId;
static jmethodID JCbitSetGetMethodId;
static jclass JClistClass;
static jmethodID JClistAddMethodID;
static jclass JCimageMetaDataClass;
static jmethodID JCimageMetaDataConstructor;

jobject createVersion(JNIEnv* env, DVI::Version version) {
    jstring tag = env->NewStringUTF(version.tag());
    jboolean dirty = version.dirty();
    jint hash = version.hash();
    jlong buildTime = version.buildTime;
    return env->NewObject(JCversionClass, JCversionConstructor, tag, buildTime, dirty, hash);
}

jobject createBitSet(JNIEnv* env, DAQ::LocationSet elements) {
    int size = elements.SIZE;
    jobject bitset = env->NewObject(JCbitSetClass, JCbitSetConstructor);
    for (uint8_t index = 0; index < size; index++) {
        if (elements.has(index)) {
            env->CallVoidMethod(bitset, JCbitSetSetMethodId, (jint) index);
        }
    }
    return bitset;
}

DAQ::LocationSet convertLocations(JNIEnv* env, jobject bitset) {
    DAQ::LocationSet locations;
    int size = locations.SIZE;
    for (uint8_t index = 0; index < size; index++) {
        if (env->CallBooleanMethod(bitset, JCbitSetGetMethodId, (jint) index)) {
           locations.insert(index);
        }
    }
    return locations;    
}

jobject createImageMetaData(JNIEnv* env, Image& image) {
    const ImageMetadata& metaData = image.metadata();
    jstring name = env->NewStringUTF(metaData.name());
    jstring annotation = env->NewStringUTF(metaData.annotation());
    jint opcode = metaData.opcode();
    jlong timestamp = metaData.timestamp();
    jlong id_ = image.id().value();
    jobject bitset = createBitSet(env, metaData.elements());
    jobject version_ = createVersion(env, metaData.release());
    return env->NewObject(JCimageMetaDataClass, JCimageMetaDataConstructor, id_, name, annotation, version_, opcode, timestamp, bitset);
}

void addObjectToList(JNIEnv* env, jobject list, jobject item) {
    env->CallVoidMethod(list, JClistAddMethodID, item);
}

Image findImage(JNIEnv* env, Store* store_, jstring imageName, jstring folderName) {
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    const char *image_name = env->GetStringUTFChars(imageName, 0);
    Id id_ = store_->catalog.lookup(image_name, folder_name);
    Image image_(id_, *store_);
    if (!image_) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "Find image %s in folder %s failed (error=%d)", image_name, folder_name, image_.error() );
        env->ThrowNew(exClass, x);
    }
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    return image_;
}

static jint JNI_VERSION = JNI_VERSION_1_8;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    printf("OnLoad\n");

    // Obtain the JNIEnv from the VM and confirm JNI_VERSION
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**> (&env), JNI_VERSION) != JNI_OK) {
        return JNI_ERR;
    }

    jclass versionClass = env->FindClass("org/lsst/ccs/daq/imageapi/Version");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCversionClass = (jclass) env->NewGlobalRef(versionClass);

    JCversionConstructor = env->GetMethodID(JCversionClass, "<init>", "(Ljava/lang/String;JZI)V");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }

    jclass bitSetClass = env->FindClass("java/util/BitSet");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCbitSetClass = (jclass) env->NewGlobalRef(bitSetClass);

    JCbitSetConstructor = env->GetMethodID(JCbitSetClass, "<init>", "()V");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCbitSetSetMethodId = env->GetMethodID(JCbitSetClass, "set", "(I)V");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCbitSetGetMethodId = env->GetMethodID(JCbitSetClass, "get", "(I)Z");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    jclass listClass = env->FindClass("java/util/List");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JClistClass = (jclass) env->NewGlobalRef(listClass);

    JClistAddMethodID = env->GetMethodID(JClistClass, "add", "(Ljava/lang/Object;)Z");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    
    jclass imageMetaDataClass = env->FindClass("org/lsst/ccs/daq/imageapi/ImageMetaData");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCimageMetaDataClass = (jclass) env->NewGlobalRef(imageMetaDataClass);
    
    JCimageMetaDataConstructor = env->GetMethodID(imageMetaDataClass, "<init>", "(JLjava/lang/String;Ljava/lang/String;Lorg/lsst/ccs/daq/imageapi/Version;IJLjava/util/BitSet;)V");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    

    // Return the JNI Version as required by method
    return JNI_VERSION;
}

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
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, imageName, folderName);
    return image_.remove();
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_listSources
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jobject result) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, imageName, folderName);
    MyInspector inspector(image_, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    inspector.run();
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_readRawImage
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jobjectArray byteBufferList) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, imageName, folderName);
    jobject buffers[128];
    DAQ::LocationSet filter;
    for (uint8_t index=0; index<env->GetArrayLength(byteBufferList); index++) {
        jobject byteBuffer = env->GetObjectArrayElement(byteBufferList, index);
        if (byteBuffer) {
            filter.insert(index);
            buffers[index] = byteBuffer;
        }
    }
    MyReader reader(image_, env, filter, buffers);
    reader.run();
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_imageapi_Store_addImageToFolder
  (JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jstring annotation, jint opcode, jobject locations) {
    Store* store_ = (Store*) store;
    const char *image_name = env->GetStringUTFChars(imageName, 0);
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    const char *annotation_ = env->GetStringUTFChars(annotation, 0);

    ImageMetadata meta(image_name, convertLocations(env, locations), opcode, annotation_); 
    Image image(folder_name, meta, *store_);
    if (!image) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "Creating image %s in folder %s failed (error=%d)", image_name, folder_name, image.error() );
        env->ThrowNew(exClass, x);
    }
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    env->ReleaseStringUTFChars(annotation, annotation_);
    return createImageMetaData(env, image);
}


JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_imageapi_Store_findImage
  (JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName) {
    
    Store* store_ = (Store*) store;
    Image image = findImage(env, store_, imageName, folderName);
    if (!image) return 0;
    return createImageMetaData(env, image);
}

