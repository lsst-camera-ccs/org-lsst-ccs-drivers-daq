package org.lsst.ccs.daq.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Collections;
import java.util.List;
import org.lsst.ccs.daq.imageapi.Catalog;
import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.DAQSourceChannel;
import org.lsst.ccs.daq.imageapi.Folder;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.Source;
import org.lsst.ccs.daq.imageapi.Store;
import org.lsst.ccs.daq.imageapi.decoder.Decompress18BitChannel;
import org.lsst.ccs.daq.imageapi.decoder.DemultiplexingIntChannel;
import org.lsst.ccs.daq.imageapi.decoder.IntBufferWriter;
import org.lsst.ccs.daq.imageapi.decoder.WritableIntChannel;

/**
 *
 * @author tonyj
 */
public class Example1 {

    public static void main(String[] args) throws DAQException, IOException {
        Store store = new Store("dev");
        System.out.printf("capacity %,d\n", store.getCapacity());
        System.out.printf("remaining %,d\n", store.getCurrent());
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
            try (DAQSourceChannel channel = source.openChannel(DAQSourceChannel.Mode.READ)) {
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
            try (DAQSourceChannel channel = source.openChannel(DAQSourceChannel.Mode.READ)) {
                int rebs = source.getMetaData().getSensor().getNRebs();
                int outputSize = source.getMetaData().getLength() / 9 * 16 / rebs / 4;
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
