package org.lsst.ccs.daq.ims;

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
        try { 
            store.findFolder(folderName);
            return new Folder(store, folderName);
        } catch (DAQException x) {
            if (x.rc() == 1) return null;
            else throw x;
        }
    }

    /**
     * Return a list of all available folders.The list is not sorted, but Folder
     * implements Comparable so it can be readily sorted if desired.
     * @return The list of folders
     * @throws DAQException
     */
    public List<Folder> list() throws DAQException {
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
     * @throws DAQException If something else goes wrong
     */
    public Folder insert(String folderName) throws CatalogException, DAQException {
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
     * @throws DAQException If something else goes wrong
     */
    public void remove(String folderName) throws CatalogException, DAQException {
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
            super(String.format("%s: %s", message, folderName), rc);
        }
    }

}
