package org.lsst.ccs.daq.guider;

import java.util.List;

/**
 *
 * @author tonyj
 */
public class Config {
    
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
