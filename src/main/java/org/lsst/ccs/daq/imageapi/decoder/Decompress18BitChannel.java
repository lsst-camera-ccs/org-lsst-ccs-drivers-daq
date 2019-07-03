package org.lsst.ccs.daq.imageapi.decoder;

import java.io.IOException;
import java.nio.IntBuffer;

/**
 *
 * @author tonyj
 */
public class Decompress18BitChannel implements WritableIntChannel {

    private final WritableIntChannel destination;

    public Decompress18BitChannel(WritableIntChannel destination) {
        this.destination = destination;
    }

    @Override
    public void write(int i) throws IOException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void write(IntBuffer input) throws IOException {
        int d0, d1;
        while (input.hasRemaining()) {
            d0 = input.get();
            destination.write(d0 & 0x2ffff);
            d0 >>= 18;
            d1 = input.get();
            destination.write((d0 | d1 << 14) & 0x2ffff);
            d1 >>= 4;
            destination.write(d1 & 0x2ffff);
            d1 >>= 18;
            d0 = input.get();
            destination.write((d1 | d0 << 10) & 0x2ffff);
            d0 >>= 8;
            destination.write(d0 & 0x2ffff);
            d0 >>= 18;
            d1 = input.get();
            destination.write((d0 | d1 << 6) & 0x2ffff);
            d1 >>= 12;
            destination.write(d1 & 0x2ffff);
            d1 >>= 18;
            d0 = input.get();
            destination.write((d1 | d0 << 2) & 0x2ffff);
            d0 >>= 16;
            d1 = input.get();
            destination.write((d0 | d1 << 16) & 0x2ffff);
            d1 >>= 2;
            destination.write(d1 & 0x2ffff);
            d1 >>= 18;
            d0 = input.get();
            destination.write((d1 | d0 << 12) & 0x2ffff);
            d0 >>= 6;
            destination.write(d0 & 0x2ffff);
            d0 >>= 18;
            d1 = input.get();
            destination.write((d0 | d1 << 8) & 0x2ffff);
            d1 >>= 10;
            destination.write(d1 & 0x2ffff);
            d1 >>= 18;
            d0 = input.get();
            destination.write((d1 | d0 << 4) & 0x2ffff);
            d0 >>= 14;
            destination.write(d0 & 0x2ffff);
        }
    }

    @Override
    public boolean isOpen() {
        return destination.isOpen();
    }

    @Override
    public void close() throws IOException {
        destination.close();
    }

}
