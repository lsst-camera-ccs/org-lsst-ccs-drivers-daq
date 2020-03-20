package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class DemultiplexingIntChannel implements WritableIntChannel {
    
    private final WritableIntChannel[] destinations;
    private final int size;
    private int next;
    private boolean isClosed;
    private final int[][] buffer;
    private int bufferOffset;
    
    public DemultiplexingIntChannel(WritableIntChannel... destinations) {
        this.destinations = destinations;
        size = destinations.length;
        this.buffer = new int[size][10000];
        this.bufferOffset = 0;
        next = 0;
    }
    
    @Override
    public boolean isOpen() {
        return !isClosed;
    }
    
    @Override
    public void close() throws IOException {
        flush();
        for (WritableIntChannel channel : destinations) {
            channel.close();
        }
        isClosed = true;
    }
    
    @Override
    public void write(int i) throws IOException {
        buffer[next++][bufferOffset] = i;
        if (next >= size) {
            next = 0;
            bufferOffset++;
            if (bufferOffset == 10000) {
                flush();
            }
        }
    }
    
    private void flush() throws IOException {
        if (next != 0) {
            throw new IOException("Incomplete buffer during flush");
        }
        for (int i = 0; i < size; i++) {
            destinations[i].write(IntBuffer.wrap(buffer[i], 0, bufferOffset));
        }
        bufferOffset = 0;
    }
    
}
