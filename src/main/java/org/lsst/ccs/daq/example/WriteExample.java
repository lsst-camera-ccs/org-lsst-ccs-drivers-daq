package org.lsst.ccs.daq.example;

import java.util.List;
import org.lsst.ccs.daq.imageapi.Catalog;
import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.Folder;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.ImageMetaData;
import org.lsst.ccs.daq.imageapi.Location;
import org.lsst.ccs.daq.imageapi.LocationSet;
import org.lsst.ccs.daq.imageapi.Source;
import org.lsst.ccs.daq.imageapi.Store;

/**
 *
 * @author tonyj
 */
public class WriteExample {
    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        Catalog catalog = store.getCatalog();
        Folder testFolder = catalog.find("tonyj-test");
        if (testFolder == null) {
           testFolder = catalog.insert("tonyj-test");
        }
        try {
            Image image = testFolder.find("imageName");
            if (image != null) {
                image.delete();
            }
        } catch (DAQException x) {
            
        }
        ImageMetaData meta = new ImageMetaData("imageName","Image Annotation", 2, LocationSet.singleton(new Location((byte) 22, (byte)1)));
        Image image = testFolder.insert(meta);
        System.out.println(image.getMetaData());
        List<Source> sources = image.listSources();
        System.out.println(sources);
    }
}
