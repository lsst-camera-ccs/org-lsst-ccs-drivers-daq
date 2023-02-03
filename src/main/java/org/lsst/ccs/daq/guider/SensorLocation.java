package org.lsst.ccs.daq.guider;

import java.util.Objects;
import org.lsst.ccs.utilities.location.Location;

/**
 *
 * @author tonyj
 */
public class SensorLocation {
    
    final Location rebLocation;
    final int sensor;

    public SensorLocation(byte bay, byte board, int sensor) {
        this(new Location(bay, board), sensor);
    }

    public SensorLocation(Location rebLocation, int sensor) {
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

    @Override
    public String toString() {
        return rebLocation.toString() + rebLocation.getSensorName(sensor);
    }
    
}
