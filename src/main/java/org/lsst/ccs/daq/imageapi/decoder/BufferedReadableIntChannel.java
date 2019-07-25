package org.lsst.ccs.daq.imageapi.decoder;

import java.io.EOFException;
import java.io.IOException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class BufferedReadableIntChannel implements ReadableIntChannel {

    private final IntBuffer internalBuffer;
    private final ReadableIntChannel input;
    
    public BufferedReadableIntChannel(ReadableIntChannel input, int bufSize) {
        internalBuffer = IntBuffer.allocate(bufSize);  
        this.input = input;
    }
            
    @Override
    public int read(IntBuffer buffer) throws IOException {
        if (internalBuffer.hasRemaining()) {
            int remaining = internalBuffer.remaining();
            internalBuffer.put(buffer);
            return remaining - internalBuffer.remaining();
        } else {
            return input.read(buffer);
        }
    }

    @Override
    public int read() throws IOException {
        if (!internalBuffer.hasRemaining()) {
            internalBuffer.clear();
            input.read(internalBuffer);
            internalBuffer.flip();
        }
        if (internalBuffer.hasRemaining()) {
            return internalBuffer.get();
        } else {
            throw new EOFException();
        }
    }
    
    @Override
    public boolean isOpen() {
        return input.isOpen();
    }

    @Override
    public void close() throws IOException {
        input.close();
    }
}