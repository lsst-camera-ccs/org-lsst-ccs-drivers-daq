package org.lsst.ccs.daq.ims;

import java.nio.ByteBuffer;

/**
 *
 * @author tonyj
 */
public abstract class DAQSourceChannelImplementation {

    abstract int read(ByteBuffer dst, int position, long offset, int length);

    abstract int write(ByteBuffer src, int position, int remaining);

    abstract void close(boolean write);
    
}
