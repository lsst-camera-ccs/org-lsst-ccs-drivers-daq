package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.nio.channels.ByteChannel;

/**
 * A reference to a source within an image. Each source represents the data
 * from the set of CCDs connected to a given REB.
 * @see Image#listSources()
 *
 * @author tonyj
 */
public class Source implements Comparable<Source> {

    private SourceMetaData metaData;
    private final Image image;
    private final Location location;
    
    public enum ChannelMode {
        /**
         * A channel opened for reading data from an image already existing in the Store.
         */
        READ, 
        /**
         * A channel for writing data to a newly created image. 
         */
        WRITE, 
        /**
         * A channel opened for streaming data from an image as it is created in the store.
         */
        STREAM
    };

    /**
     * Create a source with no meta-data, useful if source meta-data has not necessarily
     * been created yet.
     * @param image
     * @param location 
     */
    Source(Image image, Location location) {
        this.image = image;
        this.location = location;
        this.metaData = null;
    }

    Source(Image image, SourceMetaData metaData) {
        this.metaData = metaData;
        this.location = metaData.getLocation();
        this.image = image;
    }

    /**
     * Get meta-data associated with this source
     * @return The associate metadata.
     * @throws DAQException
     */
    public SourceMetaData getMetaData() throws DAQException {
        if (metaData == null) {
            metaData = image.getStore().findSource(image.getMetaData().getId(), location.index());
        }
        return metaData;
    }

    /**
     * Get the location within the focal plane of this source
     * @return THe location.
     */
    public Location getLocation() {
        return location;
    }

    /**
     * The total size of the raw data associated with this source.This may be zero
     * of the source has been newly created and not yet written, or if the source
     * is in the process of being streamed to the DAQ store.
     * @return The image size, or <code>0</code> if the image is not completely written.
     * @throws DAQException
     */
    public long size() throws DAQException {
        return getMetaData().getLength();
    }

    /**
     * The type of REB this source corresponds to.
     * @return The source type.
     * @throws DAQException
     */
    public Location.LocationType getSourceType() throws DAQException {
        return getMetaData().getSensor();
    }

    public Image getImage() {
        return image;
    }

    /**
     * Open a channel for reading or writing data to this source.
     * @param mode The mode to open the channel.
     * @return The created channel.
     * @throws DAQException If the mode is invalid for the current state of this source, 
     * or some other error occurs.
     */    
    public ByteChannel openChannel(ChannelMode mode) throws DAQException {
        return DAQSourceChannel.open(this.image.getStore(), this, mode);
    }    
    
    /**
     * Open a channel for reading or writing data to this source.
     * @param store The store to be used for read/write operations. For multi-threaded reading-writing there must be one store per channel
     * @param mode The mode to open the channel.
     * @return The created channel.
     * @throws DAQException If the mode is invalid for the current state of this source, 
     * or some other error occurs.
     */
    public ByteChannel openChannel(Store store, ChannelMode mode) throws DAQException {
        return DAQSourceChannel.open(store, this, mode);
    }

    void addStreamListener(StreamListener listener) {
        image.getStore().addStreamListener(this.image.getMetaData().getId(), this.getLocation().index(), listener);
    }

    void removeStreamListener(StreamListener listener) {
        image.getStore().removeStreamListener(this.image.getMetaData().getId(), this.getLocation().index(), listener);
    }
    
    @Override
    public int compareTo(Source o) {
        return location.compareTo(o.location);
    }

    @Override
    public String toString() {
        return "Source{" + "metaData=" + metaData + ", image=" + image + ", location=" + location + '}';
    }
}
