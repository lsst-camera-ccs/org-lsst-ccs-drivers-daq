package org.lsst.ccs.daq.ims;

public class DAQRdsStats extends DAQStats {


    /* The statistics quantities */
    private final long wakes;	      
    private final long discards;      
    private final long no_event;      
    private final long bdi_waits;     
    private final long bdi_nulls;     
    private final long bdi_seq_errors;
    private final long bdi_overflows; 
    private final long bdi_firsts;    
    private final long bdi_lasts;     
    private final long bdi_frees;     


    DAQRdsStats(byte bay, byte board, long wakes, long discards, long no_event, long bdi_waits, long bdi_nulls, long bdi_seq_errors, long bdi_overflows, long bdi_firsts, long bdi_lasts, long bdi_frees)  {

        super(20, bay, board);
        
        this.wakes	    =  wakes;	       	
        this.discards       =  discards;      
        this.no_event       =  no_event;      
        this.bdi_waits      =  bdi_waits;     
        this.bdi_nulls      =  bdi_nulls;     
        this.bdi_seq_errors =  bdi_seq_errors;
        this.bdi_overflows  =  bdi_overflows; 
        this.bdi_firsts     =  bdi_firsts;    
        this.bdi_lasts      =  bdi_lasts;     
        this.bdi_frees      =  bdi_frees;     

        statMap.put("wakes"	    , wakes	    );
        statMap.put("discards"      , discards      );
        statMap.put("no_event"      , no_event      );
        statMap.put("bdi_waits"     , bdi_waits     );
        statMap.put("bdi_nulls"     , bdi_nulls     );
        statMap.put("bdi_seq_errors", bdi_seq_errors);
        statMap.put("bdi_overflows" , bdi_overflows );
        statMap.put("bdi_firsts"    , bdi_firsts    );
        statMap.put("bdi_lasts"     , bdi_lasts     );
        statMap.put("bdi_frees"     , bdi_frees     );
    }

    public long getWakes() {
        return wakes;
    }

    public long getDiscards() {
        return discards;
    }

    public long getNo_event() {
        return no_event;
    }

    public long getBdi_waits() {
        return bdi_waits;
    }

    public long getBdi_nulls() {
        return bdi_nulls;
    }

    public long getBdi_seq_errors() {
        return bdi_seq_errors;
    }

    public long getBdi_overflows() {
        return bdi_overflows;
    }

    public long getBdi_firsts() {
        return bdi_firsts;
    }

    public long getBdi_lasts() {
        return bdi_lasts;
    }

    public long getBdi_frees() {
        return bdi_frees;
    }

}
