package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import org.lsst.ccs.daq.ims.channel.Decompress18BitChannel;
import org.lsst.ccs.daq.ims.channel.IntBufferWriter;
import org.lsst.ccs.daq.ims.channel.IntFileChannelReader;

/**
 *
 * @author tonyj
 */
public class Unpack {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        File in = new File(args[0]);
        FileInputStream input = new FileInputStream(in);
        FileChannel channel = input.getChannel();
        IntFileChannelReader reader = new IntFileChannelReader(channel, 0, (int) in.length());
        ByteBuffer outputBuffer = ByteBuffer.allocateDirect(16*100_000_000);
        IntBuffer oib = outputBuffer.asIntBuffer();
        Decompress18BitChannel decompress = new Decompress18BitChannel(new IntBufferWriter(oib));
        File out = new File(args[0]+".unpack");
        FileOutputStream output = new FileOutputStream(out);
        FileChannel outputChannel = output.getChannel();
        ByteBuffer bb  = ByteBuffer.allocateDirect(9*1_000_000);
        IntBuffer ib = bb.asIntBuffer();
        for (;;) {
            ib.clear();
            oib.clear();
            outputBuffer.clear();
            reader.read(ib);
            ib.flip();
            decompress.write(ib);
            outputBuffer.position(oib.position()*4);
            outputBuffer.flip();
            outputChannel.write(outputBuffer);        
        }
    }
}
