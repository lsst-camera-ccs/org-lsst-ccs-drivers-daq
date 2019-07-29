package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;

/**
 *
 * @author tonyj
 */
public class IntFileChannelReader extends IntBufferReader {


    IntFileChannelReader(FileChannel channel, long position, int length) throws IOException {
        super(channel.map(FileChannel.MapMode.READ_ONLY, position, length).asIntBuffer());
    }
        
}
