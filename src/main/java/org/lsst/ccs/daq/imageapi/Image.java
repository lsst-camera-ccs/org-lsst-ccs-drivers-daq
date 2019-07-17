package org.lsst.ccs.daq.imageapi;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

/**
 * Image is a reference to an image instance, which may contain
 * multiple Sources
 * @author tonyj
 */
public class Image implements Comparable<Image> {

    private final ImageMetaData metaData;
    private final Folder folder;
    /** 
     * Non-public constructor representing an existing image
     * @param id
     * @param store 
     */

    Image(Folder folder, ImageMetaData metaData) {
        this.folder = folder;
        this.metaData = metaData;
    }
    
    public ImageMetaData getMetaData() {
        return metaData;
    }
    
    public List<Source> listSources() {
        List<Source> result = new ArrayList<>();
        List<SourceMetaData> metadata = new ArrayList<>();
        folder.getStore().listSources(metaData.getName(), folder.getName(), metadata);
        metadata.forEach((data) -> {
            result.add(new Source(this, data));
        });
        return result;       
    }
    
    public void readRaw(List<ByteBuffer> buffers) {
        Image.this.readRaw(buffers, metaData.getElements());
    }
    
    public void readRaw(List<ByteBuffer> buffers, Set<Location> elements) {
        folder.getStore().readRawImage(metaData.getName(), folder.getName(), buffers, new LocationSet(elements));
    }
    
    public void readRaw(ByteBuffer buffer, Location location) {
        readRaw(Collections.singletonList(buffer), Collections.singleton(location));
    }
    
    void close(Location location) {
        folder.getStore().closeImageChannel(metaData.getName(), folder.getName(), location);
    }

    void writeRaw(ByteBuffer buffer, Location location) {
        folder.getStore().writeRawImage(metaData.getName(), folder.getName(), location, buffer);
    }
    
    /**
     * Deletes the image, This includes deleting its:
     * <ul>
     * <li>data buckets</li>
     * <li>metadata buckets</li>
     * <li>Catalog entry</li>
     * </ul>
     * @throws DAQException 
     */
    public void delete() throws DAQException {
        folder.delete(this);
    }



    /**
     * Move the image to a different folder
     * @param folderName The folder to move to
     * @throws org.lsst.ccs.daq.imageapi.DAQException
     */
    public void moveTo(String folderName) throws DAQException {
        // TODO: Switch id to name and folder
        int rc = folder.getStore().moveImageToFolder(this.metaData.getId(), folderName);
        if (rc != 0) {
            throw new DAQException(String.format("Move image to folder %s failed (rc=%d)",folderName,rc));
        }
    }

    @Override
    public int compareTo(Image o) {
        return this.metaData.getName().compareTo(o.metaData.getName());
    }

    @Override
    public int hashCode() {
        return (int) metaData.getId();
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Image other = (Image) obj;
        return this.metaData.getId() == other.metaData.getId();
    }

}
