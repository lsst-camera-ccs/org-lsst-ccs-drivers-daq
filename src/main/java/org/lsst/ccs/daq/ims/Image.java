package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.util.ArrayList;
import java.util.List;

/**
 * Image is a reference to an image instance, which may contain
 * multiple Sources. Existing images can be obtained from folders, or
 * by listening for newly created images. New writable images can be
 * created within a folder. Each source within the image
 * corresponds to one REB.
 * @see Folder#listImages()
 * @see Folder#find(java.lang.String)
 * @see Folder#insert(ImageMetaData) 
 * @see Store#addImageListener(ImageListener) 
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
    
    /**
     * Get the metadata associated with this image
     * @return The corresponding metadata
     */
    public ImageMetaData getMetaData() {
        return metaData;
    }
    
    /**
     * List the sources associated with this image.Each source represents the 
     * data obtained from a single REB.
     * @return The list of sources.
     * @throws org.lsst.ccs.daq.ims.DAQException
     */
    public List<Source> listSources() throws DAQException {
        List<Source> result = new ArrayList<>();
        for (Location location : metaData.getLocations()) {
            result.add(new Source(this, location));
        }
        return result;       
    }
           
    /**
     * Deletes the image, This includes deleting its:
     * <ul>
     * <li>data buckets</li>
     * <li>metadata buckets</li>
     * <li>Catalog entry</li>
     * </ul>
     * @throws DAQException If something goes wrong.
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
     * @throws DAQException If the image move fails
     */
    public void moveTo(String folderName) throws DAQException {
        int rc = store.moveImageToFolder(this.metaData.getId(), folderName);
        if (rc != 0) {
            throw new DAQException(String.format("Move image to folder %s failed (rc=%d)",folderName,rc));
        }
    }

    /**
     * Adds a new source to an image.This method will only succeed if the image is writable
     * @param location The location corresponding to the source to be added.
     * @param registerValues The register values (meta-data) to be associated with the new source.
     * @return The created source
     * @throws DAQException
     * @see Folder#insert(ImageMetaData) 
     */
    public Source addSource(Location location, int[] registerValues) throws DAQException {
        if (!metaData.getLocations().contains(location)) {
            throw new IllegalArgumentException("Invalid location "+location+" for image "+this);
        }
        return new Source(this, store.addSourceToImage(metaData.getId(), location, registerValues));
    }
    
    /**
     * Find the source corresponding to the given location
     * @param location The location to search for
     * @return The corresponding Source
     * @throws IllegalArgumentException If the specified source does not exist in this image.
     */
    public Source findSource(Location location) {
        if (!metaData.getLocations().contains(location)) {
            throw new IllegalArgumentException("Invalid location "+location+" for image "+this);
        }
        return new Source(this, location);
    }
    
    /**
     * Get the store in which this image resides.
     * @return The store.
     */
    public Store getStore() {
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
