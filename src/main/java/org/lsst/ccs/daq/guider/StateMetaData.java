package org.lsst.ccs.daq.guider;

import org.lsst.ccs.utilities.location.SensorLocation;
import java.time.Instant;
import org.lsst.ccs.utilities.location.Location;

/**
 * Meta-data corresponding to the instantaneous state of the guider
 */
public class StateMetaData {

    public enum State {
        STAMP, START, STOP, PAUSE, RESUME
    }

    public enum Status {
        SUCCESS, bdiError
    }
    private final State state;
    private final Status status;
    private final int sequence;
    private final int stamp;
    private final Instant timestamp;
    private final SensorLocation sensorLocation;
    private final String comment;

    @SuppressWarnings("unused")
    private StateMetaData(int type, int status, int sequence, int stamp, long timestampNanos, byte bay, byte board, int sensor, String comment) {
        this.state = State.values()[type];
        this.status = Status.values()[status];
        this.sequence = sequence;
        this.stamp = stamp;
        this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.sensorLocation = new SensorLocation(new Location(bay, board), sensor);
        this.comment = comment;
    }

    public State getState() {
        return state;
    }

    public Status getStatus() {
        return status;
    }

    public int getSequence() {
        return sequence;
    }

    public int getStamp() {
        return stamp;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public SensorLocation getSensorLocation() {
        return sensorLocation;
    }

    public String getComment() {
        return comment;
    }

    @Override
    public String toString() {
        return "StateMetaData{" + "state=" + state + ", status=" + status + ", sequence=" + sequence + ", stamp=" + stamp + ", timestamp=" + timestamp + ", sensorLocation=" + sensorLocation + ", comment=" + comment + '}';
    }
}
