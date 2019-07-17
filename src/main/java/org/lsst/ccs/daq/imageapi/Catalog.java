package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;

/**
 * A catalog contains a list of top level folders. The catalog can be obtained
 * from the associated Store.
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
     * @return The folder, or <code>null<</code> if the folder does not exist
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
     * Return a list of all available folders
     *
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

    public Folder insert(String folderName) throws CatalogException {
        int rc = store.insertFolder(folderName);
        if (rc != 0) {
            throw new CatalogException("Error creating folder", folderName, rc);
        }
        return new Folder(store, folderName);
    }

    public void remove(String folderName) throws CatalogException {
        int rc = store.removeFolder(folderName);
        if (rc != 0) {
            throw new CatalogException("Error creating folder", folderName, rc);
        }
    }

    private static class CatalogException extends DAQException {

        public CatalogException(String message, String folderName, int rc) {
            super(String.format("%s: %s (rc=%d)", message, folderName, rc));
        }
    }

}
