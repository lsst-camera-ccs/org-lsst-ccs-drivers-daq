package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import org.lsst.ccs.bus.annotations.DataAttributes;

@DataAttributes(units="unitless")
public class DAQDriverStats extends DAQStats implements Serializable {

    /* The statistics quantities */
    private final long received;
    private final long errors;  
    private final long rx0;   
    private final long rx1;   
    private final long rx2;   
    private final long rx3;   


    DAQDriverStats(byte bay, byte board, long received, long errors, long rx0, long rx1, long rx2, long rx3) {

        super(6, bay, board);

        this.received =  received;
        this.errors   =  errors;  
        this.rx0      =  rx0;   
        this.rx1      =  rx1;   
        this.rx2      =  rx2;   
        this.rx3      =  rx3;   

        statMap.put("received", received);
        statMap.put("errors"  , errors  );
        statMap.put("rx0"     , rx0   );
        statMap.put("rx1"     , rx1   );
        statMap.put("rx2"     , rx2   );
        statMap.put("rx3"     , rx3   );
    }

    public long getReceived() {
        return received;
    }

    public long getErrors() {
        return errors;
    }

    public long getRx0() {
        return rx0;
    }

    public long getRx1() {
        return rx1;
    }

    public long getRx2() {
        return rx2;
    }

    public long getRx3() {
        return rx3;
    }
}
