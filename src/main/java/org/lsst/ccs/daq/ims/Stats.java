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
    private final long rdsClient;

    private static final Logger LOG = Logger.getLogger(Stats.class.getName());

    static {
        System.loadLibrary("ccs_daq_ims");
    }

   /**
    * Enumerate choices for clearing data after reading it or not
    */

    public enum Clear {

        YES  (true),
        NO   (false);

        private boolean value;

        Clear (boolean value) {
            this.value = value;
        }

        public boolean getValue() {
            return value;
       }
    }

   /**
    * Connect to DAQ Statistics clients
    *
    * @param partition The name of the partition
    * @throws DAQException If the partition does not exist, or something else
    * goes wrong
    */
    public Stats(String partition) throws DAQException {
        this.partition = partition;
        rmsClient = attachRmsClient(partition);
        rdsClient = attachRdsClient(partition);
    }

   /**
    * Detaches DAQ clients
    *
    * @throws DAQException 
    */
    @Override
    public void close() throws DAQException {
        detachRmsClient(rmsClient);
        detachRdsClient(rdsClient);
    }

   /**
    * Get DAQ Rms Statistics for specified Location
    *
    * @param  Location requested (25 rafts times 3 REB)
    * @param  Specify whether to clear data aftef reading it 
    * @return DAQRmsStats object
    * @throws DAQException
    */
    public DAQRmsStats getDAQRmsStats(Location location, Clear clear) 
    throws DAQException {
        return getRmsStats(rmsClient, location.index(), clear.getValue());
    }

   /**
    * Get DAQ Rds Statistics for specified Location
    *
    * @param  Location requested (25 rafts times 3 REB)
    * @param  Specify whether to clear data aftef reading it 
    * @return DAQRdsStats object
    * @throws DAQException
    */
    public DAQRdsStats getDAQRdsStats(Location location, Clear clear) 
    throws DAQException {
        return getRdsStats(rdsClient, location.index(), clear.getValue());
    }

   /**
    * Get DAQ Driver Statistics for specified Location
    *
    * @param  Location requested (25 rafts times 3 REB)
    * @param  Specify whether to clear data aftef reading it 
    * @return DAQDriverStats object
    * @throws DAQException
    */
    public DAQDriverStats getDAQDriverStats(Location location, Clear clear) 
    throws DAQException {
        return getDriverStats(rmsClient, location.index(), clear.getValue());
    }

   /**
    * Get DAQ Firmware Statistics for specified Location
    *
    * @param  Location requested (25 rafts times 3 REB)
    * @param  Specify whether to clear data aftef reading it 
    * @return DAQFirmwareStats object
    * @throws DAQException
    */
    public DAQFirmwareStats getDAQFirmwareStats(Location location, Clear clear)
    throws DAQException {
        return getFirmwareStats(rmsClient, location.index(), clear.getValue());
    }

    /* Native methods  */

    private synchronized native long attachRmsClient(String partition) throws DAQException;
    private synchronized native long attachRdsClient(String partition) throws DAQException;
    private synchronized native void detachRmsClient(long rmsClient) throws DAQException;
    private synchronized native void detachRdsClient(long rdsClient) throws DAQException;
    private synchronized native DAQRmsStats getRmsStats(long rmsClient, int index, boolean clearStats) throws DAQException;
    private synchronized native DAQRdsStats getRdsStats(long rdsClient, int index, boolean clearStats) throws DAQException;
    private synchronized native DAQDriverStats getDriverStats(long rmsClient, int index, boolean clearStats) throws DAQException;
    private synchronized native DAQFirmwareStats getFirmwareStats(long rmsClient, int index, boolean clearStats) throws DAQException;

    static native String decodeException(int rc);
}
