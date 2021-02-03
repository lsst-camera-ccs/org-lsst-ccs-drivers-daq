package org.lsst.ccs.daq.ims.channel;

import java.io.EOFException;
import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.Channel;

/**
 * An interface for reading int data from a channel. Supports reading data
 * into an IntBuffer, or reading data one int at a time. 
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
     * Reads into the given buffer.
     * The default implementation continually calls {code #read()} until the buffer
     * is full or no more data is available. Implementing classes are encouraged 
     * to override this with something more efficient where possible.
     * @param buffer The buffer into which the data will be read.
     * @return The number of ints read, or -1 if end of file reached before any data was 
     * transferred.
     */
    default int read(IntBuffer buffer) throws IOException {
        int result = 0;
        try {
            while (buffer.hasRemaining())  {
                buffer.put(read());
                result++;
            }
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
