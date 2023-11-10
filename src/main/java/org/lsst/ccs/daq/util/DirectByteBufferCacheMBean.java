package org.lsst.ccs.daq.util;

/**
 *
 * @author tonyj
 */
public interface DirectByteBufferCacheMBean {

    int getBufferCount();

    int getByteBufferCacheSize();

    long getInUseCapacity();

    int getInUseCount();

    long getTotalCapacity();
    
}
