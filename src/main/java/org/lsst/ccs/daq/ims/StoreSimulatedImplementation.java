package org.lsst.ccs.daq.ims;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Instant;
import java.util.BitSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;

import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.Status;
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
    private Path rawData;
    private SourceMetaData smd;

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
        int[][] result = new int[4 * 25][addresses.length];
        LocationSet locations = new LocationSet(bitset);
        for (Location l : locations) {
            for (int i = 0; i < addresses.length; i++) {
                result[l.index()][i] = storeSimulation.readRegister(l, addresses[i]);
            }
        }
        return result;
    }

    @Override
    public void writeRegisters(long client, BitSet bitset, int[] addresses, int[] values) throws DAQException {
        LocationSet locations = new LocationSet(bitset);
        for (Location l : locations) {
            for (int i = 0; i < addresses.length; i++) {
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
        return smd;
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
    public DAQSourceChannelImplementation openSourceChannelObject(long store, long id, int index, boolean write) throws DAQException {
        return new DAQSourceChannelSimulatedImplementation(rawData);
    }

    @Override
    public SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public long attachStream(long store, int sourceTimeoutMicros) throws DAQException {
        return 11111;
    }

    @Override
    public void detachStream(long stream) throws DAQException {
    }

    @Override
    public int waitForImage(Store callback, long store, long stream1, long stream2, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException {
        try {
            ImageMetaData meta = imageTimeoutMicros == 0 ? queue.take() : queue.poll(imageTimeoutMicros, TimeUnit.MICROSECONDS);
            if (meta == null) {
                return 68; // Timeout
            } else {
                callback.imageCreatedCallback(meta);
                TimeUnit.MILLISECONDS.sleep(2300);
                callback.imageCompleteCallback(meta);
                return 0;
            }
        } catch (InterruptedException ex) {
            throw new DAQException("Interrupt while waiting for image");
        }
    }

    @Override
    public String decodeException(int rc) {
        return "Unknown exception "+rc;
    }

    @Override
    public BitSet getConfiguredSources(long store) throws DAQException {
        return LocationSet.all().getBitSet();
    }

    @Override
    public BitSet getConfiguredLocations(String partition) throws DAQException {
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

    void simulateTrigger(ImageMetaData meta, Location location, int[] registerList, Path rawData) throws DAQException {
        try {
            this.rawData = rawData;
            this.smd = new SourceMetaData((byte) (location.type().ordinal()+1), (byte)0, "simulation", release, 0, 0, Files.size(rawData),  location.getBay(), location.getBoard(), registerList);
            setRegisterList(0, 0, location.type(), registerList);
            triggerImage(0, 0, meta);
        } catch (IOException x) {
            throw new DAQException("Blah");
        }
    }

    @Override
    public long attachGuider(String partition) throws DAQException {
        return 999;
    }

    @Override
    public void detachGuider(long guider) throws DAQException {
    }

    @Override
    public Status startGuider(long guider, int rows, int cols, int integration, int overRows, int underCols, int overCols, int flushCount, String id, int[] roiData) {
        return null;
    }

    @Override
    public void validateGuider(long guider, int nRows, int nCols, int integrationTimeMilliSeconds, int[] roiData) throws DAQException {
    }

    @Override
    public Status stopGuider(long guider, String comment) {
        return null;
    }

    @Override
    public Status pauseGuider(long guider, String comment) {
        return null;
    }

    @Override
    public Status resumeGuider(long guider, String comment) {
        return null;
    }

    @Override
    public Status sleepGuider(long guider) {
        return null;
    }

    @Override
    public Status wakeGuider(long guider, int delay, int preRows, int flushCount, int readRows, int postRows, int overRows) {
        return null;
    }

    @Override
    public void waitForGuider(long subscriber, Guider.Subscriber callback) throws DAQException {
    }

    @Override
    public void abortWaitForGuider(long subscriber) throws DAQException {
    }

    @Override
    public long attachGuiderSubscriber(String partition, boolean bigEndian, int[] locations) throws DAQException {
        return 999;
    }

    @Override
    public void detachGuiderSubscriber(long subscriber) throws DAQException {
    }

    @Override
    public Config guiderConfig(long guider) throws DAQException {
        return null;
    }

    @Override
    public SeriesStatus guiderSeries(long guider) throws DAQException {
        return null;
    }

    @Override
    public Status clearGuider(long guider, int delay, int preRows, int flushCount, int readRows, int postRows, int overRows) throws DAQException {
        return null;
    }

    @Override
    public String getClientPlatform(String partition) {
        return "unknown";
    }
}
