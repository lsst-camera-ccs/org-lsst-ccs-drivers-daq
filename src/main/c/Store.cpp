#include "Store.h"

#include "org_lsst_ccs_daq_ims_StoreNativeImplementation.h"
#include "ims/Store.hh"
#include "ims/Catalog.hh"
#include "ims/Folder.hh"
#include "ims/Image.hh"
#include "ims/Source.hh"
#include "ims/Exception.hh"
#include "dsm/Exception.hh"
#include "ims/SourceMetadata.hh"
#include "daq/LocationSet.hh"
#include "daq/Path.hh"
#include "dsm/Exception.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "rms/Client.hh"
#include "rms/Instruction.hh"
#include "rms/InstructionList.hh"
#include "gds/Client.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"

#include "MyFolders.h"
#include "MyProcessor.h"
#include "MyBarrier.h"
#include "MyHarvester.h"
#include "Statistics.h"
#include "MyGuiderSubscriber.h"


#define MESSAGE_LENGTH 1024
#define MAX_GUIDER_LOCATIONS 8

using namespace IMS;

static jclass JCversionClass;
static jmethodID JCversionConstructor;
static jclass JCbitSetClass;
static jmethodID JCbitSetConstructor;
static jmethodID JCbitSetSetMethodId;
static jmethodID JCbitSetGetMethodId;
static jclass JCimageMetaDataClass;
static jmethodID JCimageMetaDataConstructor;
static jclass JCsourceMetaDataClass;
static jmethodID JCsourceMetaDataConstructor;
static jclass JCstoreClass;
static jmethodID JCimageCreatedCallbackMethod;
static jmethodID JCimageCompleteCallbackMethod;
static jmethodID JCimageSourceStreamCallbackMethod;
static jclass JCexClass;
static jmethodID JCexConstructor;
static jmethodID JCexConstructor2;
static jclass JCintArrayClass;
static jclass JClistClass;
static jmethodID JClistAddMethodID;
static jclass JCarrayListClass;
static jmethodID JCarrayListConstructor;

jstring decodeException(JNIEnv* env, jint error) {
   const char* decoded = IMS::Exception::decode(error);
   // TODO: Check for other exceptions
   return env->NewStringUTF(decoded);
}

void throwDAQException(JNIEnv* env, const char* message) {
    jstring message_ = env->NewStringUTF(message);
    jthrowable exception = (jthrowable) env->NewObject(JCexClass, JCexConstructor2, message_);
    env->Throw(exception);
}

void throwDAQException(JNIEnv* env, const char* message, jint error) {
    jstring message_ = env->NewStringUTF(message);
    jstring decoded = decodeException(env, error);
    jthrowable exception = (jthrowable) env->NewObject(JCexClass, JCexConstructor, message_, error, decoded);
    env->Throw(exception);
}

void throwDAQException(JNIEnv* env, const char* message, jint error, const char* decoded) {
    jstring message_ = env->NewStringUTF(message);
    jstring decoded_ = env->NewStringUTF(decoded);
    jthrowable exception = (jthrowable) env->NewObject(JCexClass, JCexConstructor, message_, error, decoded_);
    env->Throw(exception);
}

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
    jstring folder = env->NewStringUTF(metaData.folder());
    jint opcode = metaData.opcode();
    jlong timestamp = metaData.timestamp();
    jlong id_ = image.id().value();
    jobject bitset = createBitSet(env, metaData.elements());
    jobject version_ = createVersion(env, metaData.release());
    return env->NewObject(JCimageMetaDataClass, JCimageMetaDataConstructor, id_, name, folder, annotation, version_, opcode, timestamp, bitset);
}

jobject createSourceMetaData(JNIEnv* env, Source& source) {
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
        register_values[i] = il.lookup(i)->operand();
    }
    jintArray registerValues = env->NewIntArray(size);
    env->SetIntArrayRegion(registerValues, 0, size, register_values);
    return env->NewObject(JCsourceMetaDataClass, JCsourceMetaDataConstructor, sensor, lane, platform, version_, firmware, serialNumber, source.size(), bay, board, registerValues);
}

jobject createList(JNIEnv* env) {
    return env->NewObject(JCarrayListClass, JCarrayListConstructor);
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
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Find image %s in folder %s failed", image_name, folder_name);
        throwDAQException(env, x, image_.error());
    }
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    return image_;
}

Image findImage(JNIEnv* env, Store* store_, uint64_t id) {
    Id id_(id);
    Image image_(id_, *store_);
    if (!image_) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Find image id %ld failed", id);
        throwDAQException(env, x, image_.error());
    }
    return image_;
}

static jint JNI_VERSION = JNI_VERSION_1_8;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    // Obtain the JNIEnv from the VM and confirm JNI_VERSION
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**> (&env), JNI_VERSION) != JNI_OK) {
        return JNI_VERSION;
    }

    jclass versionClass = env->FindClass("org/lsst/ccs/daq/ims/Version");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCversionClass = (jclass) env->NewGlobalRef(versionClass);

    JCversionConstructor = env->GetMethodID(JCversionClass, "<init>", "(Ljava/lang/String;JZI)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass bitSetClass = env->FindClass("java/util/BitSet");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCbitSetClass = (jclass) env->NewGlobalRef(bitSetClass);

    JCbitSetConstructor = env->GetMethodID(JCbitSetClass, "<init>", "()V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCbitSetSetMethodId = env->GetMethodID(JCbitSetClass, "set", "(I)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCbitSetGetMethodId = env->GetMethodID(JCbitSetClass, "get", "(I)Z");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    jclass listClass = env->FindClass("java/util/ArrayList");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JClistClass = (jclass) env->NewGlobalRef(listClass);

    JClistAddMethodID = env->GetMethodID(JClistClass, "add", "(Ljava/lang/Object;)Z");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCarrayListClass = (jclass) env->NewGlobalRef(arrayListClass);

    JCarrayListConstructor = env->GetMethodID(JCarrayListClass, "<init>", "()V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass imageMetaDataClass = env->FindClass("org/lsst/ccs/daq/ims/ImageMetaData");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCimageMetaDataClass = (jclass) env->NewGlobalRef(imageMetaDataClass);

    JCimageMetaDataConstructor = env->GetMethodID(imageMetaDataClass, "<init>", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/lsst/ccs/daq/ims/Version;IJLjava/util/BitSet;)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass sourceMetaDataClass = env->FindClass("org/lsst/ccs/daq/ims/SourceMetaData");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCsourceMetaDataClass = (jclass) env->NewGlobalRef(sourceMetaDataClass);

    JCsourceMetaDataConstructor = env->GetMethodID(JCsourceMetaDataClass, "<init>", "(BBLjava/lang/String;Lorg/lsst/ccs/daq/ims/Version;IJJBB[I)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass storeClass = env->FindClass("org/lsst/ccs/daq/ims/Store");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCstoreClass = (jclass) env->NewGlobalRef(storeClass);

    JCimageCreatedCallbackMethod = env->GetMethodID(JCstoreClass, "imageCreatedCallback", "(Lorg/lsst/ccs/daq/ims/ImageMetaData;)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    JCimageCompleteCallbackMethod = env->GetMethodID(JCstoreClass, "imageCompleteCallback", "(Lorg/lsst/ccs/daq/ims/ImageMetaData;)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    JCimageSourceStreamCallbackMethod = env->GetMethodID(JCstoreClass, "imageSourceStreamCallback", "(JIJ)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }

    jclass exClass = env->FindClass("org/lsst/ccs/daq/ims/DAQException");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCexClass = (jclass) env->NewGlobalRef(exClass);

    JCexConstructor = env->GetMethodID(JCexClass, "<init>", "(Ljava/lang/String;ILjava/lang/String;)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }    

    JCexConstructor2 = env->GetMethodID(JCexClass, "<init>", "(Ljava/lang/String;)V");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    } 

    jclass intArrayClass = env->FindClass("[I");
    if (env->ExceptionCheck()) {
        return JNI_VERSION;
    }
    JCintArrayClass = (jclass) env->NewGlobalRef(intArrayClass);

    // Call corresponding function in Statistics.cpp
    JNI_Stats_OnLoad(env);
    Guider_OnLoad(env);
    
    // Return the JNI Version as required by method
    return JNI_VERSION;
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachStore
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        Store* store = new Store(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) store;
    } catch (DSM::Exception& x) {
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachStore
(JNIEnv* env, jobject obj, jlong store) {
    delete ((Store*) store);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachCamera
(JNIEnv* env, jobject obj, jlong store) {
    Store* store_ = (Store*) store;
    CMS::Camera* camera = new CMS::Camera(*store_);
    return (jlong) camera;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachCamera
(JNIEnv* env, jobject obj, jlong camera) {
    delete ((CMS::Camera*) camera);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachGuider
(JNIEnv* env, jobject obj, jstring partition) {   
    const char *partition_name = env->GetStringUTFChars(partition, 0); 
    try {
        GDS::Client* guider = new GDS::Client(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) guider;
    } catch (DSM::Exception& x) {
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachGuider
(JNIEnv* env, jobject obj, jlong guider) {
    delete ((GDS::Client*) guider);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachGuiderSubscriber
(JNIEnv* env, jobject obj, jstring partition, jboolean bigEndian, jintArray locations) {   
    jint* values = env->GetIntArrayElements(locations, 0);
    int nlocs = env->GetArrayLength(locations);
    GDS::LocationSet locs;
    for (int j=0; j<nlocs; j+=2) {
        GDS::Location loc(DAQ::Location(values[j]), values[j+1]);
        locs.insert(loc);
    }
    const char *partition_name = env->GetStringUTFChars(partition, 0);
    MyGuiderSubscriber* subscriber = new MyGuiderSubscriber(partition_name, bigEndian, locs);
    env->ReleaseStringUTFChars(partition, partition_name);
    env->ReleaseIntArrayElements(locations, values, JNI_ABORT);
    return (jlong) subscriber;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachGuiderSubscriber
(JNIEnv* env, jobject obj, jlong subscriber) {
    delete ((MyGuiderSubscriber*) subscriber);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_waitForGuider
(JNIEnv *env, jobject obj, jlong subscriber_, jobject callback) {
    MyGuiderSubscriber* subscriber =  (MyGuiderSubscriber*) subscriber_;
    subscriber->wait(env, callback);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_abortWaitForGuider
(JNIEnv *env, jobject obj, jlong subscriber_) {
    MyGuiderSubscriber* subscriber =  (MyGuiderSubscriber*) subscriber_;
    subscriber->abort(env);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_startGuider
(JNIEnv* env, jobject obj, jlong guider_, jint rows, jint cols, jint integration, jstring id, jintArray roiData) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    GDS::RoiCommon common(rows, cols, integration);
    GDS::RoiLocation locs[MAX_GUIDER_LOCATIONS];
    jint* values = env->GetIntArrayElements(roiData, 0);
    int nlocs = env->GetArrayLength(roiData)/5;
    for (int i=0; i<nlocs; i++) {
       int j = i*5;
       uint8_t index = values[j];
       uint8_t sensor = values[j+1];
       uint16_t segment = values[j+2];
       uint16_t startRow = values[j+3];
       uint16_t startCol = values[j+4];
       locs[i] = GDS::RoiLocation(GDS::Location(DAQ::Location(index), sensor), segment, startRow, startCol);
    }
    const char *_id = env->GetStringUTFChars(id, 0);
    int error = guider->start(common, locs, nlocs, _id, status);
    if (!error) error = status.status();
    env->ReleaseStringUTFChars(id, _id);
    env->ReleaseIntArrayElements(roiData, values, JNI_ABORT);
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider start failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL;
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_validateGuider
(JNIEnv* env, jobject obj, jlong guider_, jint rows, jint cols, jint integration, jintArray roiData) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    GDS::RoiCommon common(rows, cols, integration);
    GDS::RoiLocation locs[MAX_GUIDER_LOCATIONS];
    jint* values = env->GetIntArrayElements(roiData, 0);
    int nlocs = env->GetArrayLength(roiData)/5;
    for (int i=0; i<nlocs; i++) {
       int j = i*5;
       uint8_t index = values[j];
       uint8_t sensor = values[j+1];
       uint16_t segment = values[j+2];
       uint16_t startRow = values[j+3];
       uint16_t startCol = values[j+4];
       locs[i] = GDS::RoiLocation(GDS::Location(DAQ::Location(index), sensor), segment, startRow, startCol);
    }
    try {
       guider->validate(common, locs, nlocs);
       env->ReleaseIntArrayElements(roiData, values, JNI_ABORT);
    } catch (GDS::Exception& x) {
       env->ThrowNew(JCexClass, x.what());
    }

}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_stopGuider
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    int error = guider->stop(status);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider stop failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL; 
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_pauseGuider
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    int error = guider->pause(status);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider pause failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL; 
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_resumeGuider
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    int error = guider->resume(status);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider resume failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL;
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_sleepGuider
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    int error = guider->sleep(status);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider sleep failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL;
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_wakeGuider
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client*  guider = (GDS::Client*) guider_;
    GDS::Status status;
    int error = guider->wake(status);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider wake failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL;
    } else {
        return createGuiderStatus(env, status);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_guiderConfig
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client* guider = (GDS::Client*) guider_;
    GDS::Status status;
    GDS::Series series;
    GDS::RoiCommon roiCommon;
    GDS::RoiLocation locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
    unsigned nlocsbuf;
    
    int error = guider->config(status, series, roiCommon, locbuf, nlocsbuf);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider config failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
        return NULL;
    }  else {
        return createGuiderConfig(env, status, series, roiCommon, locbuf, nlocsbuf);
    }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_guiderSeries
(JNIEnv* env, jobject obj, jlong guider_) {
    GDS::Client* guider = (GDS::Client*) guider_;
    GDS::Status status;
    GDS::Series series;
    
    int error = guider->series(status, series);
    if (!error) error = status.status();
    if (error) {
        char x[MESSAGE_LENGTH];
        char y[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Guider series failed, status %d", error);
        throwDAQException(env, x, error, GDS::Exception::decode(error, y));
    }

    return createGuiderSeries(env, status, series);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_capacity
(JNIEnv * env, jobject obj, jlong store) {

    return ((Store *) store)->capacity() * (jlong) XDS::Page::SIZE;
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_remaining
(JNIEnv * env, jobject obj, jlong store) {

    return ((Store *) store)->remaining() * (jlong) XDS::Page::SIZE;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_listFolders
(JNIEnv *env, jobject obj, jlong store, jobject result) {

    Catalog& catalog = ((Store *) store)->catalog;
    MyFolders folders(catalog, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    folders.traverse();
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_insertFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    jint rc = catalog.insert(folder_name);
    env->ReleaseStringUTFChars(name, folder_name);
    return rc;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_removeFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    jint rc = catalog.remove(folder_name);
    env->ReleaseStringUTFChars(name, folder_name);
    return rc;
}

JNIEXPORT jboolean JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_findFolder
(JNIEnv *env, jobject obj, jlong store, jstring name) {
    Catalog& catalog = ((Store *) store)->catalog;
    const char *folder_name = env->GetStringUTFChars(name, 0);
    Folder folder(folder_name, catalog);
    if (!folder) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "No such folder %s", folder_name);
        throwDAQException(env, x, folder.error());
    }
    env->ReleaseStringUTFChars(name, folder_name);
    return 1;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_listImages
(JNIEnv *env, jobject obj, jlong store, jstring folderName, jobject result) {
    Store* store_ = (Store*) store;
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    Folder folder(folder_name, store_->catalog);
    if (!folder) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "No such folder %s", folder_name);
        throwDAQException(env, x, folder.error());
        return;
    }
    MyProcessor processor(*store_, env, result);
    if (env->ExceptionCheck()) {
        return;
    }
    folder.traverse(processor);
    env->ReleaseStringUTFChars(folderName, folder_name);
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_moveImageToFolder
(JNIEnv *env, jobject obj, jlong store, jlong id, jstring folderName) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, id);
    if (!image_) {
        return JNI_ERR;
    }
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    int rc = image_.moveTo(folder_name);
    env->ReleaseStringUTFChars(folderName, folder_name);
    return rc;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_deleteImage
(JNIEnv *env, jobject obj, jlong store, jlong id) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, id);
    if (!image_) {
        return JNI_ERR;
    }
    return image_.remove();
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_findSource
  (JNIEnv *env, jobject obj, jlong store, jlong id, jint location) {
    Store* store_ = (Store*) store;
    Image image_ = findImage(env, store_, id);
    if (!image_) {
        return 0;
    }
    DAQ::Location element(location);
    Source source(image_.id(), element, *store_);
    if (!source) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Source not found (id=%ld elementIndex=%d)", id, location);
        throwDAQException(env, x, source.error());
        return 0;
    }
    return createSourceMetaData(env, source);    
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_addImageToFolder
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName, jstring annotation, jint opcode, jobject locations) {
    Store* store_ = (Store*) store;
    const char *image_name = env->GetStringUTFChars(imageName, 0);
    const char *folder_name = env->GetStringUTFChars(folderName, 0);
    const char *annotation_ = env->GetStringUTFChars(annotation, 0);
    jobject result = NULL;

    DAQ::LocationSet locations_ = convertLocations(env, locations);
    ImageMetadata meta(image_name, folder_name, locations_, opcode, annotation_);
    Image image(meta, *store_);
    if (!image) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Creating image %s in folder %s failed", image_name, folder_name);
        throwDAQException(env, x, image.error());
    } else {
        result = createImageMetaData(env, image);
    }
    env->ReleaseStringUTFChars(folderName, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    env->ReleaseStringUTFChars(annotation, annotation_);
    return result;
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_findImage
(JNIEnv *env, jobject obj, jlong store, jstring imageName, jstring folderName) {

    Store* store_ = (Store*) store;
    Image image = findImage(env, store_, imageName, folderName);
    if (!image) return 0;
    return createImageMetaData(env, image);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_openSourceChannel
(JNIEnv *env, jobject obj, jlong store, jlong id, jint elementIndex, jboolean write) {
    Store* store_ = (Store*) store;
    Id id_((uint64_t) id);
    if (!id_) {
        return JNI_ERR;
    }
    DAQ::Location element(elementIndex);
    Source* source = new Source(id_, element, *store_);
    if (!source) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "Source not found (id=%ld elementIndex=%d)", id, elementIndex);
        throwDAQException(env, x, source->error());
        delete source;
    }
    return (jlong) source;
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_addSourceToImage
(JNIEnv *env, jobject obj, jlong store, jlong id, jint elementIndex, jbyte type, jstring platform, jintArray registerValues) {
    Store* store_ = (Store*) store;
    Image image = findImage(env, store_, id);
    if (!image) {
        return 0;
    }
    DAQ::Location element(elementIndex);
    const char *platform_ = env->GetStringUTFChars(platform, 0);
    SourceMetadata smd((DAQ::Sensor::Type) type, DAQ::Path::LANE::EMULATOR, platform_);
    RMS::InstructionList il;
    jint size = env->GetArrayLength(registerValues);
    jint* values = env->GetIntArrayElements(registerValues, 0);
    for (uint32_t i = 0; i < size; i++) {
        il.insert(RMS::Instruction::Opcode::GET, 0x360000+i, values[i]);
    }
    for (uint32_t i = size; i < il.size(); i++) {
        il.insert(RMS::Instruction::Opcode::GET, 0x360000+i, 0);
    }
    smd = il;
    env->ReleaseIntArrayElements(registerValues, values, JNI_ABORT);
    env->ReleaseStringUTFChars(platform, platform_);
    Source source(image.id(), element, *store_, smd);
    return createSourceMetaData(env, source);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachStream
(JNIEnv* env, jobject obj, jlong store, jint sourceTimeoutMicros) {
    Store* store_ = (Store*) store;
    return (jlong) new Stream(*store_, sourceTimeoutMicros);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachSteam
(JNIEnv* env, jobject obj, jlong stream) {
    delete ((Stream*) stream);
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_waitForImage
(JNIEnv *env, jobject obj, jobject callback, jlong store, jlong stream1, jlong stream2, jint imageTimeoutMicros, jint sourceTimeoutMicros) {
    Store* store_ = (Store*) store;
    Stream* stream1_ = (Stream*) stream1;
    Stream* stream2_ = (Stream*) stream2;
    // Per Gregg, this timeout is actually in units of 10ms
    Image image(*store_, *stream1_, imageTimeoutMicros/10000);
    if (!image) return image.error();
    env->CallVoidMethod(callback, JCimageCreatedCallbackMethod, createImageMetaData(env, image));
    MyBarrier barrier(*store_, env, image, callback, JCimageSourceStreamCallbackMethod);
    barrier.run(*stream2_, sourceTimeoutMicros);
    // We get here either because the image is complete, or because it timed out. We cannot tell
    // which, so we will just hope for the best!
    env->CallVoidMethod(callback, JCimageCompleteCallbackMethod, createImageMetaData(env, image));
    return 0;
}

JNIEXPORT jstring JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_decodeException
  (JNIEnv *env , jobject obj, jint error) {
    return decodeException(env, error);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_getConfiguredSources
  (JNIEnv *env, jobject obj, jlong store) {
    Store* store_ = (Store*) store;
    CMS::Camera camera(*store_);
    const DAQ::LocationSet& locations = camera.sources();
    return createBitSet(env, locations);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_getConfiguredLocations
  (JNIEnv *env, jobject obj, jstring partition) {
    const char *partition_ = env->GetStringUTFChars(partition, 0);
    RMS::Client client(partition_);
    env->ReleaseStringUTFChars(partition, partition_);
    const DAQ::LocationSet& locations = client.sources();
    return createBitSet(env, locations);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_getClientVersion
  (JNIEnv *env, jobject obj) {
    DVI::Version version;
    return createVersion(env, version);
}

void setRegisterList
  (JNIEnv *env,  RMS::InstructionList& instList,  jintArray regs) {

    if (regs == NULL) return;
    int numRegs = env->GetArrayLength(regs);
    if (numRegs > RMS::InstructionList::MAXIMUM) {
        char text[MESSAGE_LENGTH];
        snprintf(text, MESSAGE_LENGTH, "Too many registers specified: %d", numRegs);
        throwDAQException(env, text);
        return;
    }

    instList.clear();
    int* regArray = env->GetIntArrayElements(regs, NULL);
    for (int j = 0; j < numRegs; j++) {
        instList.insert(RMS::Instruction::GET, regArray[j], 0);
    }
    env->ReleaseIntArrayElements(regs, regArray, JNI_ABORT);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_setRegisterList
  (JNIEnv *env, jobject obj, jlong store, jlong camera, jint ccdCount,  jintArray regs) {
      CMS::Camera* camera_ = (CMS::Camera*) camera;
      if (ccdCount == 1) {
         setRegisterList(env, camera_->wavefront, regs);
      } else if (ccdCount == 2) {
         setRegisterList(env, camera_->guiding, regs);
      } else if (ccdCount == 3) {
         setRegisterList(env, camera_->science, regs);
      }
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_triggerImage
  (JNIEnv *env, jobject obj, jlong store, jlong camera, jstring folder, jstring imageName, jstring annotation, jint opcode, jobject bitset) {
    Store* store_ = (Store*) store;
    CMS::Camera* camera_ = (CMS::Camera*) camera;
    jobject result = NULL;

    const char *image_name = env->GetStringUTFChars(imageName, 0);
    const char *folder_name = env->GetStringUTFChars(folder, 0);
    const char *annotation_ = env->GetStringUTFChars(annotation, 0);
    DAQ::LocationSet locations_ = convertLocations(env, bitset);
    ImageMetadata meta(image_name, folder_name, locations_, opcode, annotation_);
    int rc = camera_->trigger(meta);
    if (rc != CMS::Exception::NONE) {
       char x[MESSAGE_LENGTH];
       snprintf(x, MESSAGE_LENGTH, "Triggering image %s in folder %s failed", image_name, folder_name);
       throwDAQException(env, x, rc, CMS::Exception::decode(rc));
    } else {
        Id id = store_->catalog.lookup(image_name, folder_name);
        Image image(id, *store_);
        if (!image) {
            char x[MESSAGE_LENGTH];
            snprintf(x, MESSAGE_LENGTH, "Accessing image %s in folder %s failed", image_name, folder_name);
            throwDAQException(env, x, image.error());
        } else {
            result = createImageMetaData(env, image);
        }
    }
    env->ReleaseStringUTFChars(folder, folder_name);
    env->ReleaseStringUTFChars(imageName, image_name);
    env->ReleaseStringUTFChars(annotation, annotation_);
    return result;
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_startSequencer
  (JNIEnv *env, jobject obj, jlong camera, jint opcode) {
    CMS::Camera* camera_ = (CMS::Camera*) camera;
    OSA::TimeStamp timestamp;
    camera_->sequence(opcode, timestamp);
    return (jlong)timestamp;
}


JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_attachClient
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        RMS::Client *client = new RMS::Client(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) client;
    } catch (DSM::Exception& x) {
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_detachClient
(JNIEnv* env, jobject obj, jlong client) {
    delete ((RMS::Client*) client);
}

JNIEXPORT jobjectArray JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_readRegisters
  (JNIEnv *env, jobject obj, jlong client, jobject locations, jintArray addresses) {

    int numRegs = env->GetArrayLength(addresses);
    if (numRegs > RMS::InstructionList::MAXIMUM) {
        char text[MESSAGE_LENGTH];
        snprintf(text, MESSAGE_LENGTH, "Too many registers specified: %d", numRegs);
        throwDAQException(env, text);
        return NULL;
    }

    DAQ::LocationSet locations_ = convertLocations(env, locations);
    RMS::Client* client_ = (RMS::Client*) client;
    RMS::InstructionList instList;

    int* regArray = env->GetIntArrayElements(addresses, NULL);
    for (int j = 0; j < numRegs; j++) {
        instList.insert(RMS::Instruction::GET, regArray[j]);
    }
    env->ReleaseIntArrayElements(addresses, regArray, JNI_ABORT);

    MyHarvester harvester(numRegs);
    client_->access(locations_, instList, harvester);
    if (harvester.errorCount() > 0) {
        char x[2*MESSAGE_LENGTH];
        char encoded[MESSAGE_LENGTH];
        harvester.errors().encode(encoded);
        snprintf(x, 2*MESSAGE_LENGTH, "%d errors reading registers %s", harvester.errorCount(), encoded);
        throwDAQException(env, x);
    }
    jobjectArray result = env->NewObjectArray(locations_.SIZE, JCintArrayClass, NULL);
    for (int j=0; j < locations_.SIZE; j++) {
        jintArray intArray = env->NewIntArray(numRegs);
        env->SetIntArrayRegion(intArray, 0, numRegs, harvester.values(j));
        env->SetObjectArrayElement(result, j, intArray);
    }
    return result;
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_StoreNativeImplementation_writeRegisters
  (JNIEnv *env, jobject obj, jlong client, jobject locations, jintArray addresses, jintArray values) {

    int numRegs = env->GetArrayLength(addresses);
    if (numRegs > RMS::InstructionList::MAXIMUM) {
        char text[MESSAGE_LENGTH];
        snprintf(text, MESSAGE_LENGTH, "Too many registers specified: %d", numRegs);
        throwDAQException(env, text);
        return;
    }

    DAQ::LocationSet locations_ = convertLocations(env, locations);
    RMS::Client* client_ = (RMS::Client*) client;
    RMS::InstructionList instList;

    int* regArray = env->GetIntArrayElements(addresses, NULL);
    int* regValues = env->GetIntArrayElements(values, NULL);
    for (int j = 0; j < numRegs; j++) {
        instList.insert(RMS::Instruction::PUT, regArray[j], regValues[j]);
    }
    env->ReleaseIntArrayElements(addresses, regArray, JNI_ABORT);
    env->ReleaseIntArrayElements(addresses, regValues, JNI_ABORT);

    MyHarvester harvester(numRegs);
    client_->access(locations_, instList, harvester);
    if (harvester.errorCount() > 0) {
        char x[MESSAGE_LENGTH];
        snprintf(x, MESSAGE_LENGTH, "%d errors writing registers", harvester.errorCount());
        throwDAQException(env, x);
    }
}
