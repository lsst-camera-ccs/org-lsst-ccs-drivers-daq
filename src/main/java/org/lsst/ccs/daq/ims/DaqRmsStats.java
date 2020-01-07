package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;

public class DaqRmsStats {

    private final Location location;

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

    DaqRmsStats(byte bay, byte board, long probes, long unopened, long in_reset, long link_down, long lists_posted, long ops_posted, long waits, long responses, long inv_tids, long orphans, long rundowns, long tid_hwm) {

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
    }

    /* Accessors to be added */
}