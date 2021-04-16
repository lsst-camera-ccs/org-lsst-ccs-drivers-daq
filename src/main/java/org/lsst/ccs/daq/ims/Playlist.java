package org.lsst.ccs.daq.ims;

import java.util.ArrayList;
import java.util.List;

/**
 * Public interface to a DAQ playlist
 *
 * @author tonyj
 */
public class Playlist implements AutoCloseable {

    private final long playlist;
    private final Emulator emulator;
    private final Store store;

    Playlist(Store store, Emulator emulator, long playlist) {
        this.store = store;
        this.emulator = emulator;
        this.playlist = playlist;
    }

    public void add(Image image) throws DAQException {
        emulator.addImageToPlaylist(playlist, image);
    }

    public List<Image> getImages() throws DAQException {
        List<Image> result = new ArrayList<>();
        List<ImageMetaData> metaData = new ArrayList<>();
        emulator.list(playlist, metaData);
        metaData.forEach((meta) -> {
            result.add(new Image(store, meta));
        });
        return result;
    }

    public int size() throws DAQException {
        return emulator.playlistSize(playlist);
    }

    public void start(boolean repeat) throws DAQException {
        emulator.startPlaylist(playlist, repeat);
    }

    @Override
    public void close() throws DAQException {
        emulator.closePlayList(playlist);
    }

}
