package org.lsst.ccs.daq.ims;

import java.util.LinkedHashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

public class DAQFirmwareStats {

    private final Location location;

    /* The statistics quantities */
    private final long link_status;        // Link Status
    private final long rlink_data;         // Remote Link Data
    private final long cell_errors;        // Cell Error Count
    private final long link_down;          // Link Down Count
    private final long link_errors;        // Link Error Count
    private final long overflow_remote0;   // Remote Overflow VC Count 
    private final long overflow_remote1;   
    private final long overflow_remote2;   
    private final long overflow_remote3;   
    private final long rx_errors;          // Receive Frame Error Count
    private final long rx_frames;          // Receive Frame Count
    private final long overflow_local0;    // Local Overflow VC Count
    private final long overflow_local1;    
    private final long overflow_local2;    
    private final long overflow_local3;    
    private final long tx_errors;          // Transmit Frame Error Count
    private final long tx_frames;          // Transmit Frame Count
    private final long rx_clock;           // Receive Clock Frequency
    private final long tx_clock;           // Transmit Clock Frequency
    private final long last_tx;            // Last OpCode Transmitted
    private final long last_rx;            // Last OpCode Received
    private final long tx_op;              // OpCode Transmit count
    private final long rx_op;              // OpCode Received count

    // Map associates each statistic with its name, and allows loop over them
    private Map<String,Long> statMap;

    DAQFirmwareStats(byte bay, byte board, long link_status, long rlink_data, long cell_errors, long link_down, long link_errors, long overflow_remote0, long overflow_remote1, long overflow_remote2, long overflow_remote3, long rx_errors, long rx_frames, long overflow_local0, long overflow_local1, long overflow_local2, long overflow_local3, long tx_errors, long tx_frames, long rx_clock, long tx_clock, long last_tx, long last_rx, long tx_op, long rx_op) {

        this.location     =  new Location(bay, board);

        this.link_status      = link_status;     
        this.rlink_data       = rlink_data;      
        this.cell_errors      = cell_errors;     
        this.link_down        = link_down;       
        this.link_errors      = link_errors;      
        this.overflow_remote0 = overflow_remote0;  
        this.overflow_remote1 = overflow_remote1;     
        this.overflow_remote2 = overflow_remote2;  
        this.overflow_remote3 = overflow_remote3;  
        this.rx_errors        = rx_errors;            	    
        this.rx_frames        = rx_frames;         
        this.overflow_local0  = overflow_local0;  
        this.overflow_local1  = overflow_local1;  
        this.overflow_local2  = overflow_local2;  
        this.overflow_local3  = overflow_local3;  
        this.tx_errors        = tx_errors;        
        this.tx_frames        = tx_frames;        
        this.rx_clock         = rx_clock;         
        this.tx_clock         = tx_clock;         
        this.last_tx          = last_tx;          
        this.last_rx          = last_rx;          
        this.tx_op            = tx_op;            
        this.rx_op            = rx_op;            

        statMap = new LinkedHashMap<>(40);
        statMap.put("link_status"     , link_status);     
        statMap.put("rlink_data"      , rlink_data);      
        statMap.put("cell_errors"     , cell_errors);     
        statMap.put("link_down"       , link_down);       
        statMap.put("link_errors"     , link_errors);     
        statMap.put("overflow_remote0", overflow_remote0); 
        statMap.put("overflow_remote1", overflow_remote1);
        statMap.put("overflow_remote2", overflow_remote2);
        statMap.put("overflow_remote3", overflow_remote3);
        statMap.put("rx_errors"       , rx_errors);       
        statMap.put("rx_frames"       , rx_frames);       
        statMap.put("overflow_local0" , overflow_local0); 
        statMap.put("overflow_local1" , overflow_local1); 
        statMap.put("overflow_local2" , overflow_local2); 
        statMap.put("overflow_local3" , overflow_local3); 
        statMap.put("tx_errors      " , tx_errors);             
        statMap.put("tx_frames"       , tx_frames);       
        statMap.put("rx_clock"        , rx_clock);         
        statMap.put("tx_clock"        , tx_clock);        
        statMap.put("last_tx"         , last_tx);         
        statMap.put("last_rx"         , last_rx);         
        statMap.put("tx_op"           , tx_op);           
        statMap.put("rx_op"           , rx_op);           
    }

    /* Accessor is via String name, using Map */

    public Long getFirmwareStat(String statName) {
        return statMap.get(statName);
    }

    /* Accessor to entire Map  */

    public Map<String,Long> getFirmwareStatMap() {
        return statMap;
    }

}
