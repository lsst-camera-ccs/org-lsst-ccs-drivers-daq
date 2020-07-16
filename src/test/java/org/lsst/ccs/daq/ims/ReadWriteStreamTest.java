package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.zip.CRC32;
import static org.junit.Assume.assumeNotNull;
import org.junit.Before;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertArrayEquals;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class ReadWriteStreamTest {

    private static final String TEST_FOLDER = "testFolder";
    private static final String TEST_IMAGE_NAME = "testImage";
    private static final int WRITE_BUFFER_SIZE = 1024*1024;
    private static final int READ_BUFFER_SIZE = 100000;
    private static final int NBUFFERS = 3 * 9;

    private Store store;
    private Folder testFolder;
    private ByteBuffer randomData;

    @Before
    public void setUp() throws DAQException {
        // Make sure test folder exists
        System.out.println("LD_LIBRARY_PATH="+System.getenv("LD_LIBRARY_PATH"));
        String testPartition = System.getProperty("DAQ_TEST_PARTITION");
        System.out.println("DAQ_TEST_PARTITION="+testPartition);
        // This causes the test to be skipped if asumption is not met
        assumeNotNull(testPartition);
        store = new Store(testPartition);
        testFolder = store.getCatalog().find(TEST_FOLDER);
        if (testFolder == null) {
            testFolder = store.getCatalog().insert(TEST_FOLDER);
        }
        Image oldImage = testFolder.find(TEST_IMAGE_NAME);
        if (oldImage != null) {
            oldImage.delete();
        }
        Random random = new Random(12345);
        randomData = ByteBuffer.allocateDirect(WRITE_BUFFER_SIZE);
        for (int i = 0; i < randomData.capacity(); i++) {
            randomData.put((byte) random.nextInt(256));
        }
        randomData.flip();
    }

    @Test
    public void testStream() throws DAQException, IOException, InterruptedException, ExecutionException {
        ExecutorService threadPool = Executors.newCachedThreadPool();
        final Queue<Future<long[]>> futureImages = new ArrayBlockingQueue<>(1);
        
        store.addImageListener(new ImageListener() {
            @Override
            public void imageCreated(Image image) {
               futureImages.add(threadPool.submit(new ImageStreamer(image)));
            }

            @Override
            public void imageComplete(Image image) {
            }
        });
        
        Location location = Location.of("R22/Reb1");
        int[] registerValues = {1, 2, 3, 4, 5, 6, 7};
        ImageMetaData imd = new ImageMetaData(TEST_IMAGE_NAME, "raw", "testAnnotion", 0, Collections.singleton(location));
        Image image = testFolder.insert(imd);
        Source source = image.addSource(location, registerValues);
        long writeLength = 0;
        CRC32 cksum = new CRC32();
        try (ByteChannel writeChannel = source.openChannel(Source.ChannelMode.WRITE)) {
            for (int i = 0; i < NBUFFERS; i++) {
                cksum.update(randomData);
                randomData.rewind();
                writeLength += writeChannel.write(randomData);
                randomData.rewind();
            }
        }
        assertEquals(NBUFFERS * randomData.capacity(), writeLength);
        assertEquals(4157543225L, cksum.getValue());
        
        long[] result = futureImages.remove().get();
        assertEquals(NBUFFERS * randomData.capacity(), result[0]);
        assertEquals(cksum.getValue(), result[1]);
        assertEquals(registerValues[0], result[2]);
        
        Source updatedSource = image.findSource(location);
        assertEquals(writeLength, updatedSource.getMetaData().getLength());
        assertArrayEquals(registerValues, Arrays.copyOfRange(updatedSource.getMetaData().getRegisterValues(), 0, registerValues.length));

        long readLength = 0;
        ByteBuffer readData = ByteBuffer.allocateDirect(WRITE_BUFFER_SIZE);
        CRC32 readCksum = new CRC32();
        try (ByteChannel readChannel = updatedSource.openChannel(Source.ChannelMode.READ)) {
            for (;;) {
                readData.clear();
                int l = readChannel.read(readData);
                if (l < 0) {
                    break;
                }
                readData.flip();
                assertEquals(0, readData.position());
                assertEquals(WRITE_BUFFER_SIZE, readData.limit());
                readLength += l;
                readCksum.update(readData);
            }
        }
        assertEquals(NBUFFERS * randomData.capacity(), readLength);
        assertEquals(cksum.getValue(), readCksum.getValue());
    }



    private static class ImageStreamer implements Callable<long[]> {

        private final Image image;

        ImageStreamer(Image image) {
            this.image = image;
        }

        @Override
        public long[] call() throws IOException, DAQException {
            List<Source> sources = image.listSources();
            Source source = sources.get(0);
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.STREAM)) {
                CRC32 readCksum = new CRC32();
                ByteBuffer buffer = ByteBuffer.allocateDirect(READ_BUFFER_SIZE);
                long totalReadSize = 0;
                int[] registerValues = { 99 };
                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                    buffer.flip();
                    readCksum.update(buffer);
                    // If we got here the source must be created, so we can ask for meta-data
                    registerValues = source.getMetaData().getRegisterValues();
                }
                return new long[]{totalReadSize, readCksum.getValue(), registerValues[0]};
            }
        }
    }
}
