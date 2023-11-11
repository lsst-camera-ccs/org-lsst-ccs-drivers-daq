package org.lsst.ccs.daq.util;

import java.util.Map;

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

    Map<Integer, Integer> getBufferSizeAndCounts();
    
}
