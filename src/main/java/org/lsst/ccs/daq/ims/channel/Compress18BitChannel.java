package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 * Compress data into packed 18bit format, as delivered by DAQ raw data.
 * @author tonyj
 */
public class Compress18BitChannel implements ReadableIntChannel {

    private final ReadableIntChannel input;
    private int bitsAvailable;
    private long dataAvailable;

    Compress18BitChannel(ReadableIntChannel input) {
        this.input = input;
        this.bitsAvailable = 0;
        this.dataAvailable = 0;
    }

    @Override
    public int read() throws IOException {
        while (bitsAvailable < 32) {
            long in = input.read() & 0xFFFFFFFFL;
            dataAvailable |= in << bitsAvailable;
            bitsAvailable += 18;
        }
        int result = (int) dataAvailable;
        bitsAvailable -= 32;
        dataAvailable >>>= 32;
        return result;
    }

    @Override
    public boolean isOpen() {
        return input.isOpen();
    }

    @Override
    public void close() throws IOException {
        input.close();
        if (bitsAvailable != 0) {
            throw new IOException("Unused data remaining on close");
        }
    }
}
