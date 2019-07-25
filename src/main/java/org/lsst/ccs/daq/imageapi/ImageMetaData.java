package org.lsst.ccs.daq.imageapi;

import java.util.BitSet;
import java.util.Collections;
import java.util.Set;

/**
 * Meta-data associated with an image
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

    public ImageMetaData(String imageName, String annotation, int opcode, Set<Location> locationSet) {
        this.name = imageName;
        this.annotation = annotation;
        this.release = null;
        this.opcode = opcode;
        this.elements = new LocationSet(locationSet);
        this.timestamp = 0;
        this.id = 0;
    }

    public String getName() {
        return name;
    }

    public String getAnnotation() {
        return annotation;
    }

    public Version getRelease() {
        return release;
    }

    public int getOpcode() {
        return opcode;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public Set<Location> getLocations() {
        return Collections.unmodifiableSet(elements);
    }

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
