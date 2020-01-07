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
public class Statistics implements AutoCloseable {

    private final String partition;
    private final long rmsClient;
    //private final long rdsClient;

    private static final Logger LOG = Logger.getLogger(Statistics.class.getName());

    static {
        System.loadLibrary("ccs_daq_ims");
    }

    /**
     * Connects to a DAQ store.
     *
     * @param partition The name of the partition
     * @throws DAQException If the partition does not exist, or something else
     * goes wrong
     */
    public Statistics(String partition) throws DAQException {
        this.partition = partition;
        rmsClient = attachRmsClient(partition);
    }

    @Override
    public void close() throws DAQException {
        detachRmsClient(rmsClient);
    }

    // Native methods    
    private synchronized native long attachRmsClient(String partition) throws DAQException;
    private synchronized native void detachRmsClient(long rmsClient) throws DAQException;
    private synchronized native DaqRmsStats getRmsStats(long rmsClient) throws DAQException;

    static native String decodeException(int rc);
}
