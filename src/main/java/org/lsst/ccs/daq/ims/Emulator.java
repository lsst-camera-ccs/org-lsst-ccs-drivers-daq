package org.lsst.ccs.daq.ims;

import java.io.File;
import java.io.IOException;
import java.util.BitSet;
import java.util.List;
import java.util.logging.Logger;

import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A connection to the DAQ emulator for managing playlists.
 *
 * @author tony Johnson
 */
public class Emulator {

    private static final Logger LOG = Logger.getLogger(Emulator.class.getName());

    static {
        System.loadLibrary("ccs_daq_ims");
    }
    private final long emuClient;
    private final Store store;

    /**
     * Connect to DAQ Statistics clients
     *
     * @param store
     * @throws DAQException If the partition does not exist, or something else
     * goes wrong
     */
    Emulator(Store store) throws DAQException {
        this.store = store;
        emuClient = attachEmuClient(store.getPartition());
    }

    public Playlist openPlaylist(File file) throws DAQException, IOException {
        long playlist = openPlaylist(emuClient, file.getCanonicalPath());
        return new Playlist(store, this, playlist);
    }

    public void closePlayList(long playlist) throws DAQException {
        closePlaylist(emuClient, playlist);
    }

    public void addImageToPlaylist(long playlist, Image image) throws DAQException {
        addImageToPlaylist(emuClient, store.getStore(), playlist, image.getMetaData().getId());
    }

    public void list(long playlist, List<ImageMetaData> result) throws DAQException {
        list(emuClient, store.getStore(), playlist, result);
    }

    public int playlistSize(long playlist) throws DAQException {
        return playlistSize(emuClient, playlist);
    }

    public void startPlaylist(long playlist, boolean repeat) throws DAQException {
        startPlaylist(emuClient, playlist, repeat);
    }

    public LocationSet getLocations() {
        BitSet locations = getLocations(emuClient);
        return new LocationSet(locations);
    }

    public void halt() throws DAQException {
        halt(emuClient);
    }

    public void stop() throws DAQException {
        stop(emuClient);
    }

    public void restart() throws DAQException {
        restart(emuClient);
    }

    void detach() throws DAQException {
        // TODO: What about playlists associated with this emulator?
        detachEmuClient(emuClient);
    }

    /* Native methods */
    private synchronized native long attachEmuClient(String partition) throws DAQException;

    private synchronized native void detachEmuClient(long emuClient) throws DAQException;

    private synchronized native long openPlaylist(long emuClient, String playlistFile) throws DAQException;

    private synchronized native void closePlaylist(long emuClient, long playlist) throws DAQException;

    private synchronized native int playlistSize(long emuClient, long playlist) throws DAQException;

    private synchronized native void list(long emuClient, long store, long playlist, List<ImageMetaData> result)
            throws DAQException;

    private synchronized native void addImageToPlaylist(long emuClient, long store, long playlist, long id)
            throws DAQException;

    private synchronized native void startPlaylist(long emuClient, long playlist, boolean repeat) throws DAQException;

    private synchronized native BitSet getLocations(long emuClient);

    private synchronized native void halt(long emuClient);

    private synchronized native void stop(long emuClient);

    private synchronized native void restart(long emuClient);

    static native String decodeException(int rc);

}
