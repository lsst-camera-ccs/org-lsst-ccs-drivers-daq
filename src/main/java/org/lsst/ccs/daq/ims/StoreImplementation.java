package org.lsst.ccs.daq.ims;

import java.util.BitSet;
import java.util.List;
import org.lsst.ccs.utilities.location.Location;

/**
 * An interface implemented by both the native and simulated
 *
 * @author tonyj
 */
interface StoreImplementation {

    long attachStore(String partition) throws DAQException;

    void detachStore(long store) throws DAQException;

    long attachCamera(long store) throws DAQException;

    void detachCamera(long camera) throws DAQException;

    long capacity(long store) throws DAQException;

    long remaining(long store) throws DAQException;

    void listFolders(long store, List<String> result) throws DAQException;

    int insertFolder(long store, String folderName) throws DAQException;

    int removeFolder(long store, String folderName) throws DAQException;

    boolean findFolder(long store, String folderName) throws DAQException;

    void listImages(long store, String folderName, List<ImageMetaData> result) throws DAQException;

    int moveImageToFolder(long store, long id, String folderName) throws DAQException;

    int deleteImage(long store, long id) throws DAQException;

    SourceMetaData findSource(long store, long id, int location) throws DAQException;

    ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException;

    ImageMetaData findImage(long store, String imageName, String folderName) throws DAQException;

    long openSourceChannel(long store, long id, int index, boolean write) throws DAQException;

    SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException;

    int waitForImage(Store callback, long store, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException;

    String decodeException(int rc);

    BitSet getConfiguredSources(long store) throws DAQException;

    Version getClientVersion() throws DAQException;

    void setRegisterList(long store, long camera, Location.LocationType type, int[] registerList) throws DAQException;

    ImageMetaData triggerImage(long store, long camera, ImageMetaData meta) throws DAQException;

    long startSequencer(long camera, int opcode) throws DAQException;

    long attachClient(String partition) throws DAQException;

    void detachClient(long client) throws DAQException;

    int[][] readRegisters(long client, BitSet locations, int[] addresses) throws DAQException;

    void writeRegisters(long client, BitSet locations, int[] addresses, int[] values) throws DAQException;

    long attachGuider(String partition) throws DAQException;

    void detachGuider(long guider) throws DAQException;
    
    void waitForGuider(Guider guider, String partition, int[] locations) throws DAQException;
    
    void startGuider(long guider, int rows, int cols, int integration, int binning, int[] roiData) throws DAQException;

    void stopGuider(long guider) throws DAQException;

    void pauseGuider(long guider) throws DAQException;

    void resumeGuider(long guider) throws DAQException;
}
