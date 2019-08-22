package org.lsst.ccs.daq.ims.channel;

import java.io.EOFException;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class IntBufferReader implements ReadableIntChannel {

    private IntBuffer input;
    
    IntBufferReader(IntBuffer buffer) {
        this.input = buffer;
    }

    @Override
    public int read() throws IOException {
        try { 
           return input.get();
        } catch (BufferUnderflowException x) {
           throw new EOFException();
        }
    }

    @Override
    public int read(IntBuffer buffer) throws IOException {
       int remaining = input.remaining();
       if (remaining == 0) {
           return -1;
       } else if (remaining>buffer.remaining()) {
           int oldLimit = input.limit();
           input.limit(input.position() + buffer.remaining());
           buffer.put(input);
           input.limit(oldLimit);
       } else {
           buffer.put(input);
       }
       return remaining - input.remaining();
    }

    @Override
    public boolean isOpen() {
        return input != null;
    }

    @Override
    public void close() throws IOException {
        input = null;
    }
    
}
