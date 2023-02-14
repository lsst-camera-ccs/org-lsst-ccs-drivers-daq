package org.lsst.ccs.daq.guider;

import java.util.LinkedHashSet;

/**
 *
 * @author tonyj
 */
public class SensorLocationSet extends LinkedHashSet<SensorLocation> {
    
    public SensorLocationSet() {
    }

    public SensorLocationSet(String sensorLocations) {
        if (sensorLocations != null && !sensorLocations.isEmpty()) {
            locationsFromCommaDelimitedString(sensorLocations);
        }
    }

    private void locationsFromCommaDelimitedString(String sensorLocations) {
        String[] ll = sensorLocations.split(",");
        for (String lll : ll) {
            lll = lll.trim();
            SensorLocation sl = SensorLocation.of(lll);
            this.add(sl);
        }
    }
}
