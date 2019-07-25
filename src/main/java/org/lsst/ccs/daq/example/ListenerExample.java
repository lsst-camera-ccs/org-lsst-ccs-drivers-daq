package org.lsst.ccs.daq.example;

import org.lsst.ccs.daq.imageapi.DAQException;
import org.lsst.ccs.daq.imageapi.Image;
import org.lsst.ccs.daq.imageapi.ImageListener;
import org.lsst.ccs.daq.imageapi.Store;

/**
 *
 * @author tonyj
 */
public class ListenerExample {

    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        store.addImageListener((Image image) -> {
            throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        });
    }

}
