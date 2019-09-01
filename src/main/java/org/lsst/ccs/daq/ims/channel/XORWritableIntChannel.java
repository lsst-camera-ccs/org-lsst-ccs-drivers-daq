package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class XORWritableIntChannel extends FilterWritableIntChannel {

    private final int xorPattern;

    public XORWritableIntChannel(WritableIntChannel input, int xorPattern) {
        super(input);
        this.xorPattern = xorPattern;
    }

    @Override
    public void write(int i) throws IOException {
        input.write(i ^ xorPattern);
    }

}
