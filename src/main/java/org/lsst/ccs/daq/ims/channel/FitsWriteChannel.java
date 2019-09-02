package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.lsst.ccs.utilities.image.FitsFileWriter;

import java.nio.IntBuffer;
/**
 *
 * @author tonyj
 */
public class FitsWriteChannel implements WritableIntChannel {

    ByteBuffer currentBuffer;
    final int segment;
    final FitsFileWriter writer;
    boolean isOpen = true;
    
    public FitsWriteChannel(FitsFileWriter writer, int segment) {
        this.writer = writer;
        this.segment = segment;
        currentBuffer = ByteBuffer.allocateDirect(1_000_000);
        currentBuffer.order(ByteOrder.BIG_ENDIAN);
    }

    @Override
    public void write(int i) throws IOException {
        if (currentBuffer.remaining()<4) {
            flush();
        }
        currentBuffer.putInt(i);
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
        while (buffer.hasRemaining()) {
            IntBuffer asIntBuffer = currentBuffer.asIntBuffer();
            if (buffer.remaining()<asIntBuffer.remaining()) {
                asIntBuffer.put(buffer);
                currentBuffer.position(currentBuffer.position() + asIntBuffer.position());
            } else {
                int oldLimit = buffer.limit();
                buffer.limit(buffer.position() + asIntBuffer.remaining());
                asIntBuffer.put(buffer);
                currentBuffer.position(currentBuffer.position() + asIntBuffer.position());
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
        }
    }

    void flush() throws IOException {
        currentBuffer.flip();
        writer.write(segment, currentBuffer);
        currentBuffer.clear();
    }
    
}
