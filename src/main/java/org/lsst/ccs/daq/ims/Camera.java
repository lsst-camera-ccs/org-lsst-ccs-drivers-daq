package org.lsst.ccs.daq.ims;

import java.time.Instant;
import java.util.HashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

/**
 * Class used for triggering camera. This implementation uses the DAQ driver
 * to actually trigger the DAQ.
 * @author tonyj
 */
public class Camera {
    private final Store store;
    public final static int OPCODE_STOP = 31;
    private final Map<Location.LocationType, int[]> registerLists = new HashMap<>();
    
    Camera(Store store) {
        this.store = store;
    }
    
    /**
     * Set the registers used for reading meta-data when an image is triggered
     * @param rebType The type of REB for which the registers should be set
     * @param registerAddresses The register addresses
     * @throws DAQException 
     */
    public void setRegisterList(Location.LocationType rebType, int[] registerAddresses) throws DAQException {
        registerLists.put(rebType, registerAddresses);
    }
   
    /**
     * Trigger an image acquisition
     * @param meta The meta-data to be associated with the triggered image
     * @return The created image
     * @throws DAQException 
     */
    public Image triggerImage(ImageMetaData meta) throws DAQException {
        return new Image(store, store.triggerImage(meta, registerLists));
    }
   
    /**
     * Start the sequencers using the given opcode. This is used to start
     * the sequencers without generating any image data (e.g. for clear).
     * @param opcode The opcode to trigger
     * @return The timestamp from when the sequencers were started.
     * @throws DAQException 
     */
    public Instant startSequencer(int opcode) throws DAQException {
        long timestampNanos = store.startSequencer(opcode);
        return Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
    }
}
