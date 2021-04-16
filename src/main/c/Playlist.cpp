#include "Playlist.h"

#include "org_lsst_ccs_daq_ims_Emulator.h"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"
#include "emu/Client.hh"
#include "emu/Exception.hh"
#include "emu/PlayList.hh"
#include "ims/Image.hh"
#include "ims/Store.hh"

#include "Store.h"

#define MESSAGE_LENGTH 256

static jclass JCexClass;
static jmethodID JCexConstructor;

jstring decodeEmu(JNIEnv* env, jint error) {
   const char* decoded = EMU::Exception::decode(error);
   // TODO: Check for other exceptions
   return env->NewStringUTF(decoded);
}

void throwDAQEmulationException(JNIEnv* env, char* message, jint error) {
    jstring message_ = env->NewStringUTF(message);
    jstring decoded = decodeEmu(env, error);
    jthrowable exception = (jthrowable) env->NewObject(JCexClass, JCexConstructor, message_, error, decoded);
    env->Throw(exception);
}

void JNI_Emulation_OnLoad(JNIEnv* env) {


    jclass exClass = env->FindClass("org/lsst/ccs/daq/ims/DAQException");
    if (env->ExceptionCheck()) {
        return;
    }
    JCexClass = (jclass) env->NewGlobalRef(exClass);

    JCexConstructor = env->GetMethodID(JCexClass, "<init>", "(Ljava/lang/String;ILjava/lang/String;)V");
    if (env->ExceptionCheck()) {
        return;
    }

    return;
}


/*  JNI methods to attach or detach the emu clients.  It is up to the
 *  Java code to save the client pointers, and use the proper client
 *  in JNI methods beyond attach.
 */

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_Emulator_attachEmuClient
(JNIEnv* env, jobject obj, jstring partition) {

    const char *partition_name = env->GetStringUTFChars(partition, 0);
    try {
        EMU::Client* client = new EMU::Client(partition_name);
        env->ReleaseStringUTFChars(partition, partition_name);
        return (jlong) client;
    } catch (EMU::Exception& x) {
        env->ReleaseStringUTFChars(partition, partition_name);
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Emulator_detachEmuClient
(JNIEnv* env, jobject obj, jlong client) {
  delete ((EMU::Client*) client);
}

JNIEXPORT jlong JNICALL Java_org_lsst_ccs_daq_ims_Emulator_openPlaylist
  (JNIEnv *env, jobject obj , jlong client, jstring fileName) {
    const char *fileName_ = env->GetStringUTFChars(fileName, 0);
    try {
        EMU::PlayList* playlist = new EMU::PlayList(fileName_);
        return (jlong) playlist;
    } catch (EMU::Exception& x) {
        env->ReleaseStringUTFChars(fileName, fileName_);
        return env->ThrowNew(JCexClass, x.what());
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Emulator_closePlaylist
  (JNIEnv *env, jobject obj, jlong client, jlong playlist) {
    delete (EMU::PlayList*) playlist;
}

JNIEXPORT jint JNICALL Java_org_lsst_ccs_daq_ims_Emulator_playlistSize
  (JNIEnv *env, jobject obj, jlong client, jlong playlist) {
    EMU::PlayList* playlist_ = (EMU::PlayList*) playlist;
    return playlist_->length();
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Emulator_list
  (JNIEnv *env, jobject obj, jlong client, jlong store, jlong playlist, jobject result) {
    EMU::PlayList* playlist_ = (EMU::PlayList*) playlist;
    IMS::Store* store_ = (IMS::Store*) store;

    unsigned remaining = playlist_->length();
    const IMS::Id* id  = playlist_->vector();
    while(remaining--) {

        IMS::Image image(*id, *store_);
        if(image) {
            addObjectToList(env, result, createImageMetaData(env, image));
        } else {
            char x[MESSAGE_LENGTH];
            snprintf(x, MESSAGE_LENGTH, "Image in playlist not found");
            throwDAQEmulationException(env, x, image.error());
            return;
        }
        id++;
    }
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Emulator_addImageToPlaylist
  (JNIEnv *env, jobject obj, jlong client, jlong store, jlong playlist, jlong id) {
    EMU::PlayList* playlist_ = (EMU::PlayList*) playlist;
    IMS::Id id_((uint64_t) id);
    playlist_->insert(id_);
}

JNIEXPORT jobject JNICALL Java_org_lsst_ccs_daq_ims_Emulator_getLocations
  (JNIEnv *env, jobject obj, jlong client) {
    EMU::Client* client_ = (EMU::Client*) client;
    const DSI::LocationSet& locations = client_->emulators();
    return createBitSet(env, locations);
}

JNIEXPORT void JNICALL Java_org_lsst_ccs_daq_ims_Emulator_startPlaylist
  (JNIEnv *env, jobject obj, jlong client, jlong playlist, jboolean repeat) {
    EMU::Client* client_ = (EMU::Client*) client;
    EMU::PlayList* playlist_ = (EMU::PlayList*) playlist;
    client_->play(*playlist_, repeat);
}
