package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;

/**
 *
 * @author tonyj
 */
public class DAQSourceChannelNativeImplementation extends DAQSourceChannelImplementation {

    private final long source_;

    DAQSourceChannelNativeImplementation(long source_) {
        this.source_ = source_;
    }

    @Override
    int read(ByteBuffer dst, int position, long offset, int length) {
        return read(source_, dst, position, offset, length);
    }

    @Override
    int write(ByteBuffer src, int position, int remaining) {
        return write(source_, src, position, remaining);
    }

    @Override
    void close(boolean write) {
        close(source_,write);
    }

    native int read(long source_, ByteBuffer dst, int position, long offset, int length);

    native int write(long source_, ByteBuffer src, int position, int remaining);

    native void close(long source_, boolean write);

}
