package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.IntBuffer;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class Compress18BitChannelTest {

    @Test
    public void testDecompressCompress() throws IOException {

        int[] data = {1 << 16, 2 << 16, 3 << 16, 4, 5, 6, 7, 8, 9};
        IntBuffer input = IntBuffer.wrap(data);
        IntBuffer output1 = IntBuffer.allocate(8);
        IntBuffer output2 = IntBuffer.allocate(8);
        try (WritableIntChannel destination = new DemultiplexingIntChannel(new IntBufferWriter(output1), new IntBufferWriter(output2));
                Decompress18BitChannel b18 = new Decompress18BitChannel(destination)) {
            b18.write(input);
        }
        assertEquals(8, output1.position());
        assertEquals(8, output2.position());
        output1.flip();
        output2.flip();

        try (MultiplexingIntChannel multi = new MultiplexingIntChannel(new IntBufferReader(output1), new IntBufferReader(output2));
                Compress18BitChannel compress = new Compress18BitChannel(multi)) {
            int[] result = new int[data.length];
            for (int i = 0; i < result.length; i++) {
                result[i] = compress.read();
            }
            assertArrayEquals(data, result);
        }
    }
}
