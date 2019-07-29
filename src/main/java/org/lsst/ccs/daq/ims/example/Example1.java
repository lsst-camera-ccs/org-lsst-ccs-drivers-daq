package org.lsst.ccs.daq.ims.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.ByteChannel;
import java.util.Collections;
import java.util.List;
import org.lsst.ccs.daq.ims.Catalog;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.channel.Decompress18BitChannel;
import org.lsst.ccs.daq.ims.channel.DemultiplexingIntChannel;
import org.lsst.ccs.daq.ims.channel.IntBufferWriter;
import org.lsst.ccs.daq.ims.channel.WritableIntChannel;

/**
 *
 * @author tonyj
 */
public class Example1 {

    public static void main(String[] args) throws DAQException, IOException {
        Store store = new Store("dev");
        System.out.printf("capacity %,d\n", store.getCapacity());
        System.out.printf("remaining %,d\n", store.getRemaining());
        final Catalog catalog = store.getCatalog();
        List<Folder> list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder insert = catalog.insert("test3");
        list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder find = catalog.find("test3");
        System.out.println(find.equals(insert));
        catalog.remove("test3");
        list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder folder = catalog.find("emu");
        List<Image> images = folder.listImages();
        Collections.sort(images);
        images.forEach((image) -> {
            System.out.println(image.getMetaData());
        });

        Image image = images.get(0);
        List<Source> sources = image.listSources();
        Collections.sort(sources);
        long totalSize = 0;
        for (Source source : sources) {
            System.out.println(source.getMetaData());
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ByteBuffer buffer = ByteBuffer.allocateDirect(1_000_000);
        long totalReadSize = 0;
        long start = System.nanoTime();
        for (Source source : sources) {
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.READ)) {
                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                }
            }
        }
        long stop = System.nanoTime();
        System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));

        // now unpack the buffers
        int i = 0;
        long unpackSize = 0;
        start = System.nanoTime();
        for (Source source : sources) {
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.READ)) {
                int rebs = source.getMetaData().getSensor().getCCDCount();
                int outputSize = (int) source.getMetaData().getLength() / 9 * 16 / rebs / 4;
                IntBufferWriter[] output = new IntBufferWriter[16 * rebs];
                for (int j = 0; j < output.length; j++) {
                    output[j] = new IntBufferWriter(IntBuffer.allocate(outputSize));
                }
                WritableIntChannel destination = new DemultiplexingIntChannel(output);
                Decompress18BitChannel b18 = new Decompress18BitChannel(destination);

                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                    buffer.flip();
                    b18.write(buffer.asIntBuffer());
                }

                for (IntBufferWriter output1 : output) {
                    unpackSize += 4 * output1.getIntBuffer().position();
                }
            }
        }
        stop = System.nanoTime();
        System.out.printf("Unpack %,d bytes in %,dns (%d MBytes/second)\n", unpackSize, (stop - start), 1000 * unpackSize / (stop - start));
    }
}
