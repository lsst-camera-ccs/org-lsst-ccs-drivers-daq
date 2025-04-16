package org.lsst.ccs.daq.guider;

import java.time.Instant;

/**
 *
 * @author tonyj
 */
public class Status {

    public enum State {
        UNDEFINED, IDLECLEARING, PAUSED, RUNNING, ERROR, OFF, CLEARING
    }
    private final Instant timestamp;
    private final int status;
    private final int sequence;
    private final State in;
    private final State out;

    private Status(long timestampNanos, int status, int sequence, int inState, int outState) {
        this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.status = status;
        this.sequence = sequence;
        this.in = State.values()[inState];
        this.out = State.values()[outState];
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public int getStatus() {
        return status;
    }

    public int getSequence() {
        return sequence;
    }

    public State getIn() {
        return in;
    }

    public State getOut() {
        return out;
    }

    @Override
    public String toString() {
        return "Status{" + "timestamp=" + timestamp + ", status=" + status + ", sequence=" + sequence + ", in=" + in + ", out=" + out + '}';
    }

}
