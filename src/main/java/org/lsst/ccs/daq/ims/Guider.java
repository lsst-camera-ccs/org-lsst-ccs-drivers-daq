package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;
import java.util.Arrays;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.GuiderListener;
import org.lsst.ccs.daq.guider.ROICommon;
import org.lsst.ccs.daq.guider.ROILocation;
import org.lsst.ccs.daq.guider.SensorLocation;
import org.lsst.ccs.daq.guider.Series;
import org.lsst.ccs.daq.guider.SeriesMetaData;
import org.lsst.ccs.daq.guider.StateMetaData;
import org.lsst.ccs.daq.guider.Status;

/**
 * An interface to the DAQ guider.
 *
 * @see Store#getGuider()
 * @author tonyj
 */
public class Guider {

    private final Store store;
    private final long guider;

    public Guider(Store store, long guider) {
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
            SensorLocation sensorLocation = location.getLocation();
            roiData[j] = sensorLocation.getRebLocation().index();
            roiData[j + 1] = sensorLocation.getSensor();
            roiData[j + 2] = location.getSegment();
            roiData[j + 3] = location.getStartRow();
            roiData[j + 4] = location.getStartCol();
        }
        System.out.println(Arrays.toString(roiData));
        return store.startGuider(guider, common.getnRows(), common.getnCols(), common.getIntegrationTimeMillis(), id, roiData);
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

    public String getPartition() {
        return store.getPartition();
    }

    public static class Subscriber implements AutoCloseable {

        private static final Logger LOG = Logger.getLogger(Subscriber.class.getName());

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
            LOG.log(Level.INFO, "resume {0}", state);
            try {
                listener.resume(state);
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
        
        public String getPartition() {
            return store.getPartition();
        }

        @Override
        public void close() throws DAQException {
            store.detachGuiderSubscriber(subscriber);
        }

    }

}
