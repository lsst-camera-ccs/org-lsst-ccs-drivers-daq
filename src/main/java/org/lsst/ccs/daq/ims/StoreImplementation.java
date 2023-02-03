package org.lsst.ccs.daq.ims;

import java.util.BitSet;
import java.util.List;
import org.lsst.ccs.daq.ims.Guider.Config;
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
        
    Guider.Status startGuider(long guider, int rows, int cols, int integration, String id, int[] roiData) throws DAQException;

    Guider.Status stopGuider(long guider) throws DAQException;

    Guider.Status pauseGuider(long guider) throws DAQException;

    Guider.Status resumeGuider(long guider) throws DAQException;

    Guider.Status sleepGuider(long guider) throws DAQException;

    Guider.Status wakeGuider(long guider) throws DAQException;

    public long attachGuiderSubscriber(String partition, boolean bigEndian, int[] locations) throws DAQException;

    public void detachGuiderSubscriber(long subscriber) throws DAQException;

    public void waitForGuider(long subscriber, Guider.Subscriber callback) throws DAQException;

    public Config guiderConfig(long guider) throws DAQException;

    public Guider.Series guiderSeries(long guider) throws DAQException;


}
