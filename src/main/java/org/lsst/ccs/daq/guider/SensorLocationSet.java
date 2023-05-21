package org.lsst.ccs.daq.guider;

import java.util.TreeSet;

/**
 *
 * @author tonyj
 */
public class SensorLocationSet extends TreeSet<SensorLocation> {

    public SensorLocationSet() {
    }

    private SensorLocationSet(String sensorLocations) {
        if (sensorLocations != null && !sensorLocations.isEmpty()) {
            locationsFromCommaDelimitedString(sensorLocations);
        }
    }

    public static SensorLocationSet of(String string) {
        return new SensorLocationSet(string);
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
