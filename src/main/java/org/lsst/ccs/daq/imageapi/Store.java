package org.lsst.ccs.daq.imageapi;

import java.util.ArrayList;
import java.util.List;
import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 *
 * @author tonyj
 */
public class Store implements AutoCloseable {

    private final Catalog catalog;
    private final String partition;
    private final long store;
    private final List<ImageListener> imageListeners = new CopyOnWriteArrayList<>();
    private final Map<Long, StreamListener> streamListenerMap = new HashMap<>();
    private Thread streamThread;

    
    static {
        System.loadLibrary("ccs_daq_ims");
    }

    /**
     * Connects to a DAQ store
     *
     * @param partition The name of the partition
     * @throws DAQException If something goes wrong
     */
    public Store(String partition) throws DAQException {
        this.partition = partition;
        catalog = new Catalog(this);
        store = attachStore(partition);
    }

    /**
     * Get the catalog associated with this store
     *
     * @return
     */
    public Catalog getCatalog() {
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

    public void addImageListener(ImageListener l) {
        imageListeners.add(l);
        synchronized (this) {
            if (streamThread == null) {
                streamThread = new Thread("ImageStreamThread_" + partition) {

                    @Override
                    public void run() {
                        for (;;) {
                            waitForImage(store);
                        }
                    }
                };
                streamThread.setDaemon(false);
                streamThread.start();
            }
        }
    }

    void imageCreatedCallback(ImageMetaData meta) {
        Image image = new Image(Store.this, meta);
        imageListeners.forEach((l) -> {
            l.imageCreated(image);
        });
    }

    void imageCompleteCallback(ImageMetaData meta) {
        Image image = new Image(Store.this, meta);
        for (Source source : image.listSources()) {
            StreamListener l = streamListenerMap.get(image.getMetaData().getId()+source.getLocation().index());
            if (l != null) {
                l.imageComplete(source.getMetaData().getLength());
            }
        }
        imageListeners.forEach((l) -> {
            l.imageComplete(image);
        });
    }

    void imageSourceStreamCallback(long imageId, int location, long length) {
        System.out.printf("Stream %,d\n",length);
        StreamListener l = streamListenerMap.get(imageId+location);
        if (l != null) {
            l.streamLength(length);
        }
    }
    
    public void removeImageListener(ImageListener l) {
        imageListeners.remove(l);
    }
    
    void addStreamListener(long imageId, int location, StreamListener listener) {
        streamListenerMap.put(imageId+location,listener); 
    }

    void removeStreamListener(long imageId, int location, StreamListener listener) {
        streamListenerMap.remove(imageId+location);    
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

    void listSources(long id, List<SourceMetaData> result) {
        listSources(store, id, result);
    }

    ImageMetaData addImageToFolder(String imageName, String folderName, ImageMetaData meta) {
        return addImageToFolder(store, imageName, folderName, meta.getAnnotation(), meta.getOpcode(), meta.getLocationBitSet());
    }

    ImageMetaData findImage(String imageName, String folderName) {
        return findImage(store, imageName, folderName);
    }

    long openSourceChannel(long id, Location location, Source.ChannelMode mode) {
        return openSourceChannel(store, id, location.index(), mode == Source.ChannelMode.WRITE);
    }

    SourceMetaData addSourceToImage(long id, Location location, int[] registerValues) {
        return addSourceToImage(store, id, location.index(), (byte) location.type().getNRebs(), "test-platform", registerValues);
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

    private synchronized native void listSources(long store, long id, List<SourceMetaData> result);

    private synchronized native ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements);

    private synchronized native ImageMetaData findImage(long store, String imageName, String folderName);

    private synchronized native long openSourceChannel(long store, long id, int index, boolean write);

    private synchronized native SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues);

    private native void waitForImage(long store);
}
