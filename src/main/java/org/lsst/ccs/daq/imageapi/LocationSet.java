package org.lsst.ccs.daq.imageapi;

import java.util.BitSet;

/**
 *
 * @author tonyj
 */
public class LocationSet {

    private final BitSet locations;

    public LocationSet() {
        locations = new BitSet();
    }
    
    LocationSet(long[] elements) {
        locations = BitSet.valueOf(elements);
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

    public int cardinality() {
        return locations.cardinality();
    }
}
