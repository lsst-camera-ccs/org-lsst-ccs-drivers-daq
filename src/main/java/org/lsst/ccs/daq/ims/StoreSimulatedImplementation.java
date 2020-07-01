package org.lsst.ccs.daq.ims;

import java.time.Instant;
import java.util.BitSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;
import org.lsst.ccs.utilities.location.Location.LocationType;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A simulated implementation of the Store API. Used for testing and running
 * subsystem without a real DAQ.
 *
 * @author tonyj
 */
class StoreSimulatedImplementation implements StoreImplementation {

    private final Map<LocationType, int[]> registerLists = new ConcurrentHashMap<>();
    StoreSimulation storeSimulation = StoreSimulation.instance();
    private final Random random = new Random();
    private final Version release = new Version("daq-simulation", toNanos(Instant.now()), false, 12345);

    @Override
    public long attachStore(String partition) throws DAQException {
        return 100;
    }

    @Override
    public void detachStore(long store) throws DAQException {

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
    public int waitForImage(long store, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
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
    public void setRegisterList(long store, LocationType rebType, int[] registerAddresses) throws DAQException {
        registerLists.put(rebType, registerAddresses);
    }

    @Override
    public ImageMetaData triggerImage(long store, ImageMetaData meta) throws DAQException {
        Instant now = Instant.now();
        long id = random.nextLong();
        storeSimulation.fireTrigger(meta.getOpcode(), meta, registerLists);
        return new ImageMetaData(id, release, now, meta);
    }

    @Override
    public long startSequencer(long store, int opcode) throws DAQException {
        Instant now = Instant.now();
        storeSimulation.fireTrigger(opcode, null, registerLists);
        return toNanos(now);
    }

    private long toNanos(Instant instant) {
        return instant.getEpochSecond() * 1_000_000_000 + instant.getNano();
    }

}
