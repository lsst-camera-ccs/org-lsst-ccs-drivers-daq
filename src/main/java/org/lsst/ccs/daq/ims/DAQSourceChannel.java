package org.lsst.ccs.daq.ims;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.time.Duration;

/**
 * A channel for reading or writing DAQ raw data
 *
 * @author tonyj
 */
class DAQSourceChannel implements ByteChannel {

    static ByteChannel open(Store store, Source source, Source.ChannelMode mode) throws DAQException {
        switch (mode) {
            case READ:
                if (source.getMetaData().getLength() == 0) {
                    throw new DAQException("Cannot read from source which is not fully written, did you mean to use ChannelMode.STREAM?");
                }
                return new ReadableDAQSourceChannel(store, source);
            case STREAM:
                return new StreamingDAQSourceChannel(store, source);
            case WRITE:
                return new WritableDAQSourceChannel(store, source);
            default:
                throw new UnsupportedOperationException("Unsupported mode " + mode);
        }
    }

    protected final Store store;
    protected DAQSourceChannelImplementation impl;
    private final Source.ChannelMode mode;

    DAQSourceChannel(Store store, Source source, Source.ChannelMode mode) throws DAQException {
        final Image image = source.getImage();
        this.store = store;
        if (mode != Source.ChannelMode.STREAM) {
            // In the case of streaming we delay opening the channel until the first data is ready 
            impl = store.openSourceChannel(image.getMetaData().getId(), source.getLocation(), mode);
        }
        this.mode = mode;
    }

    @Override
    public boolean isOpen() {
        return impl != null;
    }

    @Override
    public void close() throws IOException {
        if (impl != null) {
            synchronized (store) {
                close(mode == Source.ChannelMode.WRITE);
            }
            impl = null;
        }
    }

    void checkOpen(ByteBuffer dst) throws IOException {
        if (impl == null) {
            throw new IOException("Channel not open for read");
        }
        if (!dst.isDirect()) {
            throw new IOException("Supplied ByteBuffer is not direct");
        }
    }

    @Override
    public int read(ByteBuffer dst) throws IOException {
        throw new IOException("Channel not open for read.");
    }

    @Override
    public int write(ByteBuffer src) throws IOException {
        throw new IOException("Channel not open for write.");
    }

    protected int read(ByteBuffer dst, int position, long offset, int length) {
        return impl.read(dst, position, offset, length);
    }

    protected int write(ByteBuffer src, int position, int remaining) {
        return impl.write(src, position, remaining);
    }

    private void close(boolean write) {
        impl.close(write);
    }
    

    static class ReadableDAQSourceChannel extends DAQSourceChannel {

        private final long length;
        private long offset = 0;

        ReadableDAQSourceChannel(Store store, Source source) throws DAQException {
            super(store, source, Source.ChannelMode.READ);
            length = source.getMetaData().getLength();
        }

        @Override
        public int read(ByteBuffer dst) throws IOException {
            checkOpen(dst);
            int l = (int) Math.min(dst.remaining(), length - offset);
            if (l == 0) {
                return -1;
            }
            int position = dst.position();
            int err = read(dst, position, offset, l);
            if (err != 0) {
                DAQException daq = new DAQException("Error reading DAQ data", err);
                throw new IOException("DAQ IO exception", daq);
            }
            offset += l;
            dst.position(position + l);
            return l;
        }
    }

    static class StreamingDAQSourceChannel extends DAQSourceChannel implements StreamListener {

        private volatile long length = 0;
        private volatile boolean isComplete = false;
        private long offset = 0;
        private final Source source;
        private static final Duration READ_TIMEOUT = Duration.ofSeconds(10);

        @SuppressWarnings("LeakingThisInConstructor")
        StreamingDAQSourceChannel(Store store, Source source) throws DAQException {
            super(store, source, Source.ChannelMode.STREAM);
            this.source = source;
            source.addStreamListener(this);
        }

        @Override
        public int read(ByteBuffer dst) throws IOException {
            try {
                int l = waitForData(dst);
                if (l < 0) {
                    return -1;
                }
                if (impl == null) {
                    final Image image = source.getImage();
                    impl = store.openSourceChannel(image.getMetaData().getId(), source.getLocation(), Source.ChannelMode.STREAM);
                }
                checkOpen(dst);
                int position = dst.position();
                //System.out.printf("Read %d %d %d %d\n", channel_, position, offset, l);
                int err = read(dst, position, offset, l);
                if (err != 0) {
                    throw new DAQException("Error reading DAQ data", err);
                }
                offset += l;
                dst.position(position + l);
                return l;
            } catch (DAQException x) {
                throw new IOException("DAQ IO exception", x);
            }
        }

        private synchronized int waitForData(ByteBuffer dst) throws IOException {
            int l = (int) Math.min(dst.remaining(), length - offset);
            long finishBy = System.currentTimeMillis() + READ_TIMEOUT.toMillis();
            while (l == 0) {
                try {
                    if (isComplete) {
                        return -1;
                    } else {
                        long remainingTime = finishBy - System.currentTimeMillis();
                        if (remainingTime <= 0) {
                            throw new IOException("Streaming read timed out");
                        }
                        wait(remainingTime);
                    }
                    l = (int) Math.min(dst.remaining(), length - offset);
                } catch (InterruptedException x) {
                    throw new InterruptedIOException();
                }
            }
            return l;
        }

        @Override
        public synchronized void streamLength(long length) {
            this.length = length;
            this.notifyAll();
        }

        @Override
        public synchronized void imageComplete(long length) {
            this.length = length;
            this.isComplete = true;
            this.notifyAll();
        }

        @Override
        public void close() throws IOException {
            source.removeStreamListener(this);
            super.close();
        }

    }

    private static class WritableDAQSourceChannel extends DAQSourceChannel {

        public WritableDAQSourceChannel(Store store, Source source) throws DAQException {
            super(store, source, Source.ChannelMode.WRITE);
        }

        @Override
        public int write(ByteBuffer src) throws IOException {
            checkOpen(src);
            int remaining = src.remaining();
//            if ((remaining % (16*3)) != 0) {
//                throw new IOException("ByteBuffer size must be a multiple of 48");
//            }
            int position = src.position();
            int err = write(src, position, remaining);
            if (err != 0) {
                DAQException daq = new DAQException("Error reading DAQ data", err);
                throw new IOException("DAQ IO exception", daq);
            }
            src.position(position + remaining);
            return remaining - position;
        }
    }

}
