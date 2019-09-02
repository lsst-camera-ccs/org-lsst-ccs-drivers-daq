package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class Decompress18BitChannel extends FilterWritableIntChannel {

    private int bitsRemaining;
    private long dataRemaining;

    public Decompress18BitChannel(WritableIntChannel input) {
        super(input);
        this.bitsRemaining = 0;
        this.dataRemaining = 0;
    }

    @Override
    public void write(int data) throws IOException {
        dataRemaining |= (data & 0xFFFFFFFFL) << bitsRemaining;
        bitsRemaining += 32;
        while (bitsRemaining >= 18) {
           input.write((int) (dataRemaining & 0x2FFFF));
           dataRemaining >>>= 18;        
           bitsRemaining -= 18;
        }
    }

    @Override
    public void close() throws IOException {
        super.close();
        if (bitsRemaining != 0) {
            throw new IOException("Unused data remaining on close");
        }
    }

}
