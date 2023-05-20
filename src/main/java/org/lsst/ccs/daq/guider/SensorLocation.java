package org.lsst.ccs.daq.guider;

import java.io.Serializable;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.lsst.ccs.utilities.location.Location;

/**
 *
 * @author tonyj
 */
public class SensorLocation implements Serializable, Comparable<SensorLocation> {

    private final static Pattern SENSOR_PATTERN = Pattern.compile("R(\\d\\d)S(.)([012])");
    private static final long serialVersionUID = 1;

    final Location rebLocation;
    final int sensor;

    public SensorLocation(byte bay, byte board, int sensor) {
        this(new Location(bay, board), sensor);
    }

    public SensorLocation(Location rebLocation, int sensor) {
        int nCCDs = rebLocation.type() == Location.LocationType.SCIENCE ? 3 : 2;
        if (sensor >= nCCDs) {
            throw new IllegalArgumentException("Invalid sensor # "+sensor+" for "+rebLocation);
        }
        this.rebLocation = rebLocation;
        this.sensor = sensor;
    }

    public Location getRebLocation() {
        return rebLocation;
    }

    public int getSensor() {
        return sensor;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 83 * hash + Objects.hashCode(this.rebLocation);
        hash = 83 * hash + this.sensor;
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final SensorLocation other = (SensorLocation) obj;
        if (this.sensor != other.sensor) {
            return false;
        }
        return Objects.equals(this.rebLocation, other.rebLocation);
    }

    public static SensorLocation of(String string) {
        Matcher matcher = SENSOR_PATTERN.matcher(string);
        if (!matcher.matches()) {
            throw new IllegalArgumentException("Invalid sensor location: " + string);
        }
        String reb = "R" + matcher.group(1) + "/Reb" + matcher.group(2);
        Location rebLocation = Location.of(reb);
        int sensor = Integer.parseInt(matcher.group(3));
        return new SensorLocation(rebLocation, sensor);
    }

    @Override
    public String toString() {
        return rebLocation.getRaftName() + rebLocation.getSensorName(sensor);
    }

    public String toString(String rebSensorSeparator) {
        return rebLocation.getRaftName() + rebSensorSeparator + rebLocation.getSensorName(sensor);
    }
    
    @Override
    public int compareTo(SensorLocation other) {
        int result = this.rebLocation.compareTo(other.rebLocation);
        if (result == 0) result = this.sensor - other.sensor;
        return result;
    }

}
