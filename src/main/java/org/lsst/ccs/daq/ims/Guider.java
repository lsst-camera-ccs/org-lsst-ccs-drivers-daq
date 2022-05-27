package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;
import java.util.List;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.utilities.location.Location;

/**
 *
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

    public void start(ROICommon common, List<ROILocation> locations) throws DAQException {
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
        store.startGuider(guider, common.nRows, common.nCols, common.integrationTimeMilliSeconds, common.binning, roiData);
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

    public void stop() throws DAQException {
        store.stopGuider(guider);
    }

    public void pause() throws DAQException {
        store.pauseGuider(guider);
    }

    public void resume() throws DAQException {
        store.resumeGuider(guider);
    }
    
    public GuiderConfig config() throws DAQException {
        return store.guiderConfig(guider);
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
        LOG.log(Level.INFO, "stamp {0} {1}", new Object[]{state, rawStamp.remaining()});
    }

    void stampCallback(StateMetaData state, ByteBuffer stamp) {
        LOG.log(Level.INFO, "stamp {0} {1}", new Object[]{state, stamp.remaining()});
    }

    void detach() throws DAQException {
        store.detachGuider(this.guider);
    }

    public static class ROICommon {

        private final int nRows;
        private final int nCols;
        private final int integrationTimeMilliSeconds;
        private final int binning;

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

    public static class ROILocation {

        private final Location location;
        private final int sensor;
        private final int segment;
        private final int startRow;
        private final int startCol;

        public ROILocation(byte bay, byte board, int sensor, int segment, int startRow, int startCol) {
            this(new Location(bay, board), sensor, segment, startRow, startCol);
        }

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

    public static interface GuiderListener {

        void start(StateMetaData state, SeriesMetaData series);

        void stop(StateMetaData state);

        void pause(StateMetaData state);

        void resume(StateMetaData state);

        void stamp(StateMetaData state, ByteBuffer stamp);

        void rawStamp(StateMetaData state, ByteBuffer rawStamp);

    }

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

    public static class StateMetaData {

        public enum Type {
            STAMP, START, STOP, PAUSE, RESUME
        };

        public enum Status {
            SUCCESS, bdiError
        };
        private final Type type;
        private final Status status;
        private final long sequence;
        private final long timestamp;
        private final Location location;
        private final int sensor;

        private StateMetaData(int type, int status, int sequence, long timestamp, byte bay, byte board, int sensor) {
            this.type = Type.values()[type];
            this.status = Status.values()[status];
            this.sequence = sequence;
            this.timestamp = timestamp;
            this.location = new Location(bay, board);
            this.sensor = sensor;
        }

        @Override
        public String toString() {
            return "StateMetaData{" + "type=" + type + ", status=" + status + ", sequence=" + sequence + ", timestamp=" + timestamp + ", location=" + location + ", sensor=" + sensor + '}';
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

}
