package org.lsst.ccs.daq.guider;

/**
 * This is an extension of ROICommon which also includes extra parameters which are not
 * part of the common element of ROISpec.
 */
public class ROICommonExtended extends ROICommon {

    private final int underCols;
    private final int overCols;
    private final int overRows;
    private final int flushCount;

    public ROICommonExtended(int rows, int cols, int integrationTimeMillis, int underCols, int overCols, int overRows, int flushCount) {
        super(rows, cols, integrationTimeMillis);
        this.underCols = underCols;
        this.overCols = overCols;
        this.overRows = overRows;
        this.flushCount = flushCount;
    }

    public int getUnderCols() {
        return underCols;
    }

    public int getOverCols() {
        return overCols;
    }

    public int getOverRows() {
        return overRows;
    }

    public int getFlushCount() {
        return flushCount;
    }

    @Override
    public String toString() {
        return "ROICommonExtended [rows=" + getRows() + ", cols=" + getCols() + ", integrationTimeMillis=" + getIntegrationTimeMillis()
                + ", underCols=" + underCols + ", overCols=" + overCols + ", overRows=" + overRows + ", flushCount="
                + flushCount + "]";
    }
}
