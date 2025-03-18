package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Compress data into packed 18bit format, as delivered by DAQ raw data.
 * @author tonyj
 */
public class Compress18BitChannel extends FilterReadableIntChannel {

    private static boolean ERROR_ON_BAD_DATA = false;
    private static final Logger LOG = Logger.getLogger(Compress18BitChannel.class.getName());
    private int bitsAvailable;
    private long dataAvailable;

    Compress18BitChannel(ReadableIntChannel input) {
        super(input);
        this.bitsAvailable = 0;
        this.dataAvailable = 0;
    }

    @Override
    public int read() throws IOException {
        while (bitsAvailable < 32) {
            int data = input.read();
            if (data<0 || data>0x3FFFF) {
                if (ERROR_ON_BAD_DATA) {
                    throw new IOException("Invalid 18 bit data: "+Integer.toHexString(data));
                } else {
                    int newData = data;
                    if (data < 0) {
                        newData = 0;
                    } else {
                        newData = 0x3ffff;
                    }
                    LOG.log(Level.WARNING, "Invalid 18 bit data: {0} treated as {1}",
                        new Object[]{Integer.toHexString(data), Integer.toHexString(newData)});
                    data = newData;
                }
            }
            long in = data & 0x3FFFFL;
            dataAvailable |= in << bitsAvailable;
            bitsAvailable += 18;
        }
        int result = (int) dataAvailable;
        bitsAvailable -= 32;
        dataAvailable >>= 32;
        return result;
    }

    @Override
    public void close() throws IOException {
        input.close();
        if (bitsAvailable != 0) {
            throw new IOException("Unused data remaining on close");
        }
    }
}
