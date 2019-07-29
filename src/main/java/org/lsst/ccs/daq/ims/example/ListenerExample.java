package org.lsst.ccs.daq.ims.example;

import java.util.List;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageListener;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Store;

/**
 *
 * @author tonyj
 */
public class ListenerExample {

    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        store.addImageListener(new ImageListener() {
            @Override
            public void imageCreated(Image image) {
                System.out.println("Image created " + image);
                List<Source> sources = image.listSources();
                for (Source source : sources) {
                    System.out.println(source);
                }
            }

            @Override
            public void imageComplete(Image image) {
                System.out.println("Image complete " + image);
                List<Source> sources = image.listSources();
                for (Source source : sources) {
                    System.out.println(source);
                }
            }
        });
    }

}
