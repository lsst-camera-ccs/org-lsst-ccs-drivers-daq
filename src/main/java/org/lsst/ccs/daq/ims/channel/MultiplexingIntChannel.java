package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 *
 * @author tonyj
 */
public class MultiplexingIntChannel implements ReadableIntChannel {

    private ReadableIntChannel[] input;
    private final int size;
    private int next;

    MultiplexingIntChannel(ReadableIntChannel... input) {
        this.input = input;
        this.size = input.length;
        this.next = 0;
    }

    @Override
    public int read() throws IOException {
        int result = input[next].read();
        next = (next + 1) % size;
        return result;
    }

    @Override
    public void close() throws IOException {
        if (input != null) {
            for (ReadableIntChannel in : input) {
                in.close();
            }
            input = null;
        }
    }

    @Override
    public boolean isOpen() {
        return input != null;
    }

}
