package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.util.ArrayList;
import java.util.List;
import java.util.BitSet;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.utilities.location.Location.LocationType;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A connection to the store within a specified DAQ partition. A store is the
 * top level object from which all image functionality can be accessed.
 *
 * @author tonyj
 */
public class Store implements AutoCloseable {

    private final Camera camera;
    private final Catalog catalog;
    private final String partition;
    private final long store;
    private final List<ImageListener> imageListeners = new CopyOnWriteArrayList<>();
    private final Map<Long, Map<Integer, List<StreamListener>>> imageStreamMap = new ConcurrentHashMap<>();
    private Thread imageThread;
    private static final Logger LOG = Logger.getLogger(Store.class.getName());

    private static final int IMAGE_TIMEOUT_MICROS = 0;
    private static final int SOURCE_TIMEOUT_MICROS = 10_000_000;

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
        camera = new Camera(this);
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
     * Gets the camera associated with this store. The camera can be used to
     * trigger images.
     *
     * @return The camera associated with this store.
     */
    public Camera getCamera() {
        return camera;
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
     * @throws DAQException If unable to access to DAQ store
     */
    public long getCapacity() throws DAQException {
        return capacity(store);
    }

    /**
     * The storage capacity remaining (unused) in bytes.
     *
     * @return Bytes remaining unused.
     * @throws DAQException If unable to access the DAQ store
     */
    public long getRemaining() throws DAQException {
        return remaining(store);
    }

    /**
     * Get the set of configured locations in this partition
     *
     * @return The set of locations configured in this partition
     * @throws DAQException
     */
    public LocationSet getConfiguredSources() throws DAQException {
        BitSet locations = getConfiguredSources(store);
        return new LocationSet(locations);
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
                    @SuppressWarnings("UseSpecificCatch")
                    public void run() {
                        try {
                            for (;;) {
                                int rc = waitForImage(store, IMAGE_TIMEOUT_MICROS, SOURCE_TIMEOUT_MICROS);
                                if (rc != 0 && rc != 68) { // 68 appears to mean timeout
                                    LOG.log(Level.SEVERE, "Unexpected rc from waitForImage: {0}", rc);
                                }
                            }
                        } catch (Throwable x) {
                            LOG.log(Level.SEVERE, "ImageStreamThread exiting", x);
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
        LOG.log(Level.INFO, "Image created: {0}", image);
        synchronized (imageStreamMap) {
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
        LOG.log(Level.INFO, "Image complete: {0}", image);
        Map<Integer, List<StreamListener>> streamListeners;
        synchronized (imageStreamMap) {
            streamListeners = imageStreamMap.remove(image.getMetaData().getId());
        }
        streamListeners.forEach((locationIndex, listeners) -> {
            try {
                SourceMetaData sourceMeta = findSource(meta.getId(), locationIndex);
                listeners.forEach((l) -> {
                    l.imageComplete(sourceMeta.getLength());
                });
            } catch (DAQException ex) {
                LOG.log(Level.SEVERE, "Exception during image complete callback", ex);
            }
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
        LOG.log(Level.FINE, "Image stream: imageId={0} location={1} length={2}", new Object[]{imageId, location, length});
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
            try {
                // This indicates no streaming is currently happening for this image, we need to report
                // the final length immediately to avoid hang.
                SourceMetaData sourceMeta = findSource(imageId, location);
                listener.imageComplete(sourceMeta.getLength());
            } catch (DAQException ex) {
                LOG.log(Level.SEVERE, "Unexpected exception while adding stream listener", ex);
            }
        } else {
            List<StreamListener> listeners = streamListeners.get(location);
            if (listeners == null) {
                listeners = new CopyOnWriteArrayList<>();
                streamListeners.put(location, listeners);
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

    List<String> listFolders() throws DAQException {
        List<String> result = new ArrayList<>();
        listFolders(store, result);
        return result;
    }

    int insertFolder(String folderName) throws DAQException {
        return insertFolder(store, folderName);
    }

    int removeFolder(String folderName) throws DAQException {
        return removeFolder(store, folderName);
    }

    boolean findFolder(String folderName) throws DAQException {
        return findFolder(store, folderName);
    }

    void listImages(String folderName, List<ImageMetaData> result) throws DAQException {
        listImages(store, folderName, result);
    }

    int moveImageToFolder(long id, String folderName) throws DAQException {
        return moveImageToFolder(store, id, folderName);
    }

    int deleteImage(long id) throws DAQException {
        return deleteImage(store, id);
    }

    SourceMetaData findSource(long id, int location) throws DAQException {
        return findSource(store, id, location);
    }

    ImageMetaData addImageToFolder(String imageName, String folderName, ImageMetaData meta) throws DAQException {
        return addImageToFolder(store, imageName, folderName, meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    ImageMetaData findImage(String imageName, String folderName) throws DAQException {
        return findImage(store, imageName, folderName);
    }

    long openSourceChannel(long id, Location location, Source.ChannelMode mode) throws DAQException {
        return openSourceChannel(store, id, location.index(), mode == Source.ChannelMode.WRITE);
    }

    SourceMetaData addSourceToImage(long id, Location location, int[] registerValues) throws DAQException {
        return addSourceToImage(store, id, location.index(), (byte) location.type().getCCDCount(), "test-platform", registerValues);
    }

    // methods based on those originally in GlobalProc
    void setRegisterList(LocationType rebType, int[] registerAddresses) throws DAQException {
        setRegisterList(store, rebType == LocationType.SCIENCE, rebType == LocationType.GUIDER, registerAddresses);
    }

    ImageMetaData triggerImage(ImageMetaData meta) throws DAQException {
        return triggerImage(store, meta.getCreationFolderName(), meta.getName(), meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    long startSequencer(int opcode) throws DAQException {
        return startSequencer(store, opcode);
    }

    // Native methods    
    private synchronized native long attachStore(String partition) throws DAQException;

    private synchronized native void detachStore(long store) throws DAQException;

    private synchronized native long capacity(long store) throws DAQException;

    private synchronized native long remaining(long store) throws DAQException;

    private synchronized native void listFolders(long store, List<String> result) throws DAQException;

    private synchronized native int insertFolder(long store, String folderName) throws DAQException;

    private synchronized native int removeFolder(long store, String folderName) throws DAQException;

    private synchronized native boolean findFolder(long store, String folderName) throws DAQException;

    private synchronized native void listImages(long store, String folderName, List<ImageMetaData> result) throws DAQException;

    private synchronized native int moveImageToFolder(long store, long id, String folderName) throws DAQException;

    private synchronized native int deleteImage(long store, long id) throws DAQException;

    private synchronized native SourceMetaData findSource(long store, long id, int location) throws DAQException;

    private synchronized native ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException;

    private synchronized native ImageMetaData findImage(long store, String imageName, String folderName) throws DAQException;

    private synchronized native long openSourceChannel(long store, long id, int index, boolean write) throws DAQException;

    private synchronized native SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException;

    private native int waitForImage(long store, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException;

    static native String decodeException(int rc);

    private synchronized native BitSet getConfiguredSources(long store) throws DAQException;

    public static native Version getClientVersion() throws DAQException;

    private synchronized native void setRegisterList(long store, boolean science, boolean guider, int[] registerAddresses) throws DAQException;

    private synchronized native ImageMetaData triggerImage(long store, String metaData, String name, String annotation, int opcode, BitSet locationBitSet) throws DAQException;

    private synchronized native long startSequencer(long store, int opcode) throws DAQException;
}
