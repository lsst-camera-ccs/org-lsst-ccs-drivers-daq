package org.lsst.ccs.daq.imageapi;

/**
 * Corresponds to a REB location in the focal plane e.g. R22/Reb0
 *
 * @author tonyj
 */
public class Location implements Comparable<Location> {
    // Note: This is chosen for compatibility with DAQ::LocationSet
    private static final int BAY_MULTIPLIER = 4;
    private final byte bay;
    private final byte board;

    Location(int index) {
        bay = (byte) (index / BAY_MULTIPLIER);
        board = (byte) (index % BAY_MULTIPLIER);
    }

    public Location(byte bay, byte board) {
        this.bay = bay;
        this.board = board;
    }

    int index() {
        return BAY_MULTIPLIER * bay + board;
    }

    @Override
    public String toString() {
        return String.format("R%02d/Reb%d", bay, board);
    }

    @Override
    public int compareTo(Location o) {
        return this.index() - o.index();
    }
}
