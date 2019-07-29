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
       input.put(buffer);
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
