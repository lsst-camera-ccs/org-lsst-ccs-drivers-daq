package org.lsst.ccs.daq.ims;

import java.util.BitSet;
import java.util.List;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.Series;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.utilities.location.Location.LocationType;

/**
 * The native implementation of the store. This implementation required the DAQ
 * be present.
 *
 * @author tonyj
 */
class StoreNativeImplementation implements StoreImplementation {

    static {
        System.loadLibrary("ccs_daq_ims");
    }

    @Override
    public synchronized native long attachStore(String partition) throws DAQException;

    @Override
    public synchronized native void detachStore(long store) throws DAQException;

    @Override
    public synchronized native long attachCamera(long store) throws DAQException;

    @Override
    public synchronized native void detachCamera(long camera) throws DAQException;
    
    @Override
    public synchronized native long attachClient(String partition) throws DAQException;

    @Override
    public synchronized native void detachClient(long client) throws DAQException;

    @Override
    public synchronized native long capacity(long store) throws DAQException;

    @Override
    public synchronized native long remaining(long store) throws DAQException;

    @Override
    public synchronized native void listFolders(long store, List<String> result) throws DAQException;

    @Override
    public synchronized native int insertFolder(long store, String folderName) throws DAQException;

    @Override
    public synchronized native int removeFolder(long store, String folderName) throws DAQException;

    @Override
    public synchronized native boolean findFolder(long store, String folderName) throws DAQException;

    @Override
    public synchronized native void listImages(long store, String folderName, List<ImageMetaData> result) throws DAQException;

    @Override
    public synchronized native int moveImageToFolder(long store, long id, String folderName) throws DAQException;

    @Override
    public synchronized native int deleteImage(long store, long id) throws DAQException;

    @Override
    public synchronized native SourceMetaData findSource(long store, long id, int location) throws DAQException;

    @Override
    public synchronized native ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException;

    @Override
    public synchronized native ImageMetaData findImage(long store, String imageName, String folderName) throws DAQException;

    @Override
    public synchronized native long openSourceChannel(long store, long id, int index, boolean write) throws DAQException;

    @Override
    public synchronized native SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException;

    @Override
    public synchronized native long attachStream(long store, int sourceTimeoutMicros) throws DAQException;

    @Override
    public synchronized native void detachStream(long stream) throws DAQException;
    
    @Override
    public native int waitForImage(Store callback, long store, long stream1, long stream2, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException;

    @Override
    public native String decodeException(int rc);

    @Override
    public synchronized native BitSet getConfiguredSources(long store) throws DAQException;

    @Override
    public synchronized native BitSet getConfiguredLocations(String partition) throws DAQException;
    
    @Override
    public native Version getClientVersion() throws DAQException;
    
    @Override
    public void setRegisterList(long store, long camera, LocationType type, int[] registerList) throws DAQException {
        setRegisterList(store, camera, type.getCCDCount(), registerList);
    }

    @Override
    public ImageMetaData triggerImage(long store, long camera, ImageMetaData meta) throws DAQException {
        return triggerImage(store, camera, meta.getCreationFolderName(), meta.getName(), meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    private synchronized native void setRegisterList(long store, long camera, int ccdCount, int[] registerAddresses) throws DAQException;

    private synchronized native ImageMetaData triggerImage(long store, long camera, String metaData, String name, String annotation, int opcode, BitSet locationBitSet) throws DAQException;

    @Override
    public synchronized native long startSequencer(long camera, int opcode) throws DAQException;


    @Override
    public synchronized native int[][] readRegisters(long client, BitSet locations, int[] addresses) throws DAQException;

    @Override
    public synchronized native void writeRegisters(long client, BitSet locations, int[] addresses, int[] values) throws DAQException;

    @Override
    public synchronized native long attachGuider(String partition) throws DAQException;

    @Override
    public synchronized native void detachGuider(long guider) throws DAQException;

    @Override
    public synchronized native Status startGuider(long guider, int rows, int cols, int integration, String id, int[] roiData) throws DAQException;

    @Override
    public synchronized native Status stopGuider(long guider) throws DAQException;

    @Override
    public synchronized native Status pauseGuider(long guider) throws DAQException;

    @Override
    public synchronized native Status resumeGuider(long guider) throws DAQException;

    @Override
    public synchronized native Status sleepGuider(long guider) throws DAQException;

    @Override
    public synchronized native Status wakeGuider(long guider) throws DAQException;
    
    @Override
    public native void waitForGuider(long subscriber, Guider.Subscriber callback) throws DAQException;
       
    @Override
    public native void abortWaitForGuider(long subscriber) throws DAQException;

    @Override
    public synchronized native long attachGuiderSubscriber(String partition, boolean bigEndian, int[] locations) throws DAQException;

    @Override
    public synchronized native void detachGuiderSubscriber(long subscriber) throws DAQException;

    @Override
    public synchronized native Config guiderConfig(long guider) throws DAQException;

    @Override
    public synchronized native Series guiderSeries(long guider) throws DAQException;
}
