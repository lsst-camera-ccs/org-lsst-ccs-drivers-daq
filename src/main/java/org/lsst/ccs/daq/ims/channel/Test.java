package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;

/**
 *
 * @author tonyj
 */
public class Test {

    public static void main(String[] args) throws IOException {

        int[] data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        IntBuffer input = IntBuffer.wrap(data);
        IntBuffer output1 = IntBuffer.allocate(8);
        IntBuffer output2 = IntBuffer.allocate(8);
        WritableIntChannel destination = new DemultiplexingIntChannel(new IntBufferWriter(output1), new IntBufferWriter(output2));
        Decompress18BitChannel b18 = new Decompress18BitChannel(destination);
        b18.write(input);
        System.out.println(Arrays.toString(output1.array()));
        System.out.println(Arrays.toString(output2.array()));
        System.out.println(output1.position());
        System.out.println(output2.position());
        
        output1.flip();
        output2.flip();
        
        MultiplexingIntChannel multi = new MultiplexingIntChannel(new IntBufferReader(output1), new IntBufferReader(output2));
        Compress18BitChannel compress = new Compress18BitChannel(multi);
        int[] result = new int[data.length];
        for (int i=0; i<result.length; i++) {
            result[i] = compress.read();
        }
        System.out.println(Arrays.toString(result));
    }

}
