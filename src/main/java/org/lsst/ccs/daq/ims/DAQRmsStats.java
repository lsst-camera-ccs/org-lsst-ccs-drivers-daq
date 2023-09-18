package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import org.lsst.ccs.bus.annotations.DataAttributes;

/**
 * This class describes the statistics reported by the RMS (Raft Management
 * Service) or register reads and writes. For each DAQ Source, lists of register
 * operations can be posted, and responses gathered.
 */
@DataAttributes(units = "unitless")
public class DAQRmsStats extends DAQStats implements Serializable {

    /* The statistics quantities */
    @DataAttributes(description = "Probe Counter. The number of times the RMS server has been probed by Clients. Normally this is once per posted list, but can also be done to detect the presence of RMS servers.")
    private final long probes;
    @DataAttributes(description = "Plugin Unopened error count. The window for this error to be encountered is very small. When a request cannot be fulfilled because the Plugin is not yet initialized.")
    private final long unopened;
    @DataAttributes(description = "Link In Reset error count. When a request cannot be fulfilled because the link is being reset.")
    private final long in_reset;
    @DataAttributes(description = "Link is Down error count. When a request cannot be fulfilled because the PGP Link is down.")
    private final long link_down;
    @DataAttributes(description = "Lists Posted count. The number of requests (Lists of register operations) posted the Server.")
    private final long lists_posted;
    @DataAttributes(description = "Register Operation Requests count. The number of register operation requests sent to the REB.")
    private final long ops_posted;
    @DataAttributes(description = "Waits count. The number of times RMS has been presented with a PGP frame from the firmware.")
    private final long waits;
    @DataAttributes(description = "Register Operation Responses count. The number of register operation responses from the REB. Should be equal to the number of Waits and Operation Requests.")
    private final long responses;
    @DataAttributes(description = "Invalid Task ID count. A Response was received that does not have a valid identifier to use to pair it with a Request.")
    private final long inv_tids;
    @DataAttributes(description = "Invalid Task ID count. A Response was received that does not have a valid identifier to use to pair it with a Request.")
    private final long orphans;
    @DataAttributes(description = "Orphaned Response count. A Response was received for which there was no corresponding request.")
    private final long rundowns;
    @DataAttributes(description = "Task ID High Water Mark. There is a fixed number of buffers available to store outstanding requests while waiting for responses. If the REB does not respond to a request, then a buffer resource is lost. This counter reflects the largest number of outstanding requests. Currently, in a properly functioning system there should never be more than one outstanding request at a time, and the High Water Mark should never be more than 1.")
    private final long tid_hwm;
    private static final long serialVersionUID = -6149058303405500713L;

    DAQRmsStats(byte bay, byte board, long probes, long unopened, long in_reset, long link_down, long lists_posted, long ops_posted, long waits, long responses, long inv_tids, long orphans, long rundowns, long tid_hwm) {

        super(20, bay, board);
        this.probes = probes;
        this.unopened = unopened;
        this.in_reset = in_reset;
        this.link_down = link_down;
        this.lists_posted = lists_posted;
        this.ops_posted = ops_posted;
        this.waits = waits;
        this.responses = responses;
        this.inv_tids = inv_tids;
        this.orphans = orphans;
        this.rundowns = rundowns;
        this.tid_hwm = tid_hwm;

        statMap.put("probes", probes);
        statMap.put("unopened", unopened);
        statMap.put("in_reset", in_reset);
        statMap.put("link_down", link_down);
        statMap.put("lists_posted", lists_posted);
        statMap.put("ops_posted", ops_posted);
        statMap.put("waits", waits);
        statMap.put("responses", responses);
        statMap.put("inv_tids", inv_tids);
        statMap.put("orphans", orphans);
        statMap.put("rundowns", rundowns);
        statMap.put("tid_hwm", tid_hwm);
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
