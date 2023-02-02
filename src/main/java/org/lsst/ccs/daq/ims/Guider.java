package org.lsst.ccs.daq.ims;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.time.Instant;
import java.util.List;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import nom.tam.fits.BasicHDU;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.FitsUtil;
import nom.tam.fits.Header;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter.FileNamer;
import org.lsst.ccs.imagenaming.ImageName;
import org.lsst.ccs.utilities.image.FitsCheckSum;
import org.lsst.ccs.utilities.image.HeaderSpecification;
import org.lsst.ccs.utilities.image.HeaderWriter;
import org.lsst.ccs.utilities.image.MetaDataSet;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.taitime.CCSTimeStamp;

/**
 * An interface to the DAQ guider.
 *
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
     *
     * @param common The common settings for this start command
     * @param id The id of this sequence of guider images. From CCS this will
     * normally be the OBSID.
     * @param locations The set of ROIs to start
     * @return The status after the command completes
     * @throws DAQException
     */
    public Status start(ROICommon common, String id, List<ROILocation> locations) throws DAQException {
        final int nLocs = locations.size();
        int[] roiData = new int[nLocs * 5];
        for (int i = 0; i < nLocs; i++) {
            int j = i * 5;
            ROILocation location = locations.get(i);
            roiData[j] = location.location.rebLocation.index();
            roiData[j + 1] = location.location.sensor;
            roiData[j + 2] = location.segment;
            roiData[j + 3] = location.startRow;
            roiData[j + 4] = location.startCol;
        }
        System.out.println(Arrays.toString(roiData));
        return store.startGuider(guider, common.nRows, common.nCols, common.integrationTimeMilliSeconds, id, roiData);
    }

    public Subscriber subscribe(Set<SensorLocation> locations, ByteOrder byteOrder, GuiderListener listener) throws DAQException {
        return new Subscriber(store, locations, byteOrder, listener);
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

    public Config config() throws DAQException {
        return store.guiderConfig(guider);
    }

    public Series series() throws DAQException {
        return store.guiderSeries(guider);
    }

    void detach() throws DAQException {
        store.detachGuider(this.guider);
    }

    public static class Subscriber implements AutoCloseable {

        private final long subscriber;
        private final Store store;
        private final GuiderListener listener;

        private Subscriber(Store store, Set<SensorLocation> locations, ByteOrder byteOrder, GuiderListener listener) throws DAQException {
            this.store = store;
            this.listener = listener;
            boolean bigEndian = byteOrder == ByteOrder.BIG_ENDIAN;
            int[] locs = new int[locations.size() * 2];
            int index = 0;
            for (SensorLocation sl : locations) {
                locs[index++] = sl.getRebLocation().index();
                locs[index++] = sl.getSensor();
            }
            this.subscriber = store.attachGuiderSubscriber(store.getPartition(), bigEndian, locs);
        }

        public void waitForGuider() throws DAQException {
            store.waitForGuider(subscriber, this);
        }

        private void startCallback(StateMetaData state, SeriesMetaData series) {
            LOG.log(Level.INFO, "start {0} {1}", new Object[]{state, series});
            try {
                listener.start(state, series);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "start callback failed", x);
            }
        }

        void stopCallback(StateMetaData state) {
            LOG.log(Level.INFO, "stop {0}", state);
            try {
                listener.stop(state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "stop callback failed", x);
            }

        }

        void pauseCallback(StateMetaData state) {
            LOG.log(Level.INFO, "pause {0}", state);
            try {
                listener.pause(state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "pause callback failed", x);
            }
        }

        void resumeCallback(StateMetaData state) {
            try {
                LOG.log(Level.INFO, "resume {0}", state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "resume callback failed", x);
            }
        }

        void rawStampCallback(StateMetaData state, ByteBuffer rawStamp) {
            LOG.log(Level.INFO, "rawStamp {0} {1}", new Object[]{state, rawStamp.remaining()});
            try {
                listener.rawStamp(state, rawStamp);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "rawStamp callback failed", x);
            }
        }

        void stampCallback(StateMetaData state, ByteBuffer stamp) {
            LOG.log(Level.INFO, "stamp {0} {1}", new Object[]{state, stamp.remaining()});
            try {
                listener.stamp(state, stamp);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "stamp callback failed", x);
            }
        }

        @Override
        public void close() throws DAQException {
            store.detachGuiderSubscriber(subscriber);
        }
    }

    /**
     * ROI parameters which are shared by all ROIs.
     */
    public static class ROICommon {

        private final int nRows;
        private final int nCols;
        private final int integrationTimeMilliSeconds;

        /**
         * Create an instance of ROICommand
         *
         * @param nRows The number of rows
         * @param nCols The number of columns
         * @param integrationTimeMilliSeconds The integration time in
         * milliseconds
         */
        public ROICommon(int nRows, int nCols, int integrationTimeMilliSeconds) {
            this.nRows = nRows;
            this.nCols = nCols;
            this.integrationTimeMilliSeconds = integrationTimeMilliSeconds;
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

        @Override
        public String toString() {
            return "ROICommon{" + "nRows=" + nRows + ", nCols=" + nCols + ", integrationTimeMilliSeconds=" + integrationTimeMilliSeconds + '}';
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

        public Location getRebLocation() {
            return rebLocation;
        }

        public int getSensor() {
            return sensor;
        }

        @Override
        public int hashCode() {
            int hash = 7;
            hash = 83 * hash + Objects.hashCode(this.rebLocation);
            hash = 83 * hash + this.sensor;
            return hash;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (getClass() != obj.getClass()) {
                return false;
            }
            final SensorLocation other = (SensorLocation) obj;
            if (this.sensor != other.sensor) {
                return false;
            }
            return Objects.equals(this.rebLocation, other.rebLocation);
        }

        @Override
        public String toString() {
            return rebLocation.toString() + rebLocation.getSensorName(sensor);
        }

    }

    /**
     * Details of an ROI for a single sensor
     */
    public static class ROILocation {

        private final SensorLocation location;
        private final int segment;
        private final int startRow;
        private final int startCol;

        /**
         * Create an ROI for a single CCD
         *
         * @param bay The bay
         * @param board The board
         * @param sensor The sensor
         * @param segment The segment in which the ROI starts
         * @param startRow The start row
         * @param startCol The start column
         */
        public ROILocation(byte bay, byte board, int sensor, int segment, int startRow, int startCol) {
            this(new SensorLocation(bay, board, sensor), segment, startRow, startCol);
        }

        /**
         * Create an ROI for a single CCD
         *
         * @param location The Sensor location
         * @param segment The segment in which the ROI starts
         * @param startRow The start row
         * @param startCol The start column
         */
        public ROILocation(SensorLocation location, int segment, int startRow, int startCol) {
            this.location = location;
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

        public SensorLocation getLocation() {
            return location;
        }

        public int getSegment() {
            return segment;
        }

        @Override
        public String toString() {
            return "ROILocation{" + "location=" + location + ", segment=" + segment + ", startRow=" + startRow + ", startCol=" + startCol + '}';
        }
    }

    /**
     * A listener for guider events
     */
    public static interface GuiderListener {

        /**
         * Guider is started
         *
         * @param state The meta-data at start time
         * @param series The meta-data corresponding to the series of postage
         * stamps
         * @throws java.lang.Exception
         */
        void start(StateMetaData state, SeriesMetaData series) throws Exception;

        void stop(StateMetaData state) throws Exception;

        void pause(StateMetaData state) throws Exception;

        void resume(StateMetaData state) throws Exception;

        /**
         * Called each time there is a single postage stamp
         *
         * @param state The current state, including the timestamp of the stamp
         * @param stamp The data corresponding to the stamp (in what format??)
         */
        void stamp(StateMetaData state, ByteBuffer stamp) throws Exception;

        /**
         * Called each time there is a new postage stamp
         *
         * @param state The current state, including the timestamp of the stamp
         * @param rawStamp The data corresponding the the raw postage stamp.
         */
        void rawStamp(StateMetaData state, ByteBuffer rawStamp) throws Exception;

    }

    /**
     * Meta-data common to a series of readouts from a single ROI
     */
    public static class SeriesMetaData {

        private final ROICommon common;
        private final ROILocation location;
        private final Version version;
        private final int firmware;
        private final long serialNumber;
        private final String id;
        private final String platform;

        private SeriesMetaData(int firmware, long serialNumber, String id, String platform, ROICommon common, ROILocation location, Version version) {
            this.firmware = firmware;
            this.serialNumber = serialNumber;
            this.common = common;
            this.location = location;
            this.version = version;
            this.id = id;
            this.platform = platform;
        }

        public ROICommon getCommon() {
            return common;
        }

        public ROILocation getLocation() {
            return location;
        }

        public Version getVersion() {
            return version;
        }

        public int getFirmware() {
            return firmware;
        }

        public long getSerialNumber() {
            return serialNumber;
        }

        public String getId() {
            return id;
        }

        public String getPlatform() {
            return platform;
        }

        @Override
        public String toString() {
            return "SeriesMetaData{" + "common=" + common + ", location=" + location + ", version=" + version + ", firmware=" + firmware + ", serialNumber=" + serialNumber + ", id=" + id + ", platform=" + platform + '}';
        }

    }

    public static class Status {

        public enum State {
            UNDEFINED, STOPPED, PAUSED, RUNNING, ERROR, OFF
        };

        private final Instant timestamp;
        private final int status;
        private final int sequence;
        private final State in;
        private final State out;

        private Status(long timestampNanos, int status, int sequence, int inState, int outState) {
            this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
            this.status = status;
            this.sequence = sequence;
            this.in = State.values()[inState];
            this.out = State.values()[outState];
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
        private final SensorLocation sensorLocation;

        private StateMetaData(int type, int status, int sequence, long timestampNanos, byte bay, byte board, int sensor) {
            this.state = State.values()[type];
            this.status = Status.values()[status];
            this.sequence = sequence;
            this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
            this.sensorLocation = new SensorLocation(new Location(bay, board), sensor);
        }

        public State getState() {
            return state;
        }

        public Status getStatus() {
            return status;
        }

        public long getSequence() {
            return sequence;
        }

        public Instant getTimestamp() {
            return timestamp;
        }

        public SensorLocation getSensorLocation() {
            return sensorLocation;
        }

        @Override
        public String toString() {
            return "StateMetaData{" + "state=" + state + ", status=" + status + ", sequence=" + sequence + ", timestamp=" + timestamp + ", sensorLocation=" + sensorLocation + '}';
        }
    }

    public static class Config {

        private final Series series;
        private final ROICommon common;
        private final List<ROILocation> locations;

        private Config(Series series, ROICommon common, List<ROILocation> locations) {
            this.series = series;
            this.common = common;
            this.locations = locations;
        }

        @Override
        public String toString() {
            return "Config{" + "series=" + series + ", common=" + common + ", locations=" + locations + '}';
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

    public static class FitsWriter implements GuiderListener {

        private final String partition;
        private final FileNamer fileNamer;
        private final Map<String, HeaderSpecification> headerSpecifications;
        private BufferedFile bufferedFile;
        private Map<String, Object> properties;
        private final SensorLocation sensorLocation;

        public FitsWriter(String partition, SensorLocation location, FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications) {
            this.partition = partition;
            this.fileNamer = fileNamer;
            this.headerSpecifications = headerSpecifications;
            this.sensorLocation = location;
        }

        @Override
        public void start(StateMetaData state, SeriesMetaData series) throws IOException, FitsException {
            // Start must be issued after the FitsWriter is installed as a listener
            Map<String, Object> props = new HashMap<>();
            // ToDo: Handle non standard id
            ImageName imageName = new ImageName(series.getId());
            props.put("ImageName", imageName.toString());
            props.put("ImageDate", imageName.getDateString());
            props.put("ImageNumber", imageName.getNumberString());
            props.put("ImageController", imageName.getController().getCode());
            props.put("ImageSource", imageName.getSource().getCode());

            ROILocation roiLocation = series.getLocation();
            SensorLocation sensorLocation = roiLocation.getLocation();
            Location rebLocation = sensorLocation.getRebLocation();
            ROICommon common = series.getCommon();

            props.put("FileCreationTime", CCSTimeStamp.currentTime());
            props.put("RaftBay", rebLocation.getRaftName());
            props.put("RebSlot", rebLocation.getBoardName());
            props.put("DAQPartition", partition);
            props.put("IntegrationTime", common.getIntegrationTimeMilliSeconds());
            props.put("ROISegment", String.format("Segment%02d", roiLocation.getSegment()));
            props.put("ROICols", common.getnCols());
            props.put("ROIRows", common.getnRows());
            props.put("Firmware", String.format("%x", series.getFirmware()));
            props.put("CCDControllerSerial", String.format("%x", series.getSerialNumber() & 0xFFFFFFFFL));
            props.put("DAQVersion", series.getVersion().toString());
            props.put("Platform", series.getPlatform());

            props.put("StartTime", state.getTimestamp());
            props.put("DAQSequence", state.getSequence());

            props.put("CCDSlot", rebLocation.getSensorName(sensorLocation.getSensor()));
            File computedFileName = fileNamer.computeFileName(props);

            // Open the file and write the primary header
            BufferedFile bf = new BufferedFile(computedFileName, "rw");
            BasicHDU primary = BasicHDU.getDummyHDU();
            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("primary", props);
            HeaderWriter.addMetaDataToHeader(computedFileName, primary, headerSpecifications.get("primary"), metaDataSet);
            FitsCheckSum.setChecksum(primary);
            primary.write(bf);
            this.bufferedFile = bf;
            this.properties = props;
        }

        @Override
        public void stop(StateMetaData state) {
            try {
                if (bufferedFile != null) {
                    bufferedFile.close();
                    bufferedFile = null;
                }
            } catch (IOException ex) {
                LOG.log(Level.WARNING, "Error closing FITS file", ex);
            }
        }

        @Override
        public void pause(StateMetaData state) {
        }

        @Override
        public void resume(StateMetaData state) {
        }

        @Override
        public void stamp(StateMetaData state, ByteBuffer stamp) throws FitsException, IOException {
            Map<String, Object> props = new HashMap<>();
            props.put("StampTime", state.getTimestamp());

            int[][] intDummyData = new int[1][1];
            BasicHDU imageHDU = FitsFactory.hduFactory(intDummyData);
            Header header = imageHDU.getHeader();
            header.setXtension("IMAGE");
            header.setBitpix(32);
            header.setNaxes(2);
            header.setNaxis(1, (int) properties.get("ROIRows"));
            header.setNaxis(2, (int) properties.get("ROICols"));

            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("stamp", props);
            HeaderWriter.addMetaDataToHeader(null, imageHDU, headerSpecifications.get("stamp"), metaDataSet);
            FitsCheckSum.setChecksum(imageHDU);
            //long computeChecksum = FitsCheckSum.computeChecksum(stamp);
            //FitsCheckSum.updateDataSum(header, computeChecksum);
            long imageSize = stamp.remaining();
            header.write(bufferedFile);
            bufferedFile.getChannel().write(stamp);
            FitsUtil.pad(bufferedFile, imageSize);
        }

        @Override
        public void rawStamp(StateMetaData state, ByteBuffer rawStamp) {
            // Ignored for now, probably forever
        }

    }

}
