package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 * 
 * @author tonyj
 */
public class DemultiplexingIntChannel implements WritableIntChannel {

    private final WritableIntChannel[] destinations;
    private final int size;
    private int next;
    private boolean isClosed;

    public DemultiplexingIntChannel(WritableIntChannel... destinations) {
        this.destinations = destinations;
        size = destinations.length;
        next = 0;
    }

    @Override
    public boolean isOpen() {
        return !isClosed;
    }

    @Override
    public void close() throws IOException {
        for (WritableIntChannel channel : destinations) {
            channel.close();
        }
        isClosed = true;
    }

    @Override
    public void write(int i) throws IOException {
        destinations[next++].write(i);
        if (next>=size) next = 0;
    }

}
