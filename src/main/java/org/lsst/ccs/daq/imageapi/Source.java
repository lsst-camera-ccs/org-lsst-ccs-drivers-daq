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
        private final int nRebs;

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
        return metaData.getSensor();
    }

    public void readRaw(ByteBuffer buffer) {
        image.readRaw(buffer, metaData.getLocation());
    }

    public void writeRaw(ByteBuffer buffer) {
        System.out.printf("Location %s index %d\n",metaData.getLocation(),metaData.getLocation().index());
        image.writeRaw(buffer, metaData.getLocation());
    }
    
    public void close() {
        image.close(metaData.getLocation());
    }

    @Override
    public int compareTo(Source o) {
        return this.metaData.getLocation().compareTo(o.metaData.getLocation());
    }

    @Override
    public String toString() {
        return "Source{" + "metaData=" + metaData + ", image=" + image + '}';
    }
}
