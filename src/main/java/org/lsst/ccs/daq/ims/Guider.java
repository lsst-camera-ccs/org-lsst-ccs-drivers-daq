package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.lsst.ccs.daq.guider.ClearParameters;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.GuiderListener;
import org.lsst.ccs.daq.guider.ROICommon;
import org.lsst.ccs.daq.guider.ROICommonExtended;
import org.lsst.ccs.daq.guider.ROILocation;
import org.lsst.ccs.utilities.location.SensorLocation;
import org.lsst.ccs.daq.guider.SeriesMetaData;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.StateMetaData;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * An interface to the DAQ guider.
 *
 * @see Store#getGuider()
 * @author tonyj
 */
public class Guider {

    private static final Logger LOG = Logger.getLogger(Guider.class.getName());

    // The GDS command timeout error code. Kept in sync with the DAQ enum
    // gds/Exception.hh: enum Id { COMMAND_TIMEOUT=-1, ... }
    private static final int COMMAND_TIMEOUT = -1;

    static final int DEFAULT_MAX_RETRIES = 1;

    private final Store store;
    private final long guider;
    private final int maxRetries;

    public Guider(Store store, long guider) {
        this(store, guider, DEFAULT_MAX_RETRIES);
    }

    /**
     * Create a Guider that will retry state-changing commands that fail with a
     * command timeout.
     *
     * @param store The owning Store
     * @param guider The native guider handle
     * @param maxRetries The maximum number of retries (extra attempts after the
     * first failure) to perform when a state-changing command fails with a
     * command timeout. A value of 0 disables retries.
     */
    public Guider(Store store, long guider, int maxRetries) {
        this.store = store;
        this.guider = guider;
        this.maxRetries = maxRetries;
    }

    @FunctionalInterface
    private interface GuiderCommand {
        Status run() throws DAQException;
    }

    /**
     * Runs a state-changing guider command, retrying (up to {@code maxRetries}
     * times) only if it fails with a {@link #COMMAND_TIMEOUT}. Any other failure
     * is propagated immediately.
     */
    private Status withRetry(String name, GuiderCommand command) throws DAQException {
        int retries = 0;
        while (true) {
            try {
                Status status = command.run();
                if (retries > 0) {
                    LOG.log(Level.INFO, "Guider command {0} succeeded after {1} retr{2}",
                            new Object[]{name, retries, retries == 1 ? "y" : "ies"});
                }
                return status;
            } catch (DAQException x) {
                if (x.rc() == COMMAND_TIMEOUT && retries < maxRetries) {
                    retries++;
                    LOG.log(Level.WARNING, "Guider command {0} timed out, retry {1}/{2}",
                            new Object[]{name, retries, maxRetries});
                } else {
                    throw x;
                }
            }
        }
    }

    public Status start(ROICommon common, List<ROILocation> locations) throws DAQException {
        return this.start(common, locations, null);
    }


    public Status start(ROICommon common, List<ROILocation> locations, String id) throws DAQException {
        return start(new ROICommonExtended(common), locations, id);
    }
    /**
     * Start the guider, using the specified ROI locations
     *
     * @param common The common settings for this start command
     * @param locations The set of ROIs to start
     * @param id The id of this sequence of guider images.
     * @return The status after the command completes
     * @throws DAQException
     */
    public Status start(ROICommonExtended common, List<ROILocation> locations, String id) throws DAQException {
        final int nLocs = locations.size();
        int[] roiData = new int[nLocs * 5];
        for (int i = 0; i < nLocs; i++) {
            int j = i * 5;
            ROILocation location = locations.get(i);
            SensorLocation sensorLocation = location.getLocation();
            roiData[j] = sensorLocation.getRebLocation().index();
            roiData[j + 1] = sensorLocation.getSensor();
            roiData[j + 2] = location.getSegment();
            roiData[j + 3] = location.getStartRow();
            roiData[j + 4] = location.getStartCol();
        }
        return withRetry("start", () -> store.startGuider(guider, common.getRows(), common.getCols(), common.getIntegrationTimeMillis(),  common.getOverRows(), common.getUnderCols(), common.getOverCols(), common.getFlushCount(), id, roiData));
    }

    public void validate(ROICommon common, List<ROILocation> locations) throws DAQException {
        final int nLocs = locations.size();
        int[] roiData = new int[nLocs * 5];
        for (int i = 0; i < nLocs; i++) {
            int j = i * 5;
            ROILocation location = locations.get(i);
            SensorLocation sensorLocation = location.getLocation();
            roiData[j] = sensorLocation.getRebLocation().index();
            roiData[j + 1] = sensorLocation.getSensor();
            roiData[j + 2] = location.getSegment();
            roiData[j + 3] = location.getStartRow();
            roiData[j + 4] = location.getStartCol();
        }
        store.validateGuider(guider, common.getRows(), common.getCols(), common.getIntegrationTimeMillis(), roiData);
    }

    public Subscriber subscribe(Set<SensorLocation> locations, ByteOrder byteOrder, GuiderListener listener) throws DAQException {
        return new Subscriber(store, locations, byteOrder, listener);
    }

    public Status stop() throws DAQException {
        return stop(null);
    }

    public Status stop(String comment) throws DAQException {
        return withRetry("stop", () -> store.stopGuider(guider, comment));
    }

    public Status pause() throws DAQException {
        return pause(null);
    }

    public Status pause(String comment) throws DAQException {
        return withRetry("pause", () -> store.pauseGuider(guider, comment));
    }
    
    public Status play(String... files) throws DAQException {
        return store.playGuider(guider, files);
    }

    public Status resume() throws DAQException {
        return resume(null);
    }

    public Status resume(String comment) throws DAQException {
        return withRetry("resume", () -> store.resumeGuider(guider, comment));
    }

    public Status sleep() throws DAQException {
        return withRetry("sleep", () -> store.sleepGuider(guider));
    }

    public Status wake() throws DAQException {
        return wake(ClearParameters.defaultValue());
    }

    public Status wake(ClearParameters clearParameters) throws DAQException {
        return withRetry("wake", () -> store.wakeGuider(guider, clearParameters));
    }

    public Status clear(ClearParameters clearParameters) throws DAQException {
        return withRetry("clear", () -> store.clearGuider(guider, clearParameters));
    }

    public Config config() throws DAQException {
        return store.guiderConfig(guider);
    }

    public SeriesStatus series() throws DAQException {
        return store.guiderSeries(guider);
    }

    void detach() throws DAQException {
        store.detachGuider(this.guider);
    }

    public String getPartition() {
        return store.getPartition();
    }

    public LocationSet getConfiguredLocations() throws DAQException {
        return store.getConfiguredLocations();
    }

    public String getClientPlatform() throws DAQException {
        return store.getClientPlatform();
    }

    public static class Subscriber implements AutoCloseable {

        private static final Logger LOG = Logger.getLogger(Subscriber.class.getName());

        private volatile long subscriber;
        private volatile Thread waitThread;
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
            long sub = this.subscriber;
            if (sub != 0) {
                waitThread = Thread.currentThread();
                store.waitForGuider(sub, this);
            } else {
                throw new DAQException("Subscriber already closed");
            }
        }

        private void startCallback(StateMetaData state, SeriesMetaData series) {
            long start = System.nanoTime();
            try {
                listener.start(state, series);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "start callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "start took {0}ns {1} {2}", new Object[]{stop - start, state, series});
            }
        }

        void stopCallback(StateMetaData state) {
            long start = System.nanoTime();
            try {
                listener.stop(state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "stop callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "stop took {0}ns {1}", new Object[]{stop - start, state});
            }
        }

        void pauseCallback(StateMetaData state) {
            long start = System.nanoTime();
            try {
                listener.pause(state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "pause callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "pause took {0}ns {1}", new Object[]{stop - start, state});
            }
        }

        void resumeCallback(StateMetaData state) {
            long start = System.nanoTime();
            try {
                listener.resume(state);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "resume callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "resume took {0}ns {1}", new Object[]{stop - start, state});
            }
        }

        void rawStampCallback(StateMetaData state, ByteBuffer rawStamp) {
            long start = System.nanoTime();
            try {
                listener.rawStamp(state, rawStamp);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "rawStamp callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "rawStamp took {0}ns {1} {2}", new Object[]{stop - start, state, rawStamp.remaining()});
            }
        }

        void stampCallback(StateMetaData state, ByteBuffer stamp) {
            long start = System.nanoTime();
            try {
                listener.stamp(state, stamp);
            } catch (Exception x) {
                LOG.log(Level.WARNING, "stamp callback failed", x);
            } finally {
                long stop = System.nanoTime();
                LOG.log(Level.INFO, "stamp took {0}ns {1} {2}", new Object[]{stop - start, state, stamp.remaining()});
            }
        }

        public String getPartition() {
            return store.getPartition();
        }

        @Override
        public void close() throws DAQException {
            long sub = this.subscriber;
            if (sub != 0) {
                this.subscriber = 0;
                store.abortWaitForGuider(sub);
                try {
                    waitThread.join();
                } catch (InterruptedException ex) {
                    throw new DAQException("Unexpected interrupt");
                }
                store.detachGuiderSubscriber(sub);
            }
        }

    }

}
