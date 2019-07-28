package org.lsst.ccs.daq.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.Collections;
import java.util.Random;
import org.lsst.ccs.daq.imageapi.Catalog;
import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.Folder;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.ImageMetaData;
import org.lsst.ccs.daq.imageapi.Location;
import org.lsst.ccs.daq.imageapi.Source;
import org.lsst.ccs.daq.imageapi.Store;

/**
 *
 * @author tonyj
 */
public class WriteExample {
    public static void main(String[] args) throws DAQException, IOException {
        Store store = new Store("dev");
        Catalog catalog = store.getCatalog();
        Folder testFolder;
        try {
           testFolder = catalog.find("tonyj-test");
        } catch (DAQException x) {
           testFolder = catalog.insert("tonyj-test");
        }
        try {
            Image image = testFolder.find("imageName");
            if (image != null) {
                image.delete();
            }
        } catch (DAQException x) {
            
        }
        final Location location = new Location((byte) 22, (byte)1);
        ImageMetaData meta = new ImageMetaData("imageName","Image Annotation", 2, Collections.singleton(location));
        Image image = testFolder.insert(meta);
        System.out.println(image.getMetaData());
        int[] registerValues = { 1, 2, 3, 4, 5, 6 };
        Source source = image.addSource(location, registerValues);
        ByteBuffer buffer = ByteBuffer.allocateDirect(1000000);
        Random r = new Random();
        for (int i=0; i<1000000; i+=4) {
            buffer.putInt(r.nextInt());
        }
        buffer.flip();
        try (ByteChannel channel = source.openChannel(Source.ChannelMode.WRITE)) {
            channel.write(buffer);
        }
        System.out.println(source);
    }
}
