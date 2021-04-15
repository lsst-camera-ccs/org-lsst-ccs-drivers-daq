package org.lsst.ccs.daq.ims;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.logging.Logger;

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
        throw new UnsupportedOperationException("Not supported yet."); 
    }

    public void list(long playlist, List<ImageMetaData> result) throws DAQException {
        list(emuClient, playlist, result);
    }

    public int playlistSize(long playlist) throws DAQException {
        return playlistSize(emuClient, playlist);
    }

    public void startPlaylist(long playlist) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    void detach() throws DAQException {
        // TODO: What about playlists associated with this emulator?
        detachEmuClient(emuClient);
    }


    /* Native methods  */
    private synchronized native long attachEmuClient(String partition) throws DAQException;

    private synchronized native void detachEmuClient(long emuClient) throws DAQException;

    private synchronized native long openPlaylist(long emuClient, String playlistFile) throws DAQException;

    private synchronized native void closePlaylist(long emuClient, long playlist) throws DAQException;

    private synchronized native int playlistSize(long emuClient, long playlist) throws DAQException;

    private synchronized native void list(long emuClient, long playlist, List<ImageMetaData> result) throws DAQException;
    
    static native String decodeException(int rc);

}
