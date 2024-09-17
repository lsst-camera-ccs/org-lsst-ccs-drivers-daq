package org.lsst.ccs.daq.guider;

import org.lsst.ccs.daq.ims.Version;

/**
 * Meta-data common to a series of readouts from a single ROI
 */
public class SeriesMetaData {

    private final ROICommon common;
    private final ROILocation location;
    private final Version version;
    private final int firmware;
    private final long serialNumber;
    private final String id;
    private final String platform;
    private final boolean splitroi;
    private final int undercols;
    private final int ccdType;
    
    
    private SeriesMetaData(int firmware, long serialNumber, String id, String platform, ROICommon common, ROILocation location, Version version, boolean splitroi, int undercols, int ccdType) {
        this.firmware = firmware;
        this.serialNumber = serialNumber;
        this.common = common;
        this.location = location;
        this.version = version;
        this.id = id;
        this.platform = platform;
        this.splitroi = splitroi;
        this.undercols = undercols;
        this.ccdType = ccdType;
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

    public boolean isSplitROI() {
        return splitroi;
    }

    public int getUnderCols() {
        return undercols;
    }

    public int getCcdType() {
        return ccdType;
    }

    @Override
    public String toString() {
        return "SeriesMetaData{" + "common=" + common + ", location=" + location + ", version=" + version + ", firmware=" + firmware + ", serialNumber=" + serialNumber + ", id=" + id + ", platform=" + platform + ", splitroi=" + splitroi + ", undercols=" + undercols + ", ccdType=" + ccdType + '}';
    }
    
}
