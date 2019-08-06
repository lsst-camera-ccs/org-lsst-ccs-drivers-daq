package org.lsst.ccs.daq.ims.example;

import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.zip.CRC32;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageListener;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Store;

/**
 *
 * @author tonyj
 */
public class StreamExample {

    private static final Logger LOG = Logger.getLogger(StreamExample.class.getName());

    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        ExecutorService threadPool = Executors.newCachedThreadPool();
        store.addImageListener(new ImageListener() {
            @Override
            public void imageCreated(Image image) {
                System.out.println("Image created " + image);
                // Note, avoid long computations in image listener
                threadPool.submit(new ImageStreamer(image));
            }

            @Override
            public void imageComplete(Image image) {
                System.out.println("Image complete " + image);
            }
        });
    }

    private static class ImageStreamer implements Runnable {

        private final Image image;

        public ImageStreamer(Image image) {
            this.image = image;
        }

        @Override
        public void run() {
            try {
                List<Source> sources = image.listSources();
                Source source = sources.get(0);
                try (ByteChannel channel = source.openChannel(Source.ChannelMode.STREAM)) {
                    ByteBuffer buffer = ByteBuffer.allocateDirect(100_000);
                    long totalReadSize = 0;
                    CRC32 cksum = new CRC32();
                    long start = System.nanoTime();
                    for (;;) {
                        buffer.clear();
                        int l = channel.read(buffer);
                        if (l < 0) {
                            break;
                        }
                        buffer.flip();
                        cksum.update(buffer);
                        totalReadSize += l;
                    }
                    long stop = System.nanoTime();
                    System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));
                    System.out.printf("Checksum %,d\n", cksum.getValue());
                }
            } catch (Throwable x) {
                LOG.log(Level.SEVERE, "Error reading image", x);
            }
        }
    }

}
