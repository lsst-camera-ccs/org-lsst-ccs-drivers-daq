package org.lsst.ccs.daq.ims;

import java.io.Serializable;
import java.time.Instant;

/**
 * Representation of a DAQ version.
 *
 * @author tonyj
 */
public class Version implements Serializable {

    private static final long serialVersionUID = -1977918511697870015L;

    private final String tag;
    private final Instant buildTime;
    private final boolean dirty;
    private final int hash;

    Version(String tag, long timestampNanos, boolean dirty, int hash) {
        this.tag = tag;
        this.buildTime = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.dirty = dirty;
        this.hash = hash;
    }

    @Override
    public String toString() {
        return String.format("%s %s (%s%x)", tag, buildTime, dirty ? "dirty," : "", hash);
    }
}
