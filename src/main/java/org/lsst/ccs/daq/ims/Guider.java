package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;
import java.time.Instant;
import java.util.List;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.utilities.location.Location;

/**
 * An interface to the DAQ guider. 
 * @see Store#getGuider() 
 * @author tonyj
 */
public class Guider {

    private static final Logger LOG = Logger.getLogger(Guider.class.getName());

    private final Store store;
    private final long guider;

    Guider(Store store, long guider) {
        this.store = store;
        this.guider = guider;
    }

    /**
     * Start the guider, using the specified ROI locations
     * @param common
     * @param locations
     * @throws DAQException 
     */
    public Status start(ROICommon common, List<ROILocation> locations) throws DAQException {
        final int nLocs = locations.size();
        int[] roiData = new int[nLocs * 5];
        for (int i = 0; i < nLocs; i++) {
            int j = i * 5;
            ROILocation location = locations.get(i);
            roiData[j] = location.location.index();
            roiData[j + 1] = location.sensor;
            roiData[j + 2] = location.segment;
            roiData[j + 3] = location.startRow;
            roiData[j + 4] = location.startCol;
        }
        System.out.println(Arrays.toString(roiData));
        return store.startGuider(guider, common.nRows, common.nCols, common.integrationTimeMilliSeconds, common.binning, roiData);
    }

    public void listen(Location location, int sensor) throws DAQException {
        long subscriber = store.attachGuiderSubscriber(store.getPartition(), new int[]{location.index(), sensor});
        try {
            for (;;) {
                store.waitForGuider(subscriber, this);
            }
        } finally {
            store.detachGuiderSubscriber(subscriber);
        }
    }

    public Status stop() throws DAQException {
        return store.stopGuider(guider);
    }

    public Status pause() throws DAQException {
        return store.pauseGuider(guider);
    }

    public Status resume() throws DAQException {
        return store.resumeGuider(guider);
    }

    public Status sleep() throws DAQException {
        return store.sleepGuider(guider);
    }

    public Status wake() throws DAQException {
        return store.wakeGuider(guider);
    }
    
    public GuiderConfig config() throws DAQException {
        return store.guiderConfig(guider);
    }

    public Series series() throws DAQException {
        return store.guiderSeries(guider);
    }
    
    public void addGuiderListener(GuiderListener listener) {

    }

    public void removeGuiderListener(GuiderListener listener) {

    }

    void startCallback(StateMetaData state, SeriesMetaData series) {
        LOG.log(Level.INFO, "start {0} {1}", new Object[]{state, series});
    }

    void stopCallback(StateMetaData state) {
        LOG.log(Level.INFO, "stop {0}", state);
    }

    void pauseCallback(StateMetaData state) {
        LOG.log(Level.INFO, "pause {0}", state);
    }

    void resumeCallback(StateMetaData state) {
        LOG.log(Level.INFO, "resume {0}", state);

    }

    void rawStampCallback(StateMetaData state, ByteBuffer rawStamp) {
        LOG.log(Level.INFO, "rawStamp {0} {1}", new Object[]{state, rawStamp.remaining()});
    }

    void stampCallback(StateMetaData state, ByteBuffer stamp) {
        LOG.log(Level.INFO, "stamp {0} {1}", new Object[]{state, stamp.remaining()});
    }

    void detach() throws DAQException {
        store.detachGuider(this.guider);
    }

    /**
     * ROI parameters which are shared by all ROIs.
     */
    
    public static class ROICommon {

        private final int nRows;
        private final int nCols;
        private final int integrationTimeMilliSeconds;
        private final int binning;

        /**
         * Create an instance of ROICommand
         * @param nRows The number of rows
         * @param nCols The number of columns
         * @param integrationTimeMilliSeconds The integration time in milliseconds
         * @param binning Binning (no longer supported, must be 1)
         */
        public ROICommon(int nRows, int nCols, int integrationTimeMilliSeconds, int binning) {
            this.nRows = nRows;
            this.nCols = nCols;
            this.integrationTimeMilliSeconds = integrationTimeMilliSeconds;
            this.binning = binning;
        }

        public int getnRows() {
            return nRows;
        }

        public int getnCols() {
            return nCols;
        }

        public int getIntegrationTimeMilliSeconds() {
            return integrationTimeMilliSeconds;
        }

        public int getBinning() {
            return binning;
        }

        @Override
        public String toString() {
            return "ROICommon{" + "nRows=" + nRows + ", nCols=" + nCols + ", integrationTimeMilliSeconds=" + integrationTimeMilliSeconds + ", binning=" + binning + '}';
        }
    }
    
    public static class SensorLocation {
        private final Location rebLocation;
        private final int sensor;

        public SensorLocation(byte bay, byte board, int sensor) {
            this(new Location(bay, board), sensor);
        }
        
        public SensorLocation(Location rebLocation, int sensor) {
            this.rebLocation = rebLocation;
            this.sensor = sensor;
        }

        @Override
        public String toString() {
            return "SensorLocation{" + "rebLocation=" + rebLocation + ", sensor=" + sensor + '}';
        }
        
    }

    /**
     * Details of an ROI for a single sensor
     */
    public static class ROILocation {

        private final Location location;
        private final int sensor;
        private final int segment;
        private final int startRow;
        private final int startCol;

        /**
         * Create an ROI for a single CCD
         * @param bay The bay
         * @param board The board
         * @param sensor The sensor
         * @param segment The segment in which the ROI starts
         * @param startRow The start row
         * @param startCol The start column
         */
        public ROILocation(byte bay, byte board, int sensor, int segment, int startRow, int startCol) {
            this(new Location(bay, board), sensor, segment, startRow, startCol);
        }

        /**
         * Create an ROI for a single CCD
         * @param location The REB location
         * @param sensor The sensor
         * @param segment The segment in which the ROI starts
         * @param startRow The start row
         * @param startCol The start column
         */
        public ROILocation(Location location, int sensor, int segment, int startRow, int startCol) {
            this.location = location;
            this.sensor = sensor;
            this.segment = segment;
            this.startRow = startRow;
            this.startCol = startCol;
        }

        public int getStartRow() {
            return startRow;
        }

        public int getStartCol() {
            return startCol;
        }

        public Location getLocation() {
            return location;
        }

        public int getSensor() {
            return sensor;
        }

        public int getSegment() {
            return segment;
        }
    }

    /**
     * A listener for guider events
     */
    public static interface GuiderListener {

        /**
         * Guider is started
         * @param state The meta-data at start time
         * @param series The meta-data corresponding to the series of postage stamps
         */
        void start(StateMetaData state, SeriesMetaData series);

        void stop(StateMetaData state);

        void pause(StateMetaData state);

        void resume(StateMetaData state);
        
        /**
         * Called each time there is a single postage stamp
         * @param state The current state, including the timestamp of the stamp
         * @param stamp The data corresponding to the stamp (in what format??)
         */

        void stamp(StateMetaData state, ByteBuffer stamp);

        /**
         * Called each time there is a new postage stamp
         * @param state The current state, including the timestamp of the stamp 
         * @param rawStamp The data corresponding the the raw postage stamp.
         */
        void rawStamp(StateMetaData state, ByteBuffer rawStamp);

    }

    /** 
     * Meta-data common to a series of ROI readouts
     */
    public static class SeriesMetaData {

        private final ROICommon common;
        private final ROILocation location;
        private final Version version;
        private final int firmware;
        private final long serialNumber;

        private SeriesMetaData(int firmware, long serialNumber, ROICommon common, ROILocation location, Version version) {
            this.firmware = firmware;
            this.serialNumber = serialNumber;
            this.common = common;
            this.location = location;
            this.version = version;
        }

        @Override
        public String toString() {
            return "SeriesMetaData{" + "common=" + common + ", location=" + location + ", version=" + version + ", firmware=" + firmware + ", serialNumber=" + serialNumber + '}';
        }

    }

    public static class Status {

        private final Instant timestamp;
        private final int  status;
        private final int  sequence;
        private final StateMetaData.State in;
        private final StateMetaData.State out;

        private Status(long timestampNanos, int status, int sequence, int inState, int outState) {
            this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
            this.status = status;
            this.sequence = sequence;
            this.in = StateMetaData.State.values()[inState];
            this.out = StateMetaData.State.values()[outState];
        }

        @Override
        public String toString() {
            return "Status{" + "timestamp=" + timestamp + ", status=" + status + ", sequence=" + sequence + ", in=" + in + ", out=" + out + '}';
        }
    }
    
    
    /**
     * Meta-data corresponding to the instantaneous state of the guider
     */
    public static class StateMetaData {

        public enum State {
            STAMP, START, STOP, PAUSE, RESUME
        };

        public enum Status {
            SUCCESS, bdiError
        };
        private final State state;
        private final Status status;
        private final long sequence;
        private final Instant timestamp;
        private final Location location;
        private final int sensor;

        private StateMetaData(int type, int status, int sequence, long timestampNanos, byte bay, byte board, int sensor) {
            this.state = State.values()[type];
            this.status = Status.values()[status];
            this.sequence = sequence;
            this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
            this.location = new Location(bay, board);
            this.sensor = sensor;
        }

        @Override
        public String toString() {
            return "StateMetaData{" + "type=" + state + ", status=" + status + ", sequence=" + sequence + ", timestamp=" + timestamp + ", location=" + location + ", sensor=" + sensor + '}';
        }

    }

    public static class GuiderConfig {

        private final ROICommon common;
        private final List<ROILocation> locations;

        private GuiderConfig(ROICommon common, List<ROILocation> locations) {
            this.common = common;
            this.locations = locations;
        }

        @Override
        public String toString() {
            return "GuiderConfig{" + "common=" + common + ", locations=" + locations + '}';
        }
    }

    public static class Series {

        private final Status status;
        private final Instant begin;
        private final int sequence;
        private final int stamps;
        private final List<SensorLocation> configured;
        private final List<SensorLocation> remaining;

        private Series(Status status, long timestampNanos, int sequence, int stamps, List<SensorLocation> configured, List<SensorLocation> remaining) {
            this.status = status;
            this.begin = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
            this.sequence = sequence;
            this.stamps = stamps;
            this.configured = configured;
            this.remaining = remaining;
        }

        @Override
        public String toString() {
            return "Series{" + "status=" + status + ", begin=" + begin + ", sequence=" + sequence + ", stamps=" + stamps + ", configured=" + configured + ", remaining=" + remaining + '}';
        }

    }
    
}
