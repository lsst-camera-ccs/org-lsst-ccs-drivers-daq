package org.lsst.ccs.daq.imageapi;

/**
 *
 * @author tonyj
 */
interface StreamListener {
    void streamLength(long length);
    void imageComplete(long length);
}
