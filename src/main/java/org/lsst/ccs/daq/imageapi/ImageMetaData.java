package org.lsst.ccs.daq.imageapi;

import java.util.BitSet;
import java.util.Collections;
import java.util.Set;

/**
 * Meta-data associated with an image.
 * @author tonyj
 */
public class ImageMetaData {
    private final String name;
    private final String annotation;
    private final Version release;
    private final int opcode;
    private final long timestamp;
    private final LocationSet elements;
    private final long id;

    ImageMetaData(long id, String name, String annotation, Version release, int opcode, long timestamp, BitSet elements) {
        this.id = id;
        this.name = name;
        this.annotation = annotation;
        this.release = release;
        this.opcode = opcode;
        this.timestamp = timestamp;
        this.elements = new LocationSet(elements);
    }

    /**
     * A constructor to be used when creating new images to be written into the Store.
     * @param imageName The name of the newly created image.
     * @param annotation An annotation string to be associated with the image.
     * @param opcode The "opcode" associated with the image. This normally corresponds to the index of 
     * the main routine in the sequencer which created this image.
     * @param locationSet The set of locations to be associated with the image. After creating the imaqe
     * a {@code Source} should be added for each location specified in the locationSet.
     * @see Folder#insert(ImageMetaData) 
     * @see Image#addSource(Location, int[]) 
     */
    public ImageMetaData(String imageName, String annotation, int opcode, Set<Location> locationSet) {
        this.name = imageName;
        this.annotation = annotation;
        this.release = null;
        this.opcode = opcode;
        this.elements = new LocationSet(locationSet);
        this.timestamp = 0;
        this.id = 0;
    }

    /**
     * Get the image name.
     * @return The name.
     */
    public String getName() {
        return name;
    }

    /**
     * Get the annotation associated with the image when it was created.
     * @return The annotation
     */
    public String getAnnotation() {
        return annotation;
    }

    /**
     * Get the DAQ release which created this image.
     * @return The DAQ version
     */
    public Version getRelease() {
        return release;
    }

    /**
     * Get the opcode associated with this image. This normally corresponds to the index of 
     * the main routine in the sequencer which created this image. 
     * @return The opcode.
     */
    public int getOpcode() {
        return opcode;
    }

    /**
     * Get the timestamp associated with this image.
     * @return The timestamp
     */
    public long getTimestamp() {
        return timestamp;
    }

    /**
     * Get the set of locations (REBs) associated with this image
     * @return 
     */
    public Set<Location> getLocations() {
        return Collections.unmodifiableSet(elements);
    }

    /**
     * The opaque id associated with the image by the DAQ.
     * @return The id
     */
    public long getId() {
        return id;
    }

    BitSet getLocationBitSet() {
        return elements.getBitSet();
    }
    
    @Override
    public String toString() {
        return "ImageMetaData{" + "name=" + name + ", annotation=" + annotation + ", release=" + release + ", opcode=" + opcode + ", timestamp=" + timestamp + ", elements=" + elements + ", id=" + id + '}';
    }

}
