package org.lsst.ccs.daq.ims;

import java.time.Instant;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;

/**
 * Representation of a DAQ version.
 *
 * @author tonyj
 */
public class Version {

    private final static DateTimeFormatter DATE_TIME_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")
            .withZone(ZoneId.of("UTC"));
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
        return String.format("%s %s(%s,%x)", tag, DATE_TIME_FORMATTER.format(buildTime), dirty ? "dirty" : "", hash);
    }
}
