package org.lsst.ccs.daq.imageapi.decoder;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.Random;

/**
 *
 * @author tonyj
 */
public class Test2 {

    public static void main(String[] args) throws IOException {

        Random random = new Random();
        final int inputSize = 48 * 900_000;
        IntBuffer input = IntBuffer.allocate(inputSize);
        while (input.hasRemaining()) {
            input.put(random.nextInt());
        }
        input.flip();
        int outputSize = inputSize / 9 * 16 / 48;
        IntBufferWriter[] output = new IntBufferWriter[48];
        for (int i = 0; i < output.length; i++) {
            output[i] = new IntBufferWriter(IntBuffer.allocate(outputSize));
        }

        NullIntWriter none = new NullIntWriter();
        WritableIntChannel destination = new DemultiplexingIntChannel(output);
        Decompress18BitChannel b18 = new Decompress18BitChannel(destination);
        long start = System.nanoTime();
        b18.write(input);
        long end = System.nanoTime();
        System.out.printf("Copy took %,dnS\n",end-start);
    }

}
