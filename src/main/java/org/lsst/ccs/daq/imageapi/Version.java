package org.lsst.ccs.daq.imageapi;

/**
 *
 * @author tonyj
 */
public class Version {
    private final String tag;
    private final long buildTime;
    private final boolean dirty;
    private final int hash;    

    Version(String tag, long buildTime, boolean dirty, int hash) {
        this.tag = tag;
        this.buildTime = buildTime;
        this.dirty = dirty;
        this.hash = hash;
    }

    @Override
    public String toString() {
        return "Version{" + "tag=" + tag + ", buildTime=" + buildTime + ", dirty=" + dirty + ", hash=" + hash + '}';
    }
}
