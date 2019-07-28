package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;

/**
 * A catalog contains a list of folders associated with a store. 
 * The catalog can be obtained from the associated Store.
 * @see Store#getCatalog() 
 *
 * @author tonyj
 */
public class Catalog {

    private final Store store;

    Catalog(Store store) {
        this.store = store;
    }

    /**
     * Find the specified folder by name.
     *
     * @param folderName The folder name to search for
     * @return The folder, or <code>null</code> if the folder does not exist
     * @throws DAQException If some other error occurs
     */
    public Folder find(String folderName) throws DAQException {
        boolean ok = store.findFolder(folderName);
        if (ok) {
            return new Folder(store, folderName);
        } else {
            return null;
        }
    }

    /**
     * Return a list of all available folders. The list is not sorted, but Folder
     * implements Comparable so it can be readily sorted if desired.
     * @return The list of folders
     */
    public List<Folder> list() {
        List<Folder> result = new ArrayList<>();
        List<String> folderNames = store.listFolders();
        folderNames.forEach((name) -> {
            result.add(new Folder(store, name));
        });
        return result;
    }

    /** 
     * Insert a new folder into the catalog.
     * @param folderName The name of the new folder to create
     * @return The newly created folder
     * @throws Catalog.CatalogException If the folder cannot be created.
     */
    public Folder insert(String folderName) throws CatalogException {
        int rc = store.insertFolder(folderName);
        if (rc != 0) {
            throw new CatalogException("Error creating folder", folderName, rc);
        }
        return new Folder(store, folderName);
    }

    /**
     * Remove a folder.
     * @param folderName The name of the folder to be removed
     * @throws Catalog.CatalogException If the folder cannot be removed.
     */
    public void remove(String folderName) throws CatalogException {
        int rc = store.removeFolder(folderName);
        if (rc != 0) {
            throw new CatalogException("Error creating folder", folderName, rc);
        }
    }

    /**
     * An exception throw when errors occur when operating on the catalog.
     */
    public static class CatalogException extends DAQException {

        CatalogException(String message, String folderName, int rc) {
            super(String.format("%s: %s (rc=%d)", message, folderName, rc));
        }
    }

}
