package org.lsst.ccs.daq.example;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.List;
import nom.tam.fits.TruncatedFileException;
import org.lsst.ccs.daq.imageapi.Catalog;
import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.DAQSourceChannel;
import org.lsst.ccs.daq.imageapi.DAQSourceChannel.Mode;
import org.lsst.ccs.daq.imageapi.Folder;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.ImageMetaData;
import org.lsst.ccs.daq.imageapi.LocationSet;
import org.lsst.ccs.daq.imageapi.Source;
import org.lsst.ccs.daq.imageapi.Store;
import org.lsst.ccs.daq.imageapi.decoder.FitsIntReader;

/**
 *
 * @author tonyj
 */
public class FitsWriter {

    public static void main(String[] args) throws DAQException, IOException, TruncatedFileException {
        Store store = new Store("dev");
        Catalog catalog = store.getCatalog();
        Folder testFolder = catalog.find("tonyj-test");
        ImageMetaData meta = new ImageMetaData("Flat_screen_0000_20190322172301","Image Annotation", 0, LocationSet.of("R22/Reb0"));        
        Image image = testFolder.insert(meta);

        File dir = new File("/home/tonyj/Data/pretty/");
        File[] files = dir.listFiles((File dir1, String name) -> name.matches("1\\d_Flat_screen_0000_20190322172301.fits"));
        try (FitsIntReader reader = new FitsIntReader(files)) {
            List<Source> sources = image.listSources();
            Source source = sources.get(0);
            try (DAQSourceChannel channel = source.openChannel(Mode.WRITE)) {
                ByteBuffer buffer = ByteBuffer.allocateDirect(1_000_000);
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
