package org.lsst.ccs.daq.imageapi.decoder;

import java.io.IOException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class IntBufferWriter implements WritableIntChannel {

    private final IntBuffer output;
    private boolean isClosed = false;

    public IntBufferWriter(IntBuffer output) {
        this.output = output;
    }

    public IntBuffer getIntBuffer() {
        return output;
    }

    @Override
    public void write(int i) throws IOException {
        output.put(i);
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
        output.put(buffer);
    }

    @Override
    public boolean isOpen() {
        return isClosed;
    }

    @Override
    public void close() throws IOException {
        isClosed = true;
    }
    
}
