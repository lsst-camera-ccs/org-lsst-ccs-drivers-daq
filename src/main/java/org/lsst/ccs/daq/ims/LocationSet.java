package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import java.util.AbstractSet;
import java.util.BitSet;
import java.util.Iterator;
import java.util.Set;

/**
 * Default implementation of Set&lt;Location&gt;. This implementation uses a
 * bitmap to represent the selected locations.
 *
 * @author tonyj
 */
public class LocationSet extends AbstractSet<Location> implements Serializable {

    private static final long serialVersionUID = -4332892191028786738L;
    private final BitSet locations;

    /**
     * Creates an empty location set
     */
    public LocationSet() {
        locations = new BitSet();
    }

    /**
     * Creates a location set which is a copy of the provided set
     *
     * @param input The location set to copy
     */
    public LocationSet(Set<Location> input) {
        this();
        if (input instanceof LocationSet) {
            this.locations.or(((LocationSet) input).locations);
        } else if (input != null) {
            input.forEach((l) -> {
                locations.set(l.index());
            });
        }
    }

    /**
     * Creates a LocationSet from a list of strings
     *
     * @see Location#of(java.lang.String)
     * @param location The location(s)
     * @return The LocationSet
     */
    public static LocationSet of(String... location) {
        BitSet locations = new BitSet();
        for (String l : location) {
            locations.set(Location.of(l).index());
        }
        return new LocationSet(locations);
    }

    LocationSet(BitSet bitset) {
        locations = bitset;
    }

    BitSet getBitSet() {
        return locations;
    }

    @Override
    public boolean add(Location location) {
        final int index = location.index();
        boolean wasSet = locations.get(index);
        locations.set(index);
        return wasSet;
    }

    @Override
    public boolean remove(Object location) {
        if (location instanceof Location) {
            final int index = ((Location) location).index();
            boolean wasSet = locations.get(index);
            locations.clear(index);
            return wasSet;
        } else {
            return false;
        }
    }

    boolean isSet(int i) {
        return locations.get(i);
    }

    @Override
    public Iterator<Location> iterator() {
        return new Iterator<Location>() {
            private int current = -1;

            @Override
            public boolean hasNext() {
                return locations.nextSetBit(current + 1) >= 0;
            }

            @Override
            public Location next() {
                current = locations.nextSetBit(current + 1);
                return new Location(current);
            }
        };
    }

    @Override
    public int size() {
        return locations.cardinality();
    }

}
