package org.lsst.ccs.daq.imageapi.decoder;

import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.Channel;

/**
 *
 * @author tonyj
 */
public interface ReadableIntChannel extends Channel {

    int read() throws IOException;

    default void read(IntBuffer buffer) throws IOException {
        while (true) {
           buffer.put(read());
        }
    }
}
