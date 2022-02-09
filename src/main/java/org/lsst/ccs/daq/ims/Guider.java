package org.lsst.ccs.daq.ims;

import java.util.List;
import org.lsst.ccs.utilities.location.Location;

/**
 *
 * @author tonyj
 */
public class Guider {

    private final Store store;
    private final long guider;

    Guider(Store store, long guider) {
        this.store = store;
        this.guider = guider;
    }

    public void start(GuiderROIs rois) {
        final int nLocs = rois.locations.size();
        int[] roiData = new int[nLocs * 5];
        for (int i = 0; i < nLocs; i++) {
            int j = i * 5;
            ROILocation location = rois.locations.get(i);
            roiData[j] = location.location.index();
            roiData[j + 1] = location.sequencer;
            roiData[j + 2] = location.segment;
            roiData[j + 3] = location.startRow;
            roiData[j + 4] = location.startCol;
        }
        store.startGuider(guider, rois.nRows, rois.nCols, rois.integrationTimeMilliSeconds, rois.binning, roiData);
    }

    public void stop() {
        store.stopGuider(guider);
    }

    public void pause() {
        store.pauseGuider(guider);
    }

    public void resume() {
        store.resumeGuider(guider);
    }

    void detach() throws DAQException {
        store.detachGuider(this.guider);
    }

    public static class GuiderROIs {

        private final int nRows;
        private final int nCols;
        private final int integrationTimeMilliSeconds;
        private final int binning;
        private final List<ROILocation> locations;

        public GuiderROIs(int nRows, int nCols, int integrationTimeMilliSeconds, int binning, List<ROILocation> locations) {
            this.nRows = nRows;
            this.nCols = nCols;
            this.integrationTimeMilliSeconds = integrationTimeMilliSeconds;
            this.binning = binning;
            this.locations = locations;
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

        public List<ROILocation> getLocations() {
            return locations;
        }
    }

    public static class ROILocation {

        private final Location location;
        private final int sequencer;
        private final int segment;
        private final int startRow;
        private final int startCol;

        public ROILocation(Location location, int sequencer, int segment, int startRow, int startCol) {
            this.location = location;
            this.sequencer = sequencer;
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

        public int getSequencer() {
            return sequencer;
        }

        public int getSegment() {
            return segment;
        }
    }
}
