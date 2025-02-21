package org.lsst.ccs.daq.guider;

public class ClearParameters {
    private final int delay;
    private final int preRows;
    private final int flushCount;
    private final int readRows;
    private final static ClearParameters DEFAULT_CLEAR_PARAMETERS = new ClearParameters(50, 100, 2, 100);

    public ClearParameters(int delay, int preRows, int flushCount, int readRows) {
        this.delay = delay;
        this.preRows = preRows;
        this.flushCount = flushCount;
        this.readRows = readRows;
    }

    public int getDelay() {
        return delay;
    }

    public int getPreRows() {
        return preRows;
    }

    public int getFlushCount() {
        return flushCount;
    }

    public int getReadRows() {
        return readRows;
    }

    @Override
    public String toString() {
        return "ClearParameters [delay=" + delay + ", preRows=" + preRows + ", flushCount=" + flushCount + ", readRows="
                + readRows + "]";
    }

    public static ClearParameters defaultValue() {
        return DEFAULT_CLEAR_PARAMETERS;
    }
}
