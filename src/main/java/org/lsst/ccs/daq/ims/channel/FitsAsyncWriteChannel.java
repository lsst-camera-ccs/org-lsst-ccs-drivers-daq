package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.lsst.ccs.utilities.image.FitsFileWriter;
import java.nio.channels.CompletionHandler;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 *
 * @author tonyj
 */
public class FitsAsyncWriteChannel extends FitsWriteChannel {

    private static final int NBUFFERS = 2;
    private BlockingQueue<ByteBuffer> availableBuffers;
    private BlockingQueue<Throwable> exceptions;

    public FitsAsyncWriteChannel(FitsFileWriter writer, String segment) throws IOException {
        super(writer, segment);
    }

    // Note: initBuffers is called from the super constructor
    @Override
    void initBuffers() throws IOException {
        availableBuffers = new ArrayBlockingQueue<>(NBUFFERS);
        exceptions = new LinkedBlockingQueue<>();
        for (int n = 0; n < NBUFFERS; n++) {
            ByteBuffer bb = cache.allocateDirect(500_000);
            bb.order(ByteOrder.BIG_ENDIAN);
            availableBuffers.add(bb);
        }
        currentBuffer = waitForBuffer();
    }

    @Override
    public void close() throws IOException {
        if (isOpen) {
            isOpen = false;
            flush();
            availableBuffers.offer(currentBuffer);
            // Wait for any outstand asynch operations to complete
            while (availableBuffers.size() < NBUFFERS) {
                availableBuffers.offer(waitForBuffer());
            }
            // Free all buffers
            for (int i = 0; i < NBUFFERS; i++) {
                cache.free(availableBuffers.remove());
            }
        }
    }

    @Override
    void flush() throws IOException {
        currentBuffer.flip();
        writer.asyncWrite(segment, currentBuffer, currentBuffer, new CompletionHandler<Integer, ByteBuffer>() {
            @Override
            public void completed(Integer result, ByteBuffer attachment) {
                availableBuffers.offer(attachment);
            }

            @Override
            public void failed(Throwable exc, ByteBuffer attachment) {
                exceptions.add(exc);
                availableBuffers.offer(attachment);
            }
        });
        currentBuffer = waitForBuffer();
        currentBuffer.clear();
    }

    private ByteBuffer waitForBuffer() throws IOException {
        try {
            ByteBuffer bb = availableBuffers.take();
            handleException();
            return bb;
        } catch (InterruptedException ex) {
            throw new InterruptedIOException("Unexpected interrupt during IO");
        }        
    }
    
    private void handleException() throws IOException {
        Throwable t = exceptions.peek();
        if (t instanceof IOException) {
            throw (IOException) t;
        } else if (t != null) {
            throw new IOException("Unexpected exception during IO", t);
        }
    }

}
