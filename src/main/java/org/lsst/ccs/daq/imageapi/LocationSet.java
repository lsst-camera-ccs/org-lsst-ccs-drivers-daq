package org.lsst.ccs.daq.imageapi;

import java.util.BitSet;
import java.util.Set;
import java.util.TreeSet;

/**
 *
 * @author tonyj
 */
public class LocationSet {

    private final BitSet locations;

    public LocationSet() {
        locations = new BitSet();
    }

    LocationSet(BitSet bitset) {
        locations = bitset;
    }

    BitSet getBitSet() {
        return locations;
    }

    public void addLocation(Location location) {
        locations.set(location.index());
    }

    public void removeLocation(Location location) {
        locations.clear(location.index());
    }

    public boolean hasLocation(Location location) {
        return locations.get(location.index());
    }

    boolean isSet(int i) {
        return locations.get(i);
    }

    public static LocationSet singleton(Location location) {
        LocationSet result = new LocationSet();
        result.addLocation(location);
        return result;
    }

    public int cardinality() {
        return locations.cardinality();
    }

    Set<Location> getLocations() {
        Set<Location> result = new TreeSet<>();
        for (int index = -1;;) {
            index = locations.nextSetBit(index + 1);
            if (index < 0) {
                break;
            }
            result.add(new Location(index));
        }
        return result;
    }

    @Override
    public String toString() {
        return "LocationSet{" + "locations=" + getLocations() + '}';
    }

}
