package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import org.lsst.ccs.bus.annotations.DataAttributes;

/**
 * This class describes the statistics reported by the RMS (Raft Management
 * Service) or register reads and writes. For each DAQ Source, lists of register
 * operations can be posted, and responses gathered.
 *
 * The RCE is always waiting for pixel data to arrive from the REB. When it
 * encounters data with the Start of Image flag, it checks to see if it has
 * received any Image Metadata. If none arrives within a timeout (which can
 * happen if this Source was not included in the Image) then the pixel data is
 * discarded until the End of Image flag is seen. If the Image Metadata is
 * present, then the RCE writes the Source Metadata to the Data Store, and
 * begins processing the pixel data.
 */
@DataAttributes(units = "unitless")
public class DAQRdsStats extends DAQStats implements Serializable {

    /* The statistics quantities */
    @DataAttributes(description = "Wakes count. This counts the number of times the RDS server was awakened to put Image Metadata into it's processing queue.")
    private final long wakes;
    @DataAttributes(description = "Discards count. The number of times the Server tried to insert Image Metadata into the processing queue but failed to do so. (Presumably because the queue was full.)")
    private final long discards;
    @DataAttributes(description = "No Event count. Upon the arrival of pixel data, there was no Image Metadata available to associate with it. The pixel data is discarded.")
    private final long no_event;
    @DataAttributes(description = "Bulk Data Interface Waits count. The number of times we have waited for a page of pixel data from the Bulk Data Interface.")
    private final long bdi_waits;
    @DataAttributes(description = "BDI Null Pages count. The BDI woke up but no data was present.")
    private final long bdi_nulls;
    @DataAttributes(description = "BDI Sequence Errors count. The BDI can mark pages as coming out of sequence. ")
    private final long bdi_seq_errors;
    @DataAttributes(description = "BDI Overflows count. The BDI can report that its data buffers have overflowed.")
    private final long bdi_overflows;
    @DataAttributes(description = "BDI Firsts count. The number of BDI pages marked as First (Start of Image flag is set.)")
    private final long bdi_firsts;
    @DataAttributes(description = "BDI Lasts count. The number of BDI pages marked as last (End of Image flag is set.)")
    private final long bdi_lasts;
    @DataAttributes(description = "BDI Frees count. The number of BDI pages processed.")
    private final long bdi_frees;
    private static final long serialVersionUID = -6474883615404798290L;

    public DAQRdsStats(byte bay, byte board, long wakes, long discards, long no_event, long bdi_waits, long bdi_nulls, long bdi_seq_errors, long bdi_overflows, long bdi_firsts, long bdi_lasts, long bdi_frees) {

        super(20, bay, board);

        this.wakes = wakes;
        this.discards = discards;
        this.no_event = no_event;
        this.bdi_waits = bdi_waits;
        this.bdi_nulls = bdi_nulls;
        this.bdi_seq_errors = bdi_seq_errors;
        this.bdi_overflows = bdi_overflows;
        this.bdi_firsts = bdi_firsts;
        this.bdi_lasts = bdi_lasts;
        this.bdi_frees = bdi_frees;

        statMap.put("wakes", wakes);
        statMap.put("discards", discards);
        statMap.put("no_event", no_event);
        statMap.put("bdi_waits", bdi_waits);
        statMap.put("bdi_nulls", bdi_nulls);
        statMap.put("bdi_seq_errors", bdi_seq_errors);
        statMap.put("bdi_overflows", bdi_overflows);
        statMap.put("bdi_firsts", bdi_firsts);
        statMap.put("bdi_lasts", bdi_lasts);
        statMap.put("bdi_frees", bdi_frees);
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
