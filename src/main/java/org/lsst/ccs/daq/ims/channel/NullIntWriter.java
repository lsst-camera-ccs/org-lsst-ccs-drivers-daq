package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class NullIntWriter implements WritableIntChannel {

    @Override
    public void write(int i) throws IOException {
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
    }
    

    @Override
    public boolean isOpen() {
        return true;
    }

    @Override
    public void close() throws IOException {
    }
    
}
