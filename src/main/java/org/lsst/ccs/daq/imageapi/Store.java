package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;
import java.util.BitSet;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * A connection to the store within a specified DAQ partition. A store is the
 * top level object from which all image functionality can be accessed.
 *
 * @author tonyj
 */
public class Store implements AutoCloseable {

    private final Catalog catalog;
    private final String partition;
    private final long store;
    private final List<ImageListener> imageListeners = new CopyOnWriteArrayList<>();
    private final Map<Long, Map<Integer, List<StreamListener>>> imageStreamMap = new ConcurrentHashMap<>();
    private Thread imageThread;

    static {
        System.loadLibrary("ccs_daq_ims");
    }

    /**
     * Connects to a DAQ store.
     *
     * @param partition The name of the partition
     * @throws DAQException If the partition does not exist, or something else
     * goes wrong
     */
    public Store(String partition) throws DAQException {
        this.partition = partition;
        catalog = new Catalog(this);
        store = attachStore(partition);
    }

    /**
     * Get the catalog associated with this store. The catalog can be used to
     * access folders.
     *
     * @return The catalog associated with this store.
     */
    public Catalog getCatalog() {
        return catalog;
    }

    /**
     * The name of the associated DAQ partition.
     *
     * @return The partition name
     */
    public String getPartition() {
        return partition;
    }

    /**
     * The total capacity in bytes of this store.
     *
     * @return The capacity
     */
    public long getCapacity() {
        return capacity(store);
    }

    /**
     * The storage capacity remaining (unused) in bytes.
     *
     * @return Bytes remaining unused.
     */
    public long getRemaining() {
        return remaining(store);
    }

    /**
     * Add an image listener to this store. The image listener will be notified
     * of all new images created, in all folders.
     *
     * @param l The image listener to add.
     */
    public void addImageListener(ImageListener l) {
        imageListeners.add(l);
        synchronized (this) {
            if (imageThread == null) {
                // TODO: We should stop this thread when no one is left listening
                imageThread = new Thread("ImageStreamThread_" + partition) {

                    @Override
                    public void run() {
                        for (;;) {
                            waitForImage(store);
                        }
                    }
                };
                imageThread.setDaemon(false);
                imageThread.start();
            }
        }
    }

    /**
     * Remove an image listener. The method will perform no action if the
     * listener has not been previously added.
     *
     * @param l The image listener to remove.
     */
    public void removeImageListener(ImageListener l) {
        imageListeners.remove(l);
    }

    /**
     * Called from C++ code to notify of image creation
     *
     * @param meta The meta-data for the image.
     */
    void imageCreatedCallback(ImageMetaData meta) {
        Image image = new Image(Store.this, meta);
        synchronized(imageStreamMap) {
            imageStreamMap.put(image.getMetaData().getId(), new ConcurrentHashMap<>());
        }
        imageListeners.forEach((l) -> {
            l.imageCreated(image);
        });
    }

    /**
     * Called from C++ code to notify of image completion
     *
     * @param meta The meta-data for the image.
     */
    void imageCompleteCallback(ImageMetaData meta) {
        Image image = new Image(Store.this, meta);
        Map<Integer, List<StreamListener>> streamListeners;
        synchronized (imageStreamMap) {
            streamListeners = imageStreamMap.remove(image.getMetaData().getId());
        }
        streamListeners.forEach((locationIndex, listeners) -> {
            SourceMetaData sourceMeta = findSource(meta.getId(), locationIndex);
            listeners.forEach((l) -> {
                l.imageComplete(sourceMeta.getLength());
            });
        });
        imageListeners.forEach((l) -> {
            l.imageComplete(image);
        });
    }

    /**
     * Called from C++ code to notify of streaming data
     *
     * @param imageId The imageId for which data is reported
     * @param location The source location index for which data is reported
     * @param length The length of data currently available to be read from the
     * source
     */
    void imageSourceStreamCallback(long imageId, int location, long length) {
        Map<Integer, List<StreamListener>> streamListeners = imageStreamMap.get(imageId);
        if (streamListeners != null) {
            List<StreamListener> listeners = streamListeners.get(location);
            if (listeners != null) {
                listeners.forEach((l) -> {
                    l.streamLength(length);
                });
            }
        }
    }

    void addStreamListener(long imageId, int location, StreamListener listener) {
        Map<Integer, List<StreamListener>> streamListeners;
        synchronized (imageStreamMap) {
            streamListeners = imageStreamMap.get(imageId);
        }
        if (streamListeners == null) {
            // This indicates no streaming is currently happening for this image, we need to report 
            // the final length immediately to avoid hang.
            SourceMetaData sourceMeta = findSource(imageId, location);
            listener.imageComplete(sourceMeta.getLength());
        } else {
            List<StreamListener> listeners = streamListeners.get(location);
            if (listeners == null) {
                listeners = new CopyOnWriteArrayList<>();
                streamListeners.put(location,listeners);
            }
            listeners.add(listener);
        }
    }

    void removeStreamListener(long imageId, int location, StreamListener listener) {
        Map<Integer, List<StreamListener>> streamListeners = imageStreamMap.get(imageId);
        if (streamListeners != null) {
            List<StreamListener> listeners = streamListeners.get(location);
            if (listeners != null) {
                listeners.remove(listener);
            }
        }
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

    SourceMetaData findSource(long id, int location) {
        return findSource(store, id, location);
    }

    ImageMetaData addImageToFolder(String imageName, String folderName, ImageMetaData meta) throws DAQException {
        return addImageToFolder(store, imageName, folderName, meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    ImageMetaData findImage(String imageName, String folderName) {
        return findImage(store, imageName, folderName);
    }

    long openSourceChannel(long id, Location location, Source.ChannelMode mode) {
        return openSourceChannel(store, id, location.index(), mode == Source.ChannelMode.WRITE);
    }

    SourceMetaData addSourceToImage(long id, Location location, int[] registerValues) {
        return addSourceToImage(store, id, location.index(), (byte) location.type().getCCDCount(), "test-platform", registerValues);
    }

    // Native methods    
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

    private synchronized native SourceMetaData findSource(long store, long id, int location);

    private synchronized native ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException;

    private synchronized native ImageMetaData findImage(long store, String imageName, String folderName);

    private synchronized native long openSourceChannel(long store, long id, int index, boolean write);

    private synchronized native SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues);

    private native void waitForImage(long store);
}
