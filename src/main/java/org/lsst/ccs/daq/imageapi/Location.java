package org.lsst.ccs.daq.imageapi;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Corresponds to a REB location in the focal plane e.g.&nbsp;R22/Reb0.
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

    private final static Pattern PATTERN = Pattern.compile("R(\\d\\d)/Reb(\\d)");

    private final byte bay;
    private final byte board;

    Location(int index) {
        bay = (byte) INDEX_TO_BAY[index / BAY_MULTIPLIER];
        board = (byte) (index % BAY_MULTIPLIER);
    }

    /**
     * Create location from bay number and board number
     * @param bay Bay (e.g.\ 22, 10, 04 etc)
     * @param board Board (0-2)
     */
    public Location(int bay, int board) {
        if (bay < 0 || bay >= BAY_TO_INDEX.length || BAY_TO_INDEX[bay] == INVALID_INDEX) {
            throw new IllegalArgumentException("Invalid bay: " + bay);
        }
        if (board < 0 || board > 2) {
            throw new IllegalArgumentException("Invalid board: " + board);
        }
        this.bay = (byte) bay;
        this.board = (byte) board;
    }

    /**
     * Create location from string
     * @param location String representation of location, of form Rnn/Rebm
     * @return The corresponding location
     */
    public static Location of(String location) {
        Matcher matcher = PATTERN.matcher(location);
        if (!matcher.matches()) {
            throw new IllegalArgumentException("Illegal location: " + location);
        }
        int bay = Integer.parseInt(matcher.group(1));
        int board = Integer.parseInt(matcher.group(2));
        return new Location(bay, board);
    }
    
    /**
     * The source type for this location
     * @return The source type
     */
    public Source.SourceType type() {
       if (bay == 0 || bay == 40 || bay == 44 || bay == 04) {
           if (board == 0) return  Source.SourceType.WAVEFRONT;
           else return Source.SourceType.GUIDER;
       } else {
           return Source.SourceType.SCIENCE;
       }
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
