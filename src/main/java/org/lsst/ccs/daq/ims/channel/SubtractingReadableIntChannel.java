package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 * Subtract an arbitrary number from each value. Used for handling bzero when
 * reading FITS files.
 * @author tonyj
 */
public class SubtractingReadableIntChannel extends FilterReadableIntChannel {

    private final int offset;

    public SubtractingReadableIntChannel(ReadableIntChannel input, int offset) {
        super(input);
        this.offset = offset;
    }

    @Override
    public int read() throws IOException {
        return input.read() - offset;
    }

}
