package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 * An abstract channel which can be used to filter data int by int
 * @author tonyj
 */
public abstract class FilterReadableIntChannel implements ReadableIntChannel {
    
    protected final ReadableIntChannel input;

    public FilterReadableIntChannel(ReadableIntChannel input) {
        this.input = input;
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
