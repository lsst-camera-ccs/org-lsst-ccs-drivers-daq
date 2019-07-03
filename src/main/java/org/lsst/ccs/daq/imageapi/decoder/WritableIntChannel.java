package org.lsst.ccs.daq.imageapi.decoder;

import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.Channel;

/**
 *
 * @author tonyj
 */
public interface WritableIntChannel extends Channel {

    void write(int i) throws IOException;

    default void write(IntBuffer buffer) throws IOException {
        while (buffer.hasRemaining()) {
            write(buffer.get());
        }
    }
}
