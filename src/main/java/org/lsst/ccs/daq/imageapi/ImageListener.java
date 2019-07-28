package org.lsst.ccs.daq.imageapi;

/**
 *
 * @author tonyj
 */
public interface ImageListener {
    void imageCreated(Image image);
    void imageComplete(Image image);
}
