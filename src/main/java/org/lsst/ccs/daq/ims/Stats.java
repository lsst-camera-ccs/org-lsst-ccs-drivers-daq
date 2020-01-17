package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.util.ArrayList;
import java.util.List;
import java.util.BitSet;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A connection to the Statistics data within a specified DAQ partition. 
 *
 * @author  Al Eisner
 */
public class Stats implements AutoCloseable {

    private final String partition;
    private final long rmsClient;
    //private final long rdsClient;

    private static final Logger LOG = Logger.getLogger(Stats.class.getName());

    static {
        System.loadLibrary("ccs_daq_ims");
    }

   /**
    * Connects to DAQ Statistics clients
    *
    * @param partition The name of the partition
    * @throws DAQException If the partition does not exist, or something else
    * goes wrong
    */
    public Stats(String partition) throws DAQException {
        this.partition = partition;
        rmsClient = attachRmsClient(partition);
    }

   /**
    * Detaches DAQ clients
    *
    * @throws DAQException 
    */
    @Override
    public void close() throws DAQException {
        detachRmsClient(rmsClient);
    }

   /**
    * Get DAQ Rms Statistics for specified Location
    *
    * @param  Location requested (25 rafts times 3 REB)
    * @return DAQRmsStats object
    * @throws DAQException
    */
    public DAQRmsStats getDAQRmsStats(Location location) throws DAQException {
        return getRmsStats(rmsClient, location.index());
    }

    /* Native methods  */

    private synchronized native long attachRmsClient(String partition) throws DAQException;
    private synchronized native void detachRmsClient(long rmsClient) throws DAQException;
    private synchronized native DAQRmsStats getRmsStats(long rmsClient, int index) throws DAQException;

    static native String decodeException(int rc);
}
