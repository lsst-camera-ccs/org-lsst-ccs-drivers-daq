package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.ByteChannel;
import nom.tam.fits.TruncatedFileException;
import org.lsst.ccs.daq.ims.Catalog;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageMetaData;
import org.lsst.ccs.daq.ims.Location;
import org.lsst.ccs.daq.ims.LocationSet;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Source.ChannelMode;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.channel.FitsIntReader;

/**
 *
 * @author tonyj
 */
public class FitsWriter {

    public static void main(String[] args) throws DAQException, IOException, TruncatedFileException {
        Store store = new Store("dev");
        Catalog catalog = store.getCatalog();
        Folder testFolder = catalog.find("tonyj-test");
        Image old = testFolder.find("Flat_screen_0000_20190322172301");
        old.delete();
        ImageMetaData meta = new ImageMetaData("Flat_screen_0000_20190322172301","Image Annotation", 0, LocationSet.of("R22/Reb0"));        
        Image image = testFolder.insert(meta);

        File dir = new File("/home/tonyj/Data/pretty/");
        File[] files = dir.listFiles((File dir1, String name) -> name.matches("1\\d_Flat_screen_0000_20190322172301.fits"));
        try (FitsIntReader reader = new FitsIntReader(files)) {
            int[] registerValues = { 1, 2, 3, 4, 5, 6 };
            Source source = image.addSource(Location.of("R22/Reb0"), registerValues);
            try (ByteChannel channel = source.openChannel(ChannelMode.WRITE)) {
                ByteBuffer buffer = ByteBuffer.allocateDirect(1024*1024);
                IntBuffer intBuffer = buffer.asIntBuffer();
                for (;;) {
                    intBuffer.clear();
                    int len = reader.read(intBuffer);
                    if (len<0) break;
                    buffer.position(0);
                    buffer.limit(4*len);
                    channel.write(buffer);
                }
            }
        }
    }
}
