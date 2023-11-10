package org.lsst.ccs.daq.util;

import java.lang.management.ManagementFactory;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.management.InstanceAlreadyExistsException;
import javax.management.MBeanRegistrationException;
import javax.management.MBeanServer;
import javax.management.MalformedObjectNameException;
import javax.management.NotCompliantMBeanException;
import javax.management.ObjectName;

/**
 * Memory allocated in direct buffers is not counted again the Java heap, and as a result
 * can result in a large off-heap memory usage which the garbage collector can be very
 * slow to recover. This class aims to facilitate re-use of existing direct buffers, rather than waiting
 * for the garbage collector to recover the associated memory.
 * 
 * This simple-minded implementation keeps an independent cache for each size buffer, and is 
 * only suitable for use with a small number of distinct sized buffers.
 * @author tonyj
 */
public class DirectByteBufferCache implements DirectByteBufferCacheMBean {

    private static final Logger LOG = Logger.getLogger(DirectByteBufferCache.class.getName());
    
    private final Map<Integer, Queue<ByteBuffer>> byteBufferCache = new ConcurrentHashMap<>();
    private final AtomicInteger bufferCount = new AtomicInteger();
    private final AtomicLong totalCapacity = new AtomicLong();
    private final AtomicInteger inUseCount = new AtomicInteger();
    private final AtomicLong inUseCapacity = new AtomicLong();
    private static final DirectByteBufferCache theInstance = new DirectByteBufferCache();
    
    private DirectByteBufferCache() {
        try {
            MBeanServer mbs = ManagementFactory.getPlatformMBeanServer();
            ObjectName name = new ObjectName("org.lsst.ccs.daq.util:name=DirectByteBufferCache");
            mbs.registerMBean(this, name);
        } catch (MBeanRegistrationException | MalformedObjectNameException | InstanceAlreadyExistsException | NotCompliantMBeanException x) {
            LOG.log(Level.WARNING, "Failed to register mbean", x);
        } 
    }
    
    public static DirectByteBufferCache instance() {
        return theInstance;
    }
    
    public ByteBuffer allocateDirect(int capacity) {
        Queue<ByteBuffer> cachedByteBuffers = byteBufferCache.computeIfAbsent(capacity, (s) -> new LinkedBlockingQueue<>());
        ByteBuffer bb = cachedByteBuffers.poll();
        if (bb == null) {
            bb = ByteBuffer.allocateDirect(capacity);
            bufferCount.incrementAndGet();
            totalCapacity.addAndGet(capacity);
        }
        inUseCount.incrementAndGet();
        inUseCapacity.addAndGet(capacity);        
        return bb;
    }
    
    public void free(ByteBuffer byteBuffer) {
        final int capacity = byteBuffer.capacity();
        byteBuffer.clear();
        byteBuffer.order(ByteOrder.BIG_ENDIAN);
        Queue<ByteBuffer> cachedByteBuffers = byteBufferCache.get(capacity);
        cachedByteBuffers.add(byteBuffer);
        inUseCount.decrementAndGet();
        inUseCapacity.addAndGet(-capacity);           
    }

    @Override
    public int getBufferCount() {
        return bufferCount.get();
    }

    @Override
    public long getTotalCapacity() {
        return totalCapacity.get();
    }

    @Override
    public int getInUseCount() {
        return inUseCount.get();
    }

    @Override
    public long getInUseCapacity() {
        return inUseCapacity.get();
    }

    @Override
    public int getByteBufferCacheSize() {
        return byteBufferCache.size();
    }
}
