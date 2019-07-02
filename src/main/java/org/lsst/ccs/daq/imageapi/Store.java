package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author tonyj
 */
public class Store implements AutoCloseable {

    private final Catalog catalog;
    private final String partition;
    private final long store;

    static {
        System.loadLibrary("daq_ims");
        initialize();
    }

    /**
     * Connects to a DAQ store
     *
     * @param partition The name of the partition
     * @throws DAQException If something goes wrong
     */
    Store(String partition) throws DAQException {
        this.partition = partition;
        catalog = new Catalog(this);
        store = attachStore(partition);
    }

    /**
     * Get the catalog associated with this store
     *
     * @return
     */
    Catalog getCatalog() {
        return catalog;
    }

    public String getPartition() {
        return partition;
    }

    public long getCapacity() {
        return capacity(store);
    }

    public long getCurrent() {
        return remaining(store);
    }

    @Override
    public void close() throws DAQException {
        detachStore(store);
    }

    List<String> listFolders() {
        List<String> result = new ArrayList<>();
        listFolders(store, result);
        return result;
    }

    int insertFolder(String folderName) {
        return insertFolder(store, folderName);
    }

    int removeFolder(String folderName) {
        return removeFolder(store, folderName);
    }

    boolean findFolder(String folderName) {
        return findFolder(store, folderName);
    }

    void listImages(String folderName, List<ImageMetaData> result) {
        listImages(store, folderName, result);
    }

    int moveImageToFolder(long id, String folderName) {
        return moveImageToFolder(store, id, folderName);
    }

    int deleteImage(long id) {
        return deleteImage(store, id);
    }

    void listSources(String imageName, String folderName, List<SourceMetaData> result) {
        listSources(store, imageName, folderName, result);
    }
    
    // Callbacks used from C++
    
    // Native methods
    private static native void initialize();
    
    private synchronized native long attachStore(String partition);

    private synchronized native void detachStore(long store);

    private synchronized native long capacity(long store);

    private synchronized native long remaining(long store);

    private synchronized native void listFolders(long store, List<String> result);

    private synchronized native int insertFolder(long store, String folderName);

    private synchronized native int removeFolder(long store, String folderName);

    private synchronized native boolean findFolder(long store, String folderName);

    private synchronized native void listImages(long store, String folderName, List<ImageMetaData> result);

    private synchronized native int moveImageToFolder(long store, long id, String folderName);

    private synchronized native int deleteImage(long store, long id);

    private synchronized native void listSources(long store, String imageName, String folderName, List<SourceMetaData> result);
}
