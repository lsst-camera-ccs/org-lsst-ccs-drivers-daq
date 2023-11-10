package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.lsst.ccs.utilities.image.FitsFileWriter;

import java.nio.IntBuffer;
import org.lsst.ccs.daq.util.DirectByteBufferCache;
/**
 *
 * @author tonyj
 */
public class FitsWriteChannel implements WritableIntChannel {

    ByteBuffer currentBuffer;
    final String segment;
    final FitsFileWriter writer;
    boolean isOpen = true;
    final DirectByteBufferCache cache = DirectByteBufferCache.instance();
    
    @SuppressWarnings("OverridableMethodCallInConstructor")
    public FitsWriteChannel(FitsFileWriter writer, String segment) throws IOException {
        this.writer = writer;
        this.segment = segment;
        initBuffers();
    }

    void initBuffers() throws IOException {
        currentBuffer = cache.allocateDirect(1_000_000);
        currentBuffer.order(ByteOrder.BIG_ENDIAN);
    }

    @Override
    public void write(int i) throws IOException {
        try {
            currentBuffer.putInt(i);
        } catch (BufferOverflowException x) {
            flush();
            currentBuffer.putInt(i);
        }
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
        while (buffer.hasRemaining()) {
            IntBuffer asIntBuffer = currentBuffer.asIntBuffer();
            if (buffer.remaining()<asIntBuffer.remaining()) {
                asIntBuffer.put(buffer);
                currentBuffer.position(currentBuffer.position() + 4 * asIntBuffer.position());
            } else {
                int oldLimit = buffer.limit();
                buffer.limit(buffer.position() + asIntBuffer.remaining());
                asIntBuffer.put(buffer);
                currentBuffer.position(currentBuffer.position() + 4 * asIntBuffer.position());
                buffer.limit(oldLimit);
            }
            if (currentBuffer.remaining()==0) {
                flush();
            }
        }
    }
    
    
    @Override
    public boolean isOpen() {
        return isOpen;
    }

    @Override
    public void close() throws IOException {
        if (isOpen) {
            flush();
            isOpen = false;
            cache.free(currentBuffer);
        }
    }

    void flush() throws IOException {
        currentBuffer.flip();
        writer.write(segment, currentBuffer);
        currentBuffer.clear();
    }
    
}
