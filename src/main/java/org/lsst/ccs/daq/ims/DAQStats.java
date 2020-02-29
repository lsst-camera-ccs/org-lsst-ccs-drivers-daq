package org.lsst.ccs.daq.ims;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import org.lsst.ccs.bus.annotations.SkipEncoding;
import org.lsst.ccs.utilities.location.Location;

/**
 * A base class for all DAQ statistics blocks which makes the statistics 
 * available by name, or as a single map of name->value.
 *
 * @author  Tony Johnson
 */
public class DAQStats {
    
    // Map associates each statistic with its name, and allows loop over them
    @SkipEncoding
    final Map<String, Long> statMap;
    @SkipEncoding
    private final Location location;

    DAQStats(int statMapInitialSize, byte bay, byte board) {
        this.statMap = new LinkedHashMap<>(statMapInitialSize);
        this.location = new Location(bay, board);
    }

    /**
     * Get the location corresponding to these statistics
     * @return The location
     */
     public Location getLocation() {
        return location;
    }

    /**
     * Get the specified statistic by name
     * @param statName The name of the requested statistic
     * @return the requested value
     * @throws IllegalArgumentException if the specified statistic name is invalid
     */
    public Long getStatistic(String statName) {
        Long result = statMap.get(statName);
        if (result == null) throw new IllegalArgumentException("Requested statistic name is not available: "+statName);
        return result;
    }

    /**
     * Get the map of all available statistics
     * @return An unmodifiable map of the available statistics.
     */
    public Map<String, Long> getMap() {
        return Collections.unmodifiableMap(statMap);
    }
    
}
