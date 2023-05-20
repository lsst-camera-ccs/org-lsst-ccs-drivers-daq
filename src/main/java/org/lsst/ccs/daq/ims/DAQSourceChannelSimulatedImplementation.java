package org.lsst.ccs.daq.ims;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SeekableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 *
 * @author tonyj
 */
public class DAQSourceChannelSimulatedImplementation extends DAQSourceChannelImplementation {

    private final SeekableByteChannel byteChannel;

    public DAQSourceChannelSimulatedImplementation(Path rawData) throws DAQException {
        try {
            byteChannel = Files.newByteChannel(rawData);
        } catch (IOException x) {
            throw new DAQException("Error opening rawData "+rawData);
        }
    }

    @Override
    int read(ByteBuffer dst, int position, long offset, int length) {
        try {
            // Emulate the way the equivalent DAQ function works
            byteChannel.position(offset);
            int position1 = dst.position();
            int limit = dst.limit();
            dst.position(position);
            dst.limit(length);
            int l = byteChannel.read(dst.slice());
            dst.position(position1);
            dst.limit(limit);
            return 0;
        } catch (IOException ex) {
            return 1;
        }
    }

    @Override
    int write(ByteBuffer src, int position, int remaining) {
        throw new UnsupportedOperationException("Not supported yet."); 
    }

    @Override
    void close(boolean write) {
        try {
            byteChannel.close();
        } catch (IOException ex) {
            ///Ignored for now
        }
    }

}
