package org.lsst.ccs.daq.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.ImageListener;
import org.lsst.ccs.daq.imageapi.Source;
import org.lsst.ccs.daq.imageapi.Store;

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
                List<Source> sources = image.listSources();
                sources.forEach((source) -> {
                    System.out.println(source);
                });
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
            List<Source> sources = image.listSources();
            Source source = sources.get(0);
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.STREAM)) {
                ByteBuffer buffer = ByteBuffer.allocateDirect(100_000);
                long totalReadSize = 0;
                long start = System.nanoTime();
                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                }
                long stop = System.nanoTime();
                System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));
            } catch (Throwable x) {
                LOG.log(Level.SEVERE, "Error reading image", x);
            }
        }
    }

}
