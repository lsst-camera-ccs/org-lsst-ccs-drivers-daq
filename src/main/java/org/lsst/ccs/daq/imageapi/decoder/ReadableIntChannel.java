package org.lsst.ccs.daq.imageapi.decoder;

import java.io.EOFException;
import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.Channel;

/**
 *
 * @author tonyj
 */
public interface ReadableIntChannel extends Channel {

    /**
     * Reads a single int from the channel, or throws EOFException if no data is
     * left.
     *
     * @return The next integer read
     * @throws EOFException If no data remains in the channel
     * @throws IOException If some other IO error occurs.
     */
    int read() throws IOException;

    /**
     * Reads into the given buffer by continually calling read until the buffer
     * is full or no more data is available.
     * @return The number of ints read, or -1 if end of file reached
     */
    default int read(IntBuffer buffer) throws IOException {
        int result = 0;
        try {
            while (buffer.hasRemaining())  {
                buffer.put(read());
                result++;
            }
            buffer.flip();
            return result;
        } catch (EOFException x) {
            if (result > 0) {
                return result;
            } else {
                return -1;
            }
        }
    }
}
