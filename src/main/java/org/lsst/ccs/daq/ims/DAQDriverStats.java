package org.lsst.ccs.daq.ims;

import java.util.LinkedHashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

public class DAQDriverStats {

    private final Location location;

    /* The statistics quantities */
    private final long received;
    private final long errors;  
    private final long rxVC0;   
    private final long rxVC1;   
    private final long rxVC2;   
    private final long rxVC3;   

    // Map associates each statistic with its name, and allows loop over them
    private Map<String,Long> statMap;

    DAQDriverStats(byte bay, byte board, long received, long errors, long rxVC0, long rxVC1, long rxVC2, long rxVC3) {

        this.location     =  new Location(bay, board);

        this.received =  received;
        this.errors   =  errors;  
        this.rxVC0    =  rxVC0;   
        this.rxVC1    =  rxVC1;   
        this.rxVC2    =  rxVC2;   
        this.rxVC3    =  rxVC3;   

        statMap = new LinkedHashMap<>(20);
        statMap.put("received", received);
        statMap.put("errors"  , errors  );
        statMap.put("rxVC0"   , rxVC0   );
        statMap.put("rxVC1"   , rxVC1   );
        statMap.put("rxVC2"   , rxVC2   );
        statMap.put("rxVC3"   , rxVC3   );

    }

    /* Accessor is via String name, using Map */

    public Long getDriverStat(String statName) {
        return statMap.get(statName);
    }

    /* Accessor to entire Map  */

    public Map<String,Long> getDriverStatMap() {
        return statMap;
    }

}
