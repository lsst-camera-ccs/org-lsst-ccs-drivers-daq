package org.lsst.ccs.daq.imageapi;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;

/**
 * A channel for reading or writing DAQ raw data
 *
 * @author tonyj
 */
public class DAQSourceChannel implements ByteChannel {

    public enum Mode {
        READ, WRITE
    };
    private final Mode mode;
    private final Store store;
    private long source_;
    private final int length;
    private int offset = 0;

    DAQSourceChannel(Source source, Mode mode) {
        this.mode = mode;
        final Image image = source.getImage();
        final Folder folder = image.getFolder();
        length = source.getMetaData().getLength();
        store = folder.getStore();
        source_ = store.openSourceChannel(image.getMetaData().getName(), folder.getName(), source.getLocation(), mode);
    }

    @Override
    public int read(ByteBuffer dst) throws IOException {
        if (source_ == 0 || mode != Mode.READ) {
            throw new IOException("Channel not open for read");
        }        
        if (!dst.isDirect()) {
            throw new IOException("Supplied ByteBuffer is not direct");
        }
        int l = Math.min(dst.remaining(), length-offset);
        if (l==0) return -1;
        int position = dst.position();
        int err;
        synchronized (store) {
            err = read(source_, dst, position, offset, l);
        }
        if (err != 0) {
            throw new IOException(String.format("Error reading DAQ data (err=%d)", err));
        }
        offset += l;
        dst.position(position+l);
        return l;
    }

    @Override
    public boolean isOpen() {
        return source_ != 0;
    }

    @Override
    public void close() throws IOException {
        if (source_ != 0) {
            synchronized (store) {
                close(source_, mode == Mode.WRITE);
            }
            source_ = 0;
        }
    }

    @Override
    public int write(ByteBuffer src) throws IOException {
        if (source_ == 0 || mode != Mode.WRITE) {
            throw new IOException("Channel not open for write");
        }
        if (!src.isDirect()) {
            throw new IOException("Supplied ByteBuffer is not direct");
        }
        int remaining = src.remaining();
        int position = src.position();
        int err;
        synchronized (store) {
            err = write(source_, src, position, remaining);
        }
        if (err != 0) {
            throw new IOException(String.format("Error writing DAQ data (err=%d)", err));
        }  
        src.position(position+remaining);
        return remaining;
    }

    private native int read(long source_, ByteBuffer dst, int position, int offset, int length);

    private native int write(long source_, ByteBuffer src, int position, int remaining);

    private native void close(long source_, boolean write);
}
