package org.lsst.ccs.daq.guider;

import org.lsst.ccs.utilities.location.SensorLocation;

/**
 * Details of an ROI for a single sensor
 */
public class ROILocation {

    final SensorLocation location;
    final int segment;
    final int startRow;
    final int startCol;

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
