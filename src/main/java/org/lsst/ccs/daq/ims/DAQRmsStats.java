package org.lsst.ccs.daq.ims;

import java.util.HashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

public class DAQRmsStats {

    private final Location location;

    /* The statistics quantities */
    private final long probes;	    
    private final long unopened;    
    private final long in_reset;    
    private final long link_down;   
    private final long lists_posted;
    private final long ops_posted;  
    private final long waits;	    
    private final long responses;   
    private final long inv_tids;    
    private final long orphans;	    
    private final long rundowns;    
    private final long tid_hwm;	    

    // Map associates each statistic with its name, and allows loop over them
    private Map<String,Long> statMap;

    DAQRmsStats(byte bay, byte board, long probes, long unopened, long in_reset, long link_down, long lists_posted, long ops_posted, long waits, long responses, long inv_tids, long orphans, long rundowns, long tid_hwm) {

        this.location     =  new Location(bay, board);

        this.probes       =  probes;	    
        this.unopened     =  unopened;     
        this.in_reset     =  in_reset;     
        this.link_down    =  link_down;    
        this.lists_posted =  lists_posted;
        this.ops_posted   =  ops_posted;   
        this.waits	  =  waits;	    
        this.responses    =  responses;    
        this.inv_tids     =  inv_tids;     
        this.orphans      =  orphans;	    	    
        this.rundowns     =  rundowns;     
        this.tid_hwm      =  tid_hwm;     

        statMap = new HashMap<>();
        statMap.put("probes"      , probes      );
        statMap.put("unopened"    , unopened    );
        statMap.put("in_reset"    , in_reset    );
        statMap.put("link_down"   , link_down   );
        statMap.put("lists_posted", lists_posted);
        statMap.put("ops_posted"  , ops_posted  );
        statMap.put("waits"	  , waits	); 
        statMap.put("responses"   , responses   );
        statMap.put("inv_tids"    , inv_tids    );
        statMap.put("orphans"     , orphans     );
        statMap.put("rundowns"    , rundowns    );
        statMap.put("tid_hwm"     , tid_hwm     );
    }

    /* Accessor is via String name, using Map */

    Long getRmsStat(String statName) {
        return statMap.get(statName);
    }

    /* Accessor to entire Map  */

    public Map<String,Long> getRmsStatNap() {
        return statMap;
    }

}
