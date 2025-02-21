package org.lsst.ccs.daq.guider;

import java.util.List;

/**
 *
 * @author tonyj
 */
public class Config {

    private final Status status;
    private final ROICommon common;
    private final List<ROILocation> locations;
    private final ClearParameters clearParameters;

    private Config(Status status, ClearParameters clearParameters, ROICommon common, List<ROILocation> locations) {
        this.status = status;
        this.clearParameters = clearParameters;
        this.common = common;
        this.locations = locations;
    }

    public Status getStatus() {
        return status;
    }

    public ROICommon getCommon() {
        return common;
    }

    public List<ROILocation> getLocations() {
        return locations;
    }

    public ClearParameters getClearParameters() {
        return clearParameters;
    }

    @Override
    public String toString() {
        return "Config [status=" + status + ", common=" + common + ", locations=" + locations + ", clearParameters="
                + clearParameters + "]";
    }
}
