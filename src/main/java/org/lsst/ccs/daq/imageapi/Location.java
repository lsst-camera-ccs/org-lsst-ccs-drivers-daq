package org.lsst.ccs.daq.imageapi;

/**
 * Corresponds to a REB location in the focal plane e.g. R22/Reb0
 *
 * @author tonyj
 */
public class Location implements Comparable<Location> {

    // Note: This is chosen for compatibility with DAQ::LocationSet
    private static final int BAY_MULTIPLIER = 4;
    private final static int INVALID_INDEX = -1;
    private final static int[] BAY_TO_INDEX = new int[]{
        0, 1, 2, 3, 4, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX,
        5, 6, 7, 8, 9, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX,
        10, 11, 12, 13, 14, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX,
        15, 16, 17, 18, 19, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX,
        20, 21, 22, 23, 24, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX};
    private final static int[] INDEX_TO_BAY = new int[]{
        0, 1, 2, 3, 4, 10, 11, 12, 13, 14, 20, 21, 22, 23, 24, 30, 31, 32, 33, 34, 40, 41, 42, 43, 44};

    private final byte bay;
    private final byte board;
    
    Location(int index) {
        bay = (byte) INDEX_TO_BAY[index / BAY_MULTIPLIER];
        board = (byte) (index % BAY_MULTIPLIER);
    }

    public Location(byte bay, byte board) {
        if (bay < 0 || bay >= BAY_TO_INDEX.length || BAY_TO_INDEX[bay] == INVALID_INDEX) {
            throw new IllegalArgumentException("Invalid bay: " + bay);
        }
        if (board < 0 || board > 3) {
            throw new IllegalArgumentException("Invalid board: " + board);
        }
        this.bay = bay;
        this.board = board;
    }

    int index() {
        return BAY_MULTIPLIER * BAY_TO_INDEX[bay] + board;
    }

    @Override
        public String toString() {
        return String.format("R%02d/Reb%d", bay, board);
    }

    @Override
        public int compareTo(Location o) {
        return this.index() - o.index();
    }

    @Override
        public int hashCode() {
        return index();
    }

    @Override
        public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        return this.index() == ((Location) obj).index();
    }
}
