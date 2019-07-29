package org.lsst.ccs.daq.ims;

/**
 *
 * @author tonyj
 */
interface StreamListener {
    void streamLength(long length);
    void imageComplete(long length);
}
