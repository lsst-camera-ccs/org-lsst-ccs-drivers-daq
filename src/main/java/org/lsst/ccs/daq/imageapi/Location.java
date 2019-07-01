package org.lsst.ccs.daq.imageapi;

/**
 * Corresponds to a REB location in the focal plane e.g. R22/Reb0
 * @author tonyj
 */
public class Location {
    private final byte bay;
    private final byte board;

    public Location(byte bay, byte board) {
        this.bay = bay;
        this.board = board;
    }

    int index() {
        return 3*bay+board;
    }
    
    @Override
    public String toString() {
        return String.format("R%02d/Reb%d",bay,board);
    }
}
