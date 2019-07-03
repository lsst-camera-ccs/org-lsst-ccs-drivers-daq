package org.lsst.ccs.daq.imageapi;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

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
    
    List<Source> listSources() {
        List<Source> result = new ArrayList<>();
        List<SourceMetaData> metadata = new ArrayList<>();
        folder.getStore().listSources(metaData.getName(), folder.getName(), metadata);
        metadata.forEach((data) -> {
            result.add(new Source(this, data));
        });
        return result;       
    }
    
    void readRaw(List<ByteBuffer> buffers) {
        Image.this.readRaw(buffers, metaData.getElements());
    }
    
    void readRaw(List<ByteBuffer> buffers, LocationSet elements) {
        folder.getStore().readRawImage(metaData.getName(), folder.getName(), buffers, elements);
    }
    
    void readRaw(ByteBuffer buffer, Location location) {
        readRaw(Collections.singletonList(buffer), LocationSet.singleton(location));
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
        // TODO: Switch id to name and folder
        int rc = folder.getStore().deleteImage(this.metaData.getId());
        if (rc != 0) {
            throw new DAQException(String.format("Delete image failed (rc=%d)",rc));
        }
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
