package org.lsst.ccs.daq.ims;

import java.time.Instant;
import java.util.BitSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.Location.LocationType;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A simulated implementation of the Store API. Used for testing and running
 * subsystem without a real DAQ.
 *
 * @author tonyj
 */
class StoreSimulatedImplementation implements StoreImplementation {

    StoreSimulation storeSimulation = StoreSimulation.instance();
    private final Random random = new Random();
    private final Version release = new Version("daq-simulation", toNanos(Instant.now()), false, 12345);
    private final SynchronousQueue<ImageMetaData> queue = new SynchronousQueue<>();
    private final Map<Location.LocationType, int[]> registerLists = new HashMap<>();

    @Override
    public long attachStore(String partition) throws DAQException {
        return 100;
    }

    @Override
    public void detachStore(long store) throws DAQException {

    }

    @Override
    public long attachCamera(long store) throws DAQException {
        return 101;
    }

    @Override
    public void detachCamera(long camera) throws DAQException {

    }

    @Override
    public long attachClient(String partition) throws DAQException {
        return 102;
    }

    @Override
    public void detachClient(long client) throws DAQException {
    }

    @Override
    public int[][] readRegisters(long client, BitSet bitset, int[] addresses) throws DAQException {
        int[][] result = new int[4*25][addresses.length];
        LocationSet locations = new LocationSet(bitset);
        for (Location l : locations) {
            for (int i=0; i<addresses.length; i++) {
                result[l.index()][i] = storeSimulation.readRegister(l, addresses[i]);
            }
        }
        return result;
    }

    @Override
    public void writeRegisters(long client, BitSet bitset, int[] addresses, int[] values) throws DAQException {
        LocationSet locations = new LocationSet(bitset);
        for (Location l : locations) {
            for (int i=0; i<addresses.length; i++) {
                storeSimulation.writeRegister(l, addresses[i], values[i]);
            }
        }
    }

    @Override
    public long capacity(long store) throws DAQException {
        return 500_000_000_000L;
    }

    @Override
    public long remaining(long store) throws DAQException {
        return 250_000_000_000L;
    }

    @Override
    public void listFolders(long store, List<String> result) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int insertFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int removeFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean findFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void listImages(long store, String folderName, List<ImageMetaData> result) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int moveImageToFolder(long store, long id, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int deleteImage(long store, long id) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public SourceMetaData findSource(long store, long id, int location) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public ImageMetaData findImage(long store, String imageName, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public long openSourceChannel(long store, long id, int index, boolean write) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int waitForImage(Store callback, long store, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException {
        try {
            ImageMetaData meta = imageTimeoutMicros == 0 ? queue.take() : queue.poll(imageTimeoutMicros, TimeUnit.MICROSECONDS);
            if (meta == null) {
                return 68; // Timeout
            } else {
                callback.imageCreatedCallback(meta);
                TimeUnit.MICROSECONDS.sleep(sourceTimeoutMicros / 2);
                callback.imageCompleteCallback(meta);
                return 0;
            }
        } catch (InterruptedException ex) {
            throw new DAQException("Interrupt while waiting for image");
        }
    }

    @Override
    public String decodeException(int rc) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public BitSet getConfiguredSources(long store) throws DAQException {
        return LocationSet.all().getBitSet();
    }

    @Override
    public Version getClientVersion() throws DAQException {
        return release;
    }

    @Override
    public void setRegisterList(long store, long camera, LocationType type, int[] registerList) throws DAQException {
        registerLists.put(type, registerList);
    }

    @Override
    public ImageMetaData triggerImage(long store, long camera, ImageMetaData meta) throws DAQException {
        Instant now = Instant.now();
        long id = random.nextLong();
        storeSimulation.fireTrigger(meta.getOpcode(), meta, registerLists);
        final ImageMetaData result = new ImageMetaData(id, release, now, meta);
        queue.offer(result);
        return result;
    }

    @Override
    public long startSequencer(long camera, int opcode) throws DAQException {
        Instant now = Instant.now();
        storeSimulation.fireTrigger(opcode, null, null);
        return toNanos(now);
    }

    private long toNanos(Instant instant) {
        return instant.getEpochSecond() * 1_000_000_000 + instant.getNano();
    }

    @Override
    public long attachGuider(String partition) throws DAQException {
        return 999;
    }

    @Override
    public void detachGuider(long guider) throws DAQException {
    }

    @Override
    public void startGuider(long guider, int rows, int cols, int integration, int binning, int[] roiData) {
    }

    @Override
    public void stopGuider(long guider) {
    }

    @Override
    public void pauseGuider(long guider) {
    }

    @Override
    public void resumeGuider(long guider) {
    }

    @Override
    public void waitForGuider(long subscriber) throws DAQException {
    }

    @Override
    public long attachGuiderSubscriber(Guider guider, String partition, int[] locations) throws DAQException {
        return 999;
    }

    @Override
    public void detachGuiderSubscriber(long subscriber) throws DAQException {
    }

}
