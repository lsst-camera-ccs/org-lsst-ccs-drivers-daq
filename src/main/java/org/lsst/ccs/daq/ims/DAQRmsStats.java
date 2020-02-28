package org.lsst.ccs.daq.ims;

import java.io.Serializable;

public class DAQRmsStats extends DAQStats implements Serializable {

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

    DAQRmsStats(byte bay, byte board, long probes, long unopened, long in_reset, long link_down, long lists_posted, long ops_posted, long waits, long responses, long inv_tids, long orphans, long rundowns, long tid_hwm) {

        super(20, bay, board);
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

    public long getProbes() {
        return probes;
    }

    public long getUnopened() {
        return unopened;
    }

    public long getIn_reset() {
        return in_reset;
    }

    public long getLink_down() {
        return link_down;
    }

    public long getLists_posted() {
        return lists_posted;
    }

    public long getOps_posted() {
        return ops_posted;
    }

    public long getWaits() {
        return waits;
    }

    public long getResponses() {
        return responses;
    }

    public long getInv_tids() {
        return inv_tids;
    }

    public long getOrphans() {
        return orphans;
    }

    public long getRundowns() {
        return rundowns;
    }

    public long getTid_hwm() {
        return tid_hwm;
    }

}
