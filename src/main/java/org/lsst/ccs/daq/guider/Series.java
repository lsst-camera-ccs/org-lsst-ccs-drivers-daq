package org.lsst.ccs.daq.guider;

import org.lsst.ccs.utilities.location.SensorLocation;
import java.time.Instant;
import java.util.List;

/**
 *
 * @author tonyj
 */
public class Series {
    
    private final Status status;
    private final Instant begin;
    private final int sequence;
    private final int stamps;
    private final List<SensorLocation> configured;
    private final List<SensorLocation> remaining;

    private Series(Status status, long timestampNanos, int sequence, int stamps, List<SensorLocation> configured, List<SensorLocation> remaining) {
        this.status = status;
        this.begin = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.sequence = sequence;
        this.stamps = stamps;
        this.configured = configured;
        this.remaining = remaining;
    }

    public Status getStatus() {
        return status;
    }

    public Instant getBegin() {
        return begin;
    }

    public int getSequence() {
        return sequence;
    }

    public int getStamps() {
        return stamps;
    }

    public List<SensorLocation> getConfigured() {
        return configured;
    }

    public List<SensorLocation> getRemaining() {
        return remaining;
    }

    @Override
    public String toString() {
        return "Series{" + "status=" + status + ", begin=" + begin + ", sequence=" + sequence + ", stamps=" + stamps + ", configured=" + configured + ", remaining=" + remaining + '}';
    }
    
}
