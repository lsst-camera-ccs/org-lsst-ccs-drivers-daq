package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import org.lsst.ccs.bus.annotations.DataAttributes;

/**
 * This class describes statistics kept by the low level PGP driver code on the
 * RCE. It is notable as the level where the four virtual channels are split
 * out. All counts represent PGP frames received. Where they equal the counters
 * int he firmware statistics, they differ in being 64-bit quantities.
 *
 * NOTE: Here's as good a place as any to describe how data is assigned to each
 * virtual channel. VC0 is the command/response channel. All register traffic
 * (RMS) occurs on VC0. Also, the LAM (Look At Me) interrupts from the REB are
 * transmitted to the RCE on VC0. VC1,2,3 are used for image data. For Science
 * and Wavefront sources, only VC1 is used. There was a chance (now vanishingly
 * small) that we would transmit the data for each of the 3 CCDs on a Science
 * REB on different VCs. This is what is meant when you hear about 3 sequencer
 * REB firmware. This is also true of the Guider when operated in "Science
 * Mode." When the Guider is in guide mode, the data from the two CCDs will be
 * sent on VCs 1 and 2.
 */
@DataAttributes(units = "unitless")
public class DAQDriverStats extends DAQStats implements Serializable {

    /* The statistics quantities */
    @DataAttributes(description = "PGP Frames Received. The total number of PGP frames received. This should equal with the rx_frames from the Firmware statistics.")
    private final long received;
    @DataAttributes(description = "PGP Frame Errors. The total number of PGP frames received in error. This should equal the rx_errors from the Firmware statistics.")
    private final long errors;
    @DataAttributes(description = "PGP Frames Received (channel 0). Counts the number of PGP frames received on each virtual channel.")
    private final long rx0;
    @DataAttributes(description = "PGP Frames Received (channel 1). Counts the number of PGP frames received on each virtual channel.")
    private final long rx1;
    @DataAttributes(description = "PGP Frames Received (channel 2). Counts the number of PGP frames received on each virtual channel.")
    private final long rx2;
    @DataAttributes(description = "PGP Frames Received (channel 3). Counts the number of PGP frames received on each virtual channel.")
    private final long rx3;
    private static final long serialVersionUID = 4894440689055979272L;

    DAQDriverStats(byte bay, byte board, long received, long errors, long rx0, long rx1, long rx2, long rx3) {

        super(6, bay, board);

        this.received = received;
        this.errors = errors;
        this.rx0 = rx0;
        this.rx1 = rx1;
        this.rx2 = rx2;
        this.rx3 = rx3;

        statMap.put("received", received);
        statMap.put("errors", errors);
        statMap.put("rx0", rx0);
        statMap.put("rx1", rx1);
        statMap.put("rx2", rx2);
        statMap.put("rx3", rx3);
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
