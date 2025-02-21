package org.lsst.ccs.daq.ims;

import java.util.BitSet;
import java.util.List;

import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.Status;
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

    DAQSourceChannelImplementation openSourceChannelObject(long store, long id, int index, boolean write) throws DAQException;

    SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException;

    long attachStream(long store, int sourceTimeoutMicros) throws DAQException;

    void detachStream(long stream) throws DAQException;

    int waitForImage(Store callback, long store, long stream1, long stream2, int imageTimeoutMicros, int sourceTimeoutMicro) throws DAQException;

    String decodeException(int rc);

    BitSet getConfiguredSources(long store) throws DAQException;

    BitSet getConfiguredLocations(String partition) throws DAQException;

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

    Status startGuider(long guider, int rows, int cols, int integration, String id, int[] roiData) throws DAQException;

    void validateGuider(long guider, int nRows, int nCols, int integrationTimeMilliSeconds, int[] roiData) throws DAQException;

    Status stopGuider(long guider, String comment) throws DAQException;

    Status pauseGuider(long guider, String comment) throws DAQException;

    Status resumeGuider(long guider, String comment) throws DAQException;

    Status sleepGuider(long guider) throws DAQException;

    Status wakeGuider(long guider) throws DAQException;

    long attachGuiderSubscriber(String partition, boolean bigEndian, int[] locations) throws DAQException;

    void detachGuiderSubscriber(long subscriber) throws DAQException;

    void waitForGuider(long subscriber, Guider.Subscriber callback) throws DAQException;

    void abortWaitForGuider(long subscriber) throws DAQException;

    Config guiderConfig(long guider) throws DAQException;

    SeriesStatus guiderSeries(long guider) throws DAQException;

    Status clearGuider(long guider) throws DAQException;

    String getClientPlatform(String partition) throws DAQException;
}
