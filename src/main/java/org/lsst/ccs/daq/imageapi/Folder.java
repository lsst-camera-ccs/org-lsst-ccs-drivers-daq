package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * @author tonyj
 */
public class Folder implements Comparable<Folder> {

    private final Store store;
    private final String name;

    Folder(Store store, String name) {
        this.store = store;
        this.name = name;
    }

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
     * @param name
     * @return
     * @throws org.lsst.ccs.daq.imageapi.DAQException
     */
    public Image find(String name) throws DAQException {
        ImageMetaData meta = store.findImage(name, this.name);
        return new Image(store, meta);
    }

    public Image insert(ImageMetaData meta) {
        ImageMetaData metaNew = store.addImageToFolder(meta.getName(), this.name, meta);
        return new Image(store, metaNew);
    }

    @Override
    public int compareTo(Folder o) {
        return name.compareTo(o.name);
    }

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
