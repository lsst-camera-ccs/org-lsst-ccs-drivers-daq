package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class Compress18BitChannel implements ReadableIntChannel {

    private final ReadableIntChannel input;
    private int bitsRemaining;
    private int dataRemaining;
    
    Compress18BitChannel(ReadableIntChannel input) {
        this.input = input;
        this.bitsRemaining = 0;
        this.dataRemaining = 0;
    }

    @Override
    public int read() throws IOException {
        int result;
        int bitPosition;
        if (bitsRemaining > 0) {
            result = dataRemaining;
            bitPosition = bitsRemaining;
        } else {
            result = 0;
            bitPosition = 0;
        }
        int in = input.read();
        if (bitPosition < 32-18) {
            result |= in<<bitPosition;
            bitPosition += 18;
            in = input.read();
        } 
        result |= in<<bitPosition;
        bitPosition += 18;
        bitsRemaining = bitPosition - 32;
        dataRemaining = in>>(18-bitsRemaining);
        return result;    
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
