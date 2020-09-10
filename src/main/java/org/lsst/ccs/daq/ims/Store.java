package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.util.ArrayList;
import java.util.List;
import java.util.BitSet;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.Future;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A connection to the store within a specified DAQ partition. A store is the
 * top level object from which all image functionality can be accessed.
 *
 * @author tonyj
 */
public class Store implements AutoCloseable {

    private Camera camera;
    private RegisterClient client;
    private final Catalog catalog;
    private final String partition;
    private final long store;
    private final List<ImageListener> imageListeners = new CopyOnWriteArrayList<>();
    private final Map<Long, Map<Integer, List<StreamListener>>> imageStreamMap = new ConcurrentHashMap<>();
    private static final Logger LOG = Logger.getLogger(Store.class.getName());

    private static final int IMAGE_TIMEOUT_MICROS = 0;
    private static final int SOURCE_TIMEOUT_MICROS = 10_000_000;
    private final static StoreImplementation impl;
    private final ExecutorService executor;

    static {
        // FIXME: This requires a System (not bootstrap) property be set.
        String runMode = System.getProperty("org.lsst.ccs.run.mode");
        if (runMode != null) {
            LOG.log(Level.INFO, "runMode={0}", runMode);
        }
        impl = "simulation".equals(runMode)
               ? new StoreSimulatedImplementation() : new StoreNativeImplementation();
    }
    private Future<?> waitForImageTask;

    /**
     * Connects to a DAQ store. Uses the default executor for polling thread.
     *
     * @param partition The name of the partition
     * @throws DAQException If the partition does not exist, or something else
     * goes wrong
     */
    public Store(String partition) throws DAQException {
        this(partition, ForkJoinPool.commonPool());
    }

    /**
     * Connects to a DAQ store using the specified executor for polling.
     *
     * @param partition The name of the partition
     * @param executor The executor service to use for polling thread
     * @throws DAQException If the partition does not exist, or something else
     * goes wrong
     */
    public Store(String partition, ExecutorService executor) throws DAQException {
        this.partition = partition;
        this.executor = executor;
        catalog = new Catalog(this);
        store = impl.attachStore(partition);
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
     * Gets the camera associated with this store.The camera can be used to trigger images.
     *
     * @return The camera associated with this store.
     * @throws org.lsst.ccs.daq.ims.DAQException
     */
    public Camera getCamera() throws DAQException {
        synchronized (this) {
            if (camera == null) {
                long camera_ = impl.attachCamera(store);
                this.camera = new Camera(this, camera_);
            }
            return camera;
        }
    }

        /**
     * Gets the camera associated with this store.The camera can be used to trigger images.
     *
     * @return The camera associated with this store.
     * @throws org.lsst.ccs.daq.ims.DAQException
     */
    public RegisterClient getRegisterClient() throws DAQException {
        synchronized (this) {
            if (client == null) {
                long client_ = impl.attachClient(getPartition());
                this.client = new RegisterClient(this, client_);
            }
            return client;
        }
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
        return impl.capacity(store);
    }

    /**
     * The storage capacity remaining (unused) in bytes.
     *
     * @return Bytes remaining unused.
     * @throws DAQException If unable to access the DAQ store
     */
    public long getRemaining() throws DAQException {
        return impl.remaining(store);
    }

    /**
     * Get the set of configured locations in this partition
     *
     * @return The set of locations configured in this partition
     * @throws DAQException
     */
    public LocationSet getConfiguredSources() throws DAQException {
        BitSet locations = impl.getConfiguredSources(store);
        return new LocationSet(locations);
    }

    /**
     * Add an image listener to this store. The image listener will be notified
     * of all new images created, in all folders.
     *
     * @param l The image listener to add.
     */
    @SuppressWarnings("UseSpecificCatch")
    public void addImageListener(ImageListener l) {
        imageListeners.add(l);
        synchronized (this) {
            if (waitForImageTask == null || waitForImageTask.isCancelled() || waitForImageTask.isDone()) {
                waitForImageTask = executor.submit(() -> {
                    try {
                        Thread.currentThread().setName("ImageStreamThread_" + partition);
                        long waitForImageStore = impl.attachStore(partition);
                        try {
                            while (!Thread.currentThread().isInterrupted()) {
                                int rc = impl.waitForImage(Store.this, waitForImageStore, IMAGE_TIMEOUT_MICROS, SOURCE_TIMEOUT_MICROS);
                                if (rc != 0 && rc != 68) { // 68 appears to mean timeout
                                    LOG.log(Level.SEVERE, "Unexpected rc from waitForImage: {0}", rc);
                                }
                            }
                        } finally {
                            impl.detachStore(store);
                        }
                    } catch (Throwable x) {
                        LOG.log(Level.SEVERE, x, () -> String.format("Thread %s exiting", Thread.currentThread().getName()));
                    }
                });

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
        synchronized (this) {
            if (imageListeners.remove(l) && imageListeners.isEmpty()) {
                waitForImageTask.cancel(true);
            }
        }
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
        if (waitForImageTask != null && !waitForImageTask.isDone()) {
            waitForImageTask.cancel(true);
        }
        if (camera != null) {
            camera.detach();
        }
        if (client != null) {
            client.detach();
        }
        impl.detachStore(store);
    }

    List<String> listFolders() throws DAQException {
        List<String> result = new ArrayList<>();
        impl.listFolders(store, result);
        return result;
    }

    int insertFolder(String folderName) throws DAQException {
        return impl.insertFolder(store, folderName);
    }

    int removeFolder(String folderName) throws DAQException {
        return impl.removeFolder(store, folderName);
    }

    boolean findFolder(String folderName) throws DAQException {
        return impl.findFolder(store, folderName);
    }

    void listImages(String folderName, List<ImageMetaData> result) throws DAQException {
        impl.listImages(store, folderName, result);
    }

    int moveImageToFolder(long id, String folderName) throws DAQException {
        return impl.moveImageToFolder(store, id, folderName);
    }

    int deleteImage(long id) throws DAQException {
        return impl.deleteImage(store, id);
    }

    SourceMetaData findSource(long id, int location) throws DAQException {
        return impl.findSource(store, id, location);
    }

    ImageMetaData addImageToFolder(String imageName, String folderName, ImageMetaData meta) throws DAQException {
        return impl.addImageToFolder(store, imageName, folderName, meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    ImageMetaData findImage(String imageName, String folderName) throws DAQException {
        return impl.findImage(store, imageName, folderName);
    }

    long openSourceChannel(long id, Location location, Source.ChannelMode mode) throws DAQException {
        return impl.openSourceChannel(store, id, location.index(), mode == Source.ChannelMode.WRITE);
    }

    SourceMetaData addSourceToImage(long id, Location location, int[] registerValues) throws DAQException {
        return impl.addSourceToImage(store, id, location.index(), (byte) location.type().getCCDCount(), "test-platform", registerValues);
    }

    ImageMetaData triggerImage(long camera, ImageMetaData meta) throws DAQException {
        return impl.triggerImage(store, camera, meta);
    }

    long startSequencer(long camera, int opcode) throws DAQException {
        return impl.startSequencer(camera, opcode);
    }

    public static Version getClientVersion() throws DAQException {
        return impl.getClientVersion();
    }

    static String decodeException(int rc) {
        return impl.decodeException(rc);
    }

    void detachCamera(long camera) throws DAQException {
        impl.detachCamera(camera);
    }

    void setRegisterList(long camera, Location.LocationType rebType, int[] registerAddresses) throws DAQException {
        impl.setRegisterList(store, camera, rebType, registerAddresses);
    }

    void detachClient(long client) throws DAQException {
        impl.detachClient(client);
    }

    int[] readRegisters(long client, BitSet locations, int address) throws DAQException {
        return impl.readRegisters(client, locations, address);
    }
}
