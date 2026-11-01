package org.lsst.ccs.daq.guider;

import java.util.List;

/**
 *
 * @author tonyj
 */
public class Config {

    private final Status status;
    private final ROICommonExtended common;
    private final List<ROILocation> locations;
    private final ClearParameters clearParameters;

    private Config(Status status, ClearParameters clearParameters, ROICommonExtended common, List<ROILocation> locations) {
        this.status = status;
        this.clearParameters = clearParameters;
        this.common = common;
        this.locations = locations;
    }

    /**
     * Creates a Config wrapping the given status, with no ROI/clear details.
     * Intended for tests, which cannot otherwise construct a Config.
     */
    static Config of(Status status) {
        return new Config(status, null, null, null);
    }

    public Status getStatus() {
        return status;
    }

    public ROICommonExtended getCommon() {
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
