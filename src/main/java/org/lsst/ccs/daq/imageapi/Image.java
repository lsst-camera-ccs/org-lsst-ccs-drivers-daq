package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;

/**
 * Image is a reference to an image instance, which may contain
 * multiple Sources
 * @author tonyj
 */
public class Image implements Comparable<Image> {

    private final ImageMetaData metaData;
    private final Store store;
    /** 
     * Non-public constructor representing an existing image
     * @param id
     * @param store 
     */

    Image(Store store, ImageMetaData metaData) {
        this.store = store;
        this.metaData = metaData;
    }
    
    public ImageMetaData getMetaData() {
        return metaData;
    }
    
    public List<Source> listSources() {
        List<Source> result = new ArrayList<>();
        List<SourceMetaData> metadata = new ArrayList<>();
        store.listSources(metaData.getId(), metadata);
        metadata.forEach((data) -> {
            result.add(new Source(this, data));
        });
        return result;       
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
        int rc = store.deleteImage(metaData.getId());
        if (rc != 0) {
            throw new DAQException(String.format("Delete image failed (rc=%d)", rc));
        }
    }



    /**
     * Move the image to a different folder
     * @param folderName The folder to move to
     * @throws org.lsst.ccs.daq.imageapi.DAQException
     */
    public void moveTo(String folderName) throws DAQException {
        int rc = store.moveImageToFolder(this.metaData.getId(), folderName);
        if (rc != 0) {
            throw new DAQException(String.format("Move image to folder %s failed (rc=%d)",folderName,rc));
        }
    }

    public Source addSource(Location location, int[] registerValues) {
        if (!metaData.getLocations().contains(location)) {
            throw new IllegalArgumentException("Invalid location "+location+" for image "+this);
        }
        return new Source(this, store.addSourceToImage(metaData.getId(), location, registerValues));
    }
    
    Store getStore() {
        return store;
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

    @Override
    public String toString() {
        return "Image{" + "metaData=" + metaData + '}';
    }
}
