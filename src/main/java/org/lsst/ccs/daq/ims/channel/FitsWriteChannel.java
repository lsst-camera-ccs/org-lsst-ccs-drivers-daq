package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import org.lsst.ccs.utilities.image.FitsFileWriter;

import java.nio.IntBuffer;
/**
 *
 * @author tonyj
 */
public class FitsWriteChannel implements WritableIntChannel {

    ByteBuffer bb = ByteBuffer.allocateDirect(1_000_000);
    private final int segment;
    private final FitsFileWriter writer;
    private boolean isOpen = true;
    
    public FitsWriteChannel(FitsFileWriter writer, int segment) {
        this.writer = writer;
        this.segment = segment;
    }

    @Override
    public void write(int i) throws IOException {
        if (bb.remaining()<4) {
            flush();
        }
        bb.putInt(i);
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
        while (buffer.hasRemaining()) {
            IntBuffer asIntBuffer = bb.asIntBuffer();
            if (buffer.remaining()<asIntBuffer.remaining()) {
                asIntBuffer.put(buffer);
                bb.position(bb.position() + asIntBuffer.position());
            } else {
                int oldLimit = buffer.limit();
                buffer.limit(buffer.position() + asIntBuffer.remaining());
                asIntBuffer.put(buffer);
                bb.position(bb.position() + asIntBuffer.position());
                buffer.limit(oldLimit);
            }
            if (bb.remaining()==0) {
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

    private void flush() throws IOException {
        bb.flip();
        writer.write(segment, bb);
        bb.clear();
    }
    
}
