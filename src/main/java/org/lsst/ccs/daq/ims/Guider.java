package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.EnumSet;
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
import org.lsst.ccs.daq.guider.RetryStatus;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.daq.guider.Status.State;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * An interface to the DAQ guider.
 *
 * @see Store#getGuider()
 * @author tonyj
 */
public class Guider {

    private static final Logger LOG = Logger.getLogger(Guider.class.getName());

    // GDS error codes. Kept in sync with the DAQ enum
    // gds/Exception.hh: enum Id { COMMAND_TIMEOUT=-1, ..., INVALID_TRANSITION=2, ... }
    private static final int COMMAND_TIMEOUT = -1;
    private static final int INVALID_TRANSITION = 2;

    // The guider states that indicate each state-changing command has already
    // taken effect, used to reconcile a lost response after a command timeout.
    // Some commands have more than one settled state because the state reached
    // depends on the state the command started from (e.g. pause lands in PAUSED
    // from RUNNING but in IDLEPAUSE from IDLECLEARING).
    private static final Set<State> START_SUCCESS = EnumSet.of(State.PAUSED);
    private static final Set<State> STOP_SUCCESS = EnumSet.of(State.IDLECLEARING);
    private static final Set<State> PAUSE_SUCCESS = EnumSet.of(State.PAUSED, State.IDLEPAUSE);
    private static final Set<State> RESUME_SUCCESS = EnumSet.of(State.RUNNING, State.IDLECLEARING);
    private static final Set<State> SLEEP_SUCCESS = EnumSet.of(State.SLEEPING);
    private static final Set<State> WAKE_SUCCESS = EnumSet.of(State.IDLECLEARING);
    private static final Set<State> CLEAR_SUCCESS = EnumSet.of(State.CLEARING);

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
     * <p>
     * A command timeout is ambiguous: either the command never reached the DAQ
     * (in which case a retry is correct) or the command was acted on but its
     * response was lost (in which case the guider has already changed state and a
     * blind retry would fail with {@link #INVALID_TRANSITION}). When a set of
     * {@code successStates} is supplied, on a timeout we query the current guider
     * state via {@link #config()} and reconcile:
     * <ul>
     * <li>if the guider is in one of {@code successStates}, the command took
     * effect and we return the queried status without re-issuing it;</li>
     * <li>if the guider is in an inconclusive state ({@link State#ERROR} or
     * {@link State#UNDEFINED}), we give up and throw a descriptive exception
     * rather than fabricate success or blindly retry;</li>
     * <li>otherwise we retry the command (bounded by {@code maxRetries}).</li>
     * </ul>
     * A set (rather than a single state) is required because the settled state a
     * command reaches depends on the state it started from: e.g. {@code pause}
     * reaches {@link State#PAUSED} from {@link State#RUNNING} but
     * {@link State#IDLEPAUSE} from {@link State#IDLECLEARING}. When
     * {@code successStates} is {@code null} or empty (for a command with no
     * settled success state) a timeout is handled by a plain retry.
     *
     * @param name the command name, for logging
     * @param successStates the guider states that mean the command already
     * succeeded, or {@code null}/empty to disable state reconciliation
     * @param command the command to run
     */
    private RetryStatus withRetry(String name, Set<State> successStates, GuiderCommand command) throws DAQException {
        int timeouts = 0;
        int retries = 0;
        while (true) {
            try {
                Status status = command.run();
                if (retries > 0) {
                    LOG.log(Level.INFO, "Guider command {0} succeeded after {1} retr{2}",
                            new Object[]{name, retries, retries == 1 ? "y" : "ies"});
                }
                return new RetryStatus(status, timeouts, retries);
            } catch (DAQException x) {
                if (x.rc() != COMMAND_TIMEOUT || maxRetries == 0) {
                    // Not a timeout, or retries disabled: behave as the plain command.
                    throw x;
                }
                timeouts++;
                // Timeout: try to reconcile against the guider's actual state before retrying.
                if (successStates != null && !successStates.isEmpty()) {
                    Status current = config().getStatus();
                    State actual = current.getOut();
                    if (successStates.contains(actual)) {
                        LOG.log(Level.INFO, "Guider command {0} timed out but the guider is already in state {1}; "
                                + "the command took effect but its response was lost", new Object[]{name, actual});
                        return new RetryStatus(current, timeouts, retries);
                    }
                    if (actual == State.ERROR || actual == State.UNDEFINED) {
                        throw new DAQException(String.format(
                                "Guider command %s timed out and the guider is in state %s; "
                                + "the outcome of the command is indeterminate", name, actual), COMMAND_TIMEOUT);
                    }
                }
                if (retries < maxRetries) {
                    retries++;
                    LOG.log(Level.WARNING, "Guider command {0} timed out, retry {1}/{2}",
                            new Object[]{name, retries, maxRetries});
                } else {
                    throw x;
                }
            }
        }
    }

    public RetryStatus start(ROICommon common, List<ROILocation> locations) throws DAQException {
        return this.start(common, locations, null);
    }


    public RetryStatus start(ROICommon common, List<ROILocation> locations, String id) throws DAQException {
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
    public RetryStatus start(ROICommonExtended common, List<ROILocation> locations, String id) throws DAQException {
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
        return withRetry("start", START_SUCCESS, () -> store.startGuider(guider, common.getRows(), common.getCols(), common.getIntegrationTimeMillis(),  common.getOverRows(), common.getUnderCols(), common.getOverCols(), common.getFlushCount(), id, roiData));
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

    public RetryStatus stop() throws DAQException {
        return stop(null);
    }

    public RetryStatus stop(String comment) throws DAQException {
        return withRetry("stop", STOP_SUCCESS, () -> store.stopGuider(guider, comment));
    }

    public RetryStatus pause() throws DAQException {
        return pause(null);
    }

    public RetryStatus pause(String comment) throws DAQException {
        return withRetry("pause", PAUSE_SUCCESS, () -> store.pauseGuider(guider, comment));
    }
    
    public Status play(String... files) throws DAQException {
        return store.playGuider(guider, files);
    }

    public RetryStatus resume() throws DAQException {
        return resume(null);
    }

    public RetryStatus resume(String comment) throws DAQException {
        return withRetry("resume", RESUME_SUCCESS, () -> store.resumeGuider(guider, comment));
    }

    public RetryStatus sleep() throws DAQException {
        return withRetry("sleep", SLEEP_SUCCESS, () -> store.sleepGuider(guider));
    }

    public RetryStatus wake() throws DAQException {
        return wake(ClearParameters.defaultValue());
    }

    public RetryStatus wake(ClearParameters clearParameters) throws DAQException {
        return withRetry("wake", WAKE_SUCCESS, () -> store.wakeGuider(guider, clearParameters));
    }

    public RetryStatus clear(ClearParameters clearParameters) throws DAQException {
        return withRetry("clear", CLEAR_SUCCESS, () -> store.clearGuider(guider, clearParameters));
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
