package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.channels.FileChannel;

/**
 *
 * @author tonyj
 */
public class MemoryMappedIntFileReader extends IntBufferReader {

    MemoryMappedIntFileReader(FileChannel channel, long position, int length) throws IOException {
        super(channel.map(FileChannel.MapMode.READ_ONLY, position, length).asIntBuffer());
    }
        
}
