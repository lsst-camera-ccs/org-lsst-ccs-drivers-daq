package org.lsst.ccs.daq.ims;

public class DAQDriverStats extends DAQStats {

    /* The statistics quantities */
    private final long received;
    private final long errors;  
    private final long rxVC0;   
    private final long rxVC1;   
    private final long rxVC2;   
    private final long rxVC3;   


    DAQDriverStats(byte bay, byte board, long received, long errors, long rxVC0, long rxVC1, long rxVC2, long rxVC3) {

        super(6, bay, board);

        this.received =  received;
        this.errors   =  errors;  
        this.rxVC0    =  rxVC0;   
        this.rxVC1    =  rxVC1;   
        this.rxVC2    =  rxVC2;   
        this.rxVC3    =  rxVC3;   

        statMap.put("received", received);
        statMap.put("errors"  , errors  );
        statMap.put("rxVC0"   , rxVC0   );
        statMap.put("rxVC1"   , rxVC1   );
        statMap.put("rxVC2"   , rxVC2   );
        statMap.put("rxVC3"   , rxVC3   );
    }

    public long getReceived() {
        return received;
    }

    public long getErrors() {
        return errors;
    }

    public long getRxVC0() {
        return rxVC0;
    }

    public long getRxVC1() {
        return rxVC1;
    }

    public long getRxVC2() {
        return rxVC2;
    }

    public long getRxVC3() {
        return rxVC3;
    }
}
