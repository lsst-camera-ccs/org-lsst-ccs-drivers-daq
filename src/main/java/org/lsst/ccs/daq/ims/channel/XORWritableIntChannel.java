package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class XORWritableIntChannel implements WritableIntChannel {

    private final WritableIntChannel input;
    private final int xorPattern;

    public XORWritableIntChannel(WritableIntChannel input, int xorPattern) {
        this.input = input;
        this.xorPattern = xorPattern;
    }

    @Override
    public void write(int i) throws IOException {
        input.write(i ^ xorPattern);
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
