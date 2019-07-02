package org.lsst.ccs.daq.imageapi;

import java.util.BitSet;

/**
 * Meta-data associated with an image
 * @author tonyj
 */
class ImageMetaData {
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

    public LocationSet getElements() {
        return elements;
    }

    public long getId() {
        return id;
    }

    @Override
    public String toString() {
        return "ImageMetaData{" + "name=" + name + ", annotation=" + annotation + ", release=" + release + ", opcode=" + opcode + ", timestamp=" + timestamp + ", elements=" + elements + ", id=" + id + '}';
    }
}
