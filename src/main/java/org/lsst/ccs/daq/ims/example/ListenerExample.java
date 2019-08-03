package org.lsst.ccs.daq.ims.example;

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
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

    private static final Logger LOG = Logger.getLogger(ListenerExample.class.getName());
    

    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        store.addImageListener(new ImageListener() {
            @Override
            public void imageCreated(Image image) {
                try {
                    System.out.println("Image created " + image);
                    List<Source> sources = image.listSources();
                    for (Source source : sources) {
                        System.out.println(source);
                    }
                } catch (DAQException ex) {
                    LOG.log(Level.SEVERE, "Exception in imageCreated listener", ex);
                }
            }

            @Override
            public void imageComplete(Image image) {
                try {
                    System.out.println("Image complete " + image);
                    List<Source> sources = image.listSources();
                    for (Source source : sources) {
                        System.out.println(source);
                    }
                } catch (DAQException ex) {
                    LOG.log(Level.SEVERE, "Exception in imageComplete listener", ex);
                }
            }
        });
    }

}
