package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.channels.ByteChannel;
import java.util.Collections;
import java.util.List;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Store;

/**
 *
 * @author tonyj
 */
public class RawDump {

    public static void main(String[] args) throws IOException, DAQException {
        ByteBuffer buffer = ByteBuffer.allocateDirect(1200000);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        try (Store store = new Store("camera")) {
            Folder raw = store.getCatalog().find("raw");
            List<Image> images = raw.listImages();
            Collections.sort(images);
            for (Image image : images) {
                List<Source> sources = image.listSources();
                for (Source source : sources) {
                    try (ByteChannel openChannel = source.openChannel(Source.ChannelMode.READ);
                            NullIntWriter devNull = new NullIntWriter();
                            BadPixelDetector bad = new BadPixelDetector(devNull);
                            XORWritableIntChannel xor = new XORWritableIntChannel(bad, 0x1FFFF);
                            Decompress18BitChannel decompress18BitChannel = new Decompress18BitChannel(xor)) {
                        for (;;) {
                            buffer.clear();
                            int read = openChannel.read(buffer);
                            if (read < 0) {
                                break;
                            }
                            buffer.flip();
                            IntBuffer intBuffer = buffer.asIntBuffer();

                            decompress18BitChannel.write(intBuffer);
                        }
                        System.out.printf("%s %s %d/%d\n", image, source, bad.getBadPixels(), bad.getTotalPixels());
                    }
                }
            }
        }
    }
}
