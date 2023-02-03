package org.lsst.ccs.daq.guider;

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
    private final long sequence;
    private final Instant timestamp;
    private final SensorLocation sensorLocation;

    private StateMetaData(int type, int status, int sequence, long timestampNanos, byte bay, byte board, int sensor) {
        this.state = State.values()[type];
        this.status = Status.values()[status];
        this.sequence = sequence;
        this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.sensorLocation = new SensorLocation(new Location(bay, board), sensor);
    }

    public State getState() {
        return state;
    }

    public Status getStatus() {
        return status;
    }

    public long getSequence() {
        return sequence;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public SensorLocation getSensorLocation() {
        return sensorLocation;
    }

    @Override
    public String toString() {
        return "StateMetaData{" + "state=" + state + ", status=" + status + ", sequence=" + sequence + ", timestamp=" + timestamp + ", sensorLocation=" + sensorLocation + '}';
    }
    
}
