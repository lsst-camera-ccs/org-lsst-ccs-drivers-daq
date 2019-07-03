package org.lsst.ccs.daq.imageapi;

import java.nio.ByteBuffer;

/**
 * Reference to both Source data + metadata buckets
 *
 * @author tonyj
 */
public class Source implements Comparable<Source> {

    private final SourceMetaData metaData;
    private final Image image;

    public enum SourceType {
        WAVEFRONT(1), GUIDER(2), SCIENCE(3);
        private int nRebs;

        SourceType(int nRebs) {
            this.nRebs = nRebs;
        }

        public int getNRebs() {
            return nRebs;
        }
    }

    Source(Image image, SourceMetaData metaData) {
        this.metaData = metaData;
        this.image = image;
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
        return metaData.getLocation();
    }

    public int size() {
        return metaData.getLength();
    }

    public SourceType getSourceType() {
        return null;
    }

    void readRaw(ByteBuffer buffer) {
        image.readRaw(buffer, this.metaData.getLocation());
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
