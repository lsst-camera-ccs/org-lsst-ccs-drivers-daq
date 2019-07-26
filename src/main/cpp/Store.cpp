#include "Store.h"

#include "org_lsst_ccs_daq_imageapi_Store.h"
#include "ims/Store.hh"
#include "ims/Catalog.hh"
#include "ims/Folder.hh"
#include "ims/Image.hh"
#include "ims/Source.hh"
#include "ims/SourceMetadata.hh"
#include "daq/LocationSet.hh"
#include "dsm/Exception.hh"
#include "xds/Page.hh"

#include "MyFolders.h"
#include "MyProcessor.h"

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
static jclass JCsourceMetaDataClass;
static jmethodID JCsourceMetaDataConstructor;

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
            printf("Setting bit %d\n",index);
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

jobject createSourceMetaData(JNIEnv* env, const Source& source) {
    const SourceMetadata metaData = source.metadata();
    jbyte sensor = metaData.sensor();
    jbyte lane = metaData.lane();
    jstring platform = env->NewStringUTF(metaData.platform());
    jobject version_ = createVersion(env, metaData.software());
    jint firmware = metaData.firmware();
    jlong serialNumber = metaData.serial_number();
    jbyte bay = source.location().bay();
    jbyte board = source.location().board();

    const RMS::InstructionList& il = metaData.instructions();
    int size = il.size();
    jint register_values[size];
    for (int i = 0; i < size; i++) {
        register_values[i] = il.lookup(i)->reg();
    }
    jintArray registerValues = env->NewIntArray(size);
    env->SetIntArrayRegion(registerValues, 0, size, register_values);
    return env->NewObject(JCsourceMetaDataClass, JCsourceMetaDataConstructor, sensor, lane, platform, version_, firmware, serialNumber, source.size(), bay, board, registerValues);
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
        sprintf(x, "Find image %s in folder %s failed (error=%d)", image_name, folder_name, image_.error());
        env->ThrowNew(exClass, x);
    }
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    return image_;
}

Image findImage(JNIEnv* env, Store* store_, uint64_t id) {
    Id id_(id);
    Image image_(id_, *store_);
    if (!image_) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "Find image id %d failed (error=%d)", id, image_.error());
        env->ThrowNew(exClass, x);
    }
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

    jclass sourceMetaDataClass = env->FindClass("org/lsst/ccs/daq/imageapi/SourceMetaData");
    if (env->ExceptionCheck()) {
        return JNI_ERR;
    }
    JCsourceMetaDataClass = (jclass) env->NewGlobalRef(sourceMetaDataClass);

    JCsourceMetaDataConstructor = env->GetMethodID(JCsourceMetaDataClass, "<init>", "(BBLjava/lang/String;Lorg/lsst/ccs/daq/imageapi/Version;IJIBB[I)V");
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
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    Folder folder(folder_name,catalog);
    if (!folder) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "No such folder %s (error=%d)", folder_name, folder.error());
        env->ThrowNew(exClass, x);        
    }
    env->ReleaseStringUTFChars(name, folder_name);
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
    Image image_ = findImage(env, store_, id);
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    int rc = image_.moveTo(folder_name);
    env->ReleaseStringUTFChars(folderName, folder_name);
    return rc;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_imageapi_Store_deleteImage
(JNIEnv *env, jobject obj, jlong store, jlong id) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, id);
    return image_.remove();
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_imageapi_Store_listSources
(JNIEnv *env, jobject obj, jlong store, jlong id, jobject result) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, id);
    DAQ::LocationSet locations_ = image_.metadata().elements();
    DAQ::Location element;

    while (locations_.remove(element)) {
        Source source(image_.id(), element, *store_);
        jobject metaData_ = createSourceMetaData(env, source);
        addObjectToList(env, result, metaData_);
    }
}


JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_imageapi_Store_addImageToFolder
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jstring annotation, jint opcode, jobject locations) {
    Store* store_ = (Store*) store;
    const char *image_name = env->GetStringUTFChars(imageName, 0);
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    const char *annotation_ = env->GetStringUTFChars(annotation, 0);

    DAQ::LocationSet locations_ = convertLocations(env, locations);
    ImageMetadata meta(image_name, locations_, opcode, annotation_);
    Image image(folder_name, meta, *store_);
    if (!image) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "Creating image %s in folder %s failed (error=%d)", image_name, folder_name, image.error());
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

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_imageapi_Store_openSourceChannel
  (JNIEnv *env, jobject obj, jlong store, jlong id, jint elementIndex, jboolean write) {
    Store* store_ = (Store*) store;
    Image image = findImage(env, store_, id);
    DAQ::Location element(elementIndex);
    Source* source = new Source(image.id(), element, *store_);
    if (!*source) {
        jclass exClass = env->FindClass("org/lsst/ccs/daq/imageapi/DAQException");
        char x[256];
        sprintf(x, "Source not found (error=%d)", source->error());
        env->ThrowNew(exClass, x);
        delete source;
    }    
    return (jlong) source;
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_imageapi_Store_addSourceToImage
  (JNIEnv *env, jobject obj, jlong store, jlong id, jint elementIndex, jbyte type, jstring platform, jintArray registerValues) {
    Store* store_ = (Store*) store;
    Image image = findImage(env, store_, id);
    DAQ::Location element(elementIndex);    
    const char *platform_ = env->GetStringUTFChars(platform, 0);
    SourceMetadata smd((DAQ::Sensor::Type) type, DAQ::Lane::Type::EMULATION, platform_);
    RMS::InstructionList il;
    jint size = env->GetArrayLength(registerValues);
    jint* values = env->GetIntArrayElements(registerValues, 0);
    for (uint32_t i = 0; i < size; i++) {
        il.insert(RMS::Instruction::Opcode::GET, values[i]);
    }
    for (uint32_t i = size; i < il.size(); i++) {
        il.insert(RMS::Instruction::Opcode::GET, 0);
    }
    smd = il;
    env->ReleaseIntArrayElements(registerValues, values, JNI_ABORT);
    env->ReleaseStringUTFChars(platform, platform_);
    Source source(image.id(), element, *store_, smd);
    return createSourceMetaData(env, source);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_imageapi_Store_waitForImage
  (JNIEnv *env, jobject obj, jlong store) {
    Store* store_ = (Store*) store;
    Stream stream(*store_);
    Image image(*store_, stream);
    if (!image) return NULL;
    return createImageMetaData(env, image);
}
