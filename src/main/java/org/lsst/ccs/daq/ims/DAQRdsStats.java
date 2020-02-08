package org.lsst.ccs.daq.ims;

import java.util.LinkedHashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

public class DAQRdsStats {

    private final Location location;

    /* The statistics quantities */
    private final long wakes;	      
    private final long discards;      
    private final long bdi_waits;     
    private final long bdi_nulls;     
    private final long bdi_seq_errors;
    private final long bdi_overflows; 
    // private final long no_event;      
    private final long bdi_firsts;    
    private final long bdi_lasts;     
    private final long bdi_frees;     

    // Map associates each statistic with its name, and allows loop over them
    private Map<String,Long> statMap;

    DAQRdsStats(byte bay, byte board, long wakes, long discards, long bdi_waits, long bdi_nulls, long bdi_seq_errors, long bdi_overflows, long bdi_firsts, long bdi_lasts, long bdi_frees)  {

        this.location     =  new Location(bay, board);
        
        this.wakes	    =  wakes;	       	
        this.discards       =  discards;      
        this.bdi_waits      =  bdi_waits;     
        this.bdi_nulls      =  bdi_nulls;     
        this.bdi_seq_errors =  bdi_seq_errors;
        this.bdi_overflows  =  bdi_overflows; 
        // this.no_event       =  no_event;      
        this.bdi_firsts     =  bdi_firsts;    
        this.bdi_lasts      =  bdi_lasts;     
        this.bdi_frees      =  bdi_frees;     

        statMap = new LinkedHashMap<>(20);
        statMap.put("wakes"	    , wakes	    );
        statMap.put("discards"      , discards      );
        statMap.put("bdi_waits"     , bdi_waits     );
        statMap.put("bdi_nulls"     , bdi_nulls     );
        statMap.put("bdi_seq_errors", bdi_seq_errors);
        statMap.put("bdi_overflows" , bdi_overflows );
        // statMap.put("no_event"      , no_event      );
        statMap.put("bdi_firsts"    , bdi_firsts    );
        statMap.put("bdi_lasts"     , bdi_lasts     );
        statMap.put("bdi_frees"     , bdi_frees     );
    }

    /* Accessor is via String name, using Map */

    public Long getRdsStat(String statName) {
        return statMap.get(statName);
    }

    /* Accessor to entire Map  */

    public Map<String,Long> getRdsStatMap() {
        return statMap;
    }


}
