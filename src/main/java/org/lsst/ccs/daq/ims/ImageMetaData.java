package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.LocationSet;
import java.io.Serializable;
import java.time.Instant;
import java.util.BitSet;
import java.util.Collections;
import java.util.Set;

/**
 * Meta-data associated with an image.
 *
 * @author tonyj
 */
public class ImageMetaData implements Serializable {

    private static final long serialVersionUID = 6128502939552309548L;

    private final String name;
    private final String annotation;
    private final Version release;
    private final int opcode;
    private final Instant timestamp;
    private final LocationSet elements;
    private final long id;
    private final String creationFolder;

    ImageMetaData(long id, String name, String folderName, String annotation, Version release, int opcode, long timestampNanos, BitSet elements) {
        this.id = id;
        this.name = name;
        this.annotation = annotation;
        this.release = release;
        this.opcode = opcode;
        this.timestamp = Instant.ofEpochSecond(timestampNanos / 1_000_000_000, timestampNanos % 1_000_000_000);
        this.elements = new LocationSet(elements);
        this.creationFolder = folderName;
    }
    
    ImageMetaData(long id, Version release, Instant timestamp, ImageMetaData meta) {
        this.id = id;
        this.timestamp = timestamp;
        this.release = release;
        this.annotation = meta.annotation;
        this.creationFolder = meta.creationFolder;
        this.elements = meta.elements;
        this.opcode = meta.opcode;
        this.name = meta.name;
    }

    /**
     * A constructor to be used when creating new images to be written into the
     * Store or when triggering the camera
     *
     * @param imageName The name of the newly created image.
     * @param creationFolder The folder in which the image was originally created.
     * @param annotation An annotation string to be associated with the image.
     * @param opcode The "opcode" associated with the image. This normally
     * corresponds to the index of the main routine in the sequencer which
     * created this image.
     * @param locationSet The set of locations to be associated with the image.
     * After creating the imaqe a {@code Source} should be added for each
     * location specified in the locationSet.
     * @see Folder#insert(ImageMetaData)
     * @see Image#addSource(Location, int[])
     */
    public ImageMetaData(String imageName, String creationFolder, String annotation, int opcode, Set<Location> locationSet) {
        this.name = imageName;
        this.creationFolder = creationFolder;
        this.annotation = annotation;
        this.release = null;
        this.opcode = opcode;
        this.elements = new LocationSet(locationSet);
        this.timestamp = Instant.EPOCH;
        this.id = 0;
    }

    /**
     * Get the image name.
     *
     * @return The name.
     */
    public String getName() {
        return name;
    }

    /**
     * Get the annotation associated with the image when it was created.
     *
     * @return The annotation
     */
    public String getAnnotation() {
        return annotation;
    }

    /**
     * Get the DAQ release which created this image.
     *
     * @return The DAQ version
     */
    public Version getRelease() {
        return release;
    }

    /**
     * Get the opcode associated with this image. This normally corresponds to
     * the index of the main routine in the sequencer which created this image.
     *
     * @return The opcode.
     */
    public int getOpcode() {
        return opcode;
    }

    /**
     * Get the timestamp associated with this image.
     *
     * @return The timestamp
     */
    public Instant getTimestamp() {
        return timestamp;
    }

    /**
     * Get the set of locations (REBs) associated with this image
     *
     * @return
     */
    public Set<Location> getLocations() {
        return Collections.unmodifiableSet(elements);
    }

    /**
     * The opaque id associated with the image by the DAQ.
     *
     * @return The id
     */
    public long getId() {
        return id;
    }

    /**
     * Get the folder in which the image was originally created
     *
     * @return The image creation folder name
     */
    public String getCreationFolderName() {
        return creationFolder;
    }

    BitSet getLocationBitSet() {
        return elements.getBitSet();
    }

    @Override
    public String toString() {
        return "ImageMetaData{" + "name=" + name + ", annotation=" + annotation + ", release=" + release + ", opcode=" + opcode + ", timestamp=" + timestamp + ", elements=" + elements + ", id=" + id + ", creationFolder=" + creationFolder + '}';
    }

}
