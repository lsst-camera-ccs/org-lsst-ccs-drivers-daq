package org.lsst.ccs.daq.guider;

import java.util.LinkedHashSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.Location.LocationType;

/**
 *
 * @author tonyj
 */
public class SensorLocationSet extends LinkedHashSet<SensorLocation> {
    
    private final static Pattern SENSOR_PATTERN = Pattern.compile("(.*)/(SG[01])");

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
            Matcher matcher = SENSOR_PATTERN.matcher(lll);
            if (matcher.matches()) {
                String reb = matcher.group(1);
                Location rebLocation = Location.of(reb);
                if (rebLocation.type() != LocationType.GUIDER) throw new IllegalArgumentException("Error parsing sensorLocations: "+sensorLocations+" reb "+reb+" is not a guider");
                String sensor = matcher.group(2);
                int sensorIndex = Integer.parseInt(sensor.substring(2));
                SensorLocation sensorLocation = new SensorLocation(rebLocation, sensorIndex);
                this.add(sensorLocation);
            } else { 
                throw new IllegalArgumentException("Error parsing sensorLocations: "+sensorLocations);
            }
        }
    }
}
