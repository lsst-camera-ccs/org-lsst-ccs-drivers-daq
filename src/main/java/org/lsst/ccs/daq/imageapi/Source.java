package org.lsst.ccs.daq.imageapi;

import java.nio.channels.ByteChannel;

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
    
    public enum ChannelMode {
        READ, WRITE, STREAM
    };

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

    public long size() {
        return metaData.getLength();
    }

    public SourceType getSourceType() {
        return metaData.getSensor();
    }

    Image getImage() {
        return image;
    }
    
    public ByteChannel openChannel(ChannelMode mode) throws DAQException {
        return DAQSourceChannel.open(this, mode);
    }

    void addStreamListener(StreamListener listener) {
        image.getStore().addStreamListener(this.image.getMetaData().getId(), this.getLocation().index(), listener);
    }

    void removeStreamListener(StreamListener listener) {
        image.getStore().removeStreamListener(this.image.getMetaData().getId(), this.getLocation().index(), listener);
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
