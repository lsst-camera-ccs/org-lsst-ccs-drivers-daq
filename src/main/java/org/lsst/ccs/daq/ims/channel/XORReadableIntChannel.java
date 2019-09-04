package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class XORReadableIntChannel extends FilterReadableIntChannel {

    private final int xorPattern;

    public XORReadableIntChannel(ReadableIntChannel input, int xorPattern) {
        super(input);
        this.xorPattern = xorPattern;
    }

    @Override
    public int read() throws IOException {
        return input.read() ^ xorPattern;
    }

}
