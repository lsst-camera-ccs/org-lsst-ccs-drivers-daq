package org.lsst.ccs.daq.ims.channel;

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import org.lsst.ccs.daq.util.DirectByteBufferCache;

/**
 * A class for reading data from a file channel, at a given position and length
 * within the corresponding file.
 * @author tonyj
 */
public class IntFileChannelReader implements ReadableIntChannel {

    private final FileChannel channel;
    private long position;
    private int length;
    private ByteBuffer bb = DirectByteBufferCache.instance().allocateDirect(1_000_000);
    private IntBuffer ib = bb.asIntBuffer();


    IntFileChannelReader(FileChannel channel, long position, int length) throws IOException {
        this.channel = channel;
        this.position = position;
        this.length = length;
        ib.flip();
    }

    @Override
    public int read() throws IOException {
        while (!ib.hasRemaining()) {
            int intsRead = fillBuffer();
            if (intsRead<0) {
                throw new EOFException();
            }
        }
        return ib.get();
    }

    @Override
    public int read(IntBuffer buffer) throws IOException {
        while (!ib.hasRemaining()) {
            int intsRead = fillBuffer();
            if (intsRead < 0) return intsRead;
        }
       int remaining = ib.remaining();
       if (remaining>buffer.remaining()) {
           int oldLimit = ib.limit();
           ib.limit(ib.position() + buffer.remaining());
           buffer.put(ib);
           ib.limit(oldLimit);
       } else {
           buffer.put(ib);
       }
       return remaining - ib.remaining();
    }

    @Override
    public boolean isOpen() {
        return bb != null;
    }

    @Override
    public void close() throws IOException {
        DirectByteBufferCache.instance().free(bb);
        ib = null;
        bb = null;
    }

    private int fillBuffer() throws IOException {
        bb.clear();
        if (length<bb.capacity()) {
            bb.limit(length);
        }
        int bytesRead = channel.read(bb,position);
        if (bytesRead<=0) {
            return -1;
        }
        position += bytesRead;
        length -= bytesRead;
        ib.position(0);
        ib.limit(bytesRead/4);
        return bytesRead/4;
    }
        
}
