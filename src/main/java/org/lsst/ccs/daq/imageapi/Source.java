package org.lsst.ccs.daq.imageapi;

import java.nio.ByteBuffer;
import javax.tools.DocumentationTool.Location;

/**
 * Reference to both Source data + metadata buckets
 *
 * @author tonyj
 */
public class Source implements Comparable<Source> {

    private final SourceMetaData metaData;

    private enum Stripe {
        STRIPE0, STRIPE1, STRIPE2
    }

    private enum SourceType {
        SCIENCE, WAVEFRONT, GUIDER
    }

    Source(SourceMetaData metaData) {
        this.metaData = metaData;
    }

    /**
     * Get meta-data associated with this source
     *
     * @return
     */
    public SourceMetaData getMetaData() {
        return metaData;
    }

    public Location getLocation() {
        return null;
    }

    public long size() {
        return 0;
    }

    public SourceType getSourceType() {
        return null;
    }

    /**
     * Fetch decoded pixel data
     *
     * @param stripeMask Mask with bit set for each stripe to be read
     * @param stripeBuffer A set of stripeBuffers to receive the data. Must
     * provide one stripeBuffer for each bit set in the stripeMask
     * @throws IllegalArgumentException if the number of bits set in the stripe
     * mask does not match the number of stripeBuffers provided, or if the
     * stripeMask is not legal for the SourceType.
     */
    void decodePixels(int stripeMask, ByteBuffer... stripeBuffer) throws IllegalArgumentException {

    }

    @Override
    public int compareTo(Source o) {
        return this.metaData.getLocation().compareTo(o.metaData.getLocation());

    }

}
