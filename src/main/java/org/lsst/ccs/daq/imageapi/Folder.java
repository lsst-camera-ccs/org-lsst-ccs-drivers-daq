package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Represents a Folder in the DAQ catalog. Folders can contain images
 * but not nested folders.
 * @see Catalog#list()
 * @author tonyj
 */
public class Folder implements Comparable<Folder> {

    private final Store store;
    private final String name;

    Folder(Store store, String name) {
        this.store = store;
        this.name = name;
    }

    /**
     * Obtain a list of images in this folder. The returned list is not sorted, 
     * but implements comparable so can easily be sorted if required.
     * @return The list of folders.
     */
    public List<Image> listImages() {
        List<Image> result = new ArrayList<>();
        List<ImageMetaData> metaData = new ArrayList<>();
        store.listImages(name, metaData);
        metaData.forEach((meta) -> {
            result.add(new Image(store, meta));
        });
        return result;
    }

    /**
     * Find an image by name
     *
     * @param name The name of the image to search for
     * @return The found image.
     * @throws DAQException if the image cannot be found, or some other error occurs.
     */
    public Image find(String name) throws DAQException {
        ImageMetaData meta = store.findImage(name, this.name);
        return new Image(store, meta);
    }

    /** 
     * Create a new image in this folder. The new image will initially have no data
     * associated with it, but will be writable.
     * @see ImageMetaData#ImageMetaData(java.lang.String, java.lang.String, int, java.util.Set) 
     * @param meta The meta data corresponding to the image to be created.
     * @return The newly created image
     * @throws DAQException if the image cannot be created, or some other error occurs.
     */
    public Image insert(ImageMetaData meta) throws DAQException {
        ImageMetaData metaNew = store.addImageToFolder(meta.getName(), this.name, meta);
        return new Image(store, metaNew);
    }

    @Override
    public int compareTo(Folder o) {
        return name.compareTo(o.name);
    }

    /**
     * Get the name of the folder
     * @return The folder name
     */
    public String getName() {
        return name;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 31 * hash + Objects.hashCode(this.name);
        return hash;
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
        return Objects.equals(name, ((Folder) obj).name);
    }

    Store getStore() {
        return store;
    }
}
