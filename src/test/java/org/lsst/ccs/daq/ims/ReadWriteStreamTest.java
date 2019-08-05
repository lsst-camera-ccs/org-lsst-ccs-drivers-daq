package org.lsst.ccs.daq.ims;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.Collections;
import java.util.Random;
import java.util.zip.CRC32;
import java.util.zip.Checksum;
import org.junit.jupiter.api.AfterEach;
import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 *
 * @author tonyj
 */
public class ReadWriteStreamTest {

    private static final String TEST_PARTITION = "dev";
    private static final String TEST_FOLDER = "testFolder";
    private static final String TEST_IMAGE_NAME = "testImage";
    private static final int BUFFER_SIZE = 1024*1024;

    private Folder testFolder;
    private ByteBuffer randomData;

    @BeforeEach
    public void setUp() throws DAQException {
        // Make sure test folder exists
        Store store = new Store(TEST_PARTITION);
        testFolder = store.getCatalog().find(TEST_FOLDER);
        if (testFolder == null) {
            testFolder = store.getCatalog().insert(TEST_FOLDER);
        }
        Image oldImage = testFolder.find(TEST_IMAGE_NAME);
        if (oldImage != null) {
            oldImage.delete();
        }
        Random random = new Random(12345);
        randomData = ByteBuffer.allocateDirect(BUFFER_SIZE);
        for (int i = 0; i < randomData.capacity(); i++) {
            randomData.put((byte) random.nextInt(256));
//            randomData.put((byte) i);
        }
        randomData.flip();
//        for (int i = 0; i < 10; i++) {
//            System.out.printf("%d: %d\n", i, randomData.get(i));
//        }
//        for (int i = 1024*1024 - 10; i < BUFFER_SIZE; i++) {
//            System.out.printf("%d: %d\n", i, randomData.get(i));
//        }
    }

    @AfterEach
    public void tearDown() {
    }

    @Test
    public void testReadWrite() throws DAQException, IOException {
        Location location = Location.of("R22/Reb1");
        int[] registerValues = {1, 2, 3, 4, 5, 6, 7};
        ImageMetaData imd = new ImageMetaData(TEST_IMAGE_NAME, "testAnnotion", 0, Collections.singleton(location));
        Image image = testFolder.insert(imd);
        Source source = image.addSource(location, registerValues);
        long writeLength = 0;
        Checksum cksum = new CRC32();
        try (ByteChannel writeChannel = source.openChannel(Source.ChannelMode.WRITE)) {
            for (int i = 0; i < 3 * 48; i++) {
                updateChecksum(cksum, randomData);
                writeLength += writeChannel.write(randomData);
                randomData.rewind();
            }
        }
        assertEquals(3 * 48 * randomData.capacity(), writeLength);
        assertEquals(2944118306L, cksum.getValue());
//        Image image = testFolder.find(TEST_IMAGE_NAME);
        Source updatedSource = image.findSource(location);
//        assertEquals(writeLength, updatedSource.getMetaData().getLength());
        long readLength = 0;
        ByteBuffer readData = ByteBuffer.allocateDirect(BUFFER_SIZE);
        Checksum readCksum = new CRC32();
        try (ByteChannel readChannel = updatedSource.openChannel(Source.ChannelMode.READ)) {
            for (;;) {
//                for (int i=0; i<readData.capacity();i++) {
//                    readData.put(i,(byte)-1);
//                }
                readData.clear();
                int l = readChannel.read(readData);
//                System.out.println("read=" + l);
                if (l < 0) {
                    break;
                }
                readData.flip();
                assertEquals(0,readData.position());
                assertEquals(BUFFER_SIZE, readData.limit());
//                for (int i=0; i<readData.limit(); i++) {
//                    assertEquals(randomData.get(i), readData.get(i),"At "+(readLength+i));
//                }
                readLength += l;
                updateChecksum(readCksum, readData);
//                for (int i = 0; i < 10; i++) {
//                    System.out.printf("%d: %d\n", i, readData.get(i));
//                }
//                for (int i = 1024*1024 - 10; i < BUFFER_SIZE; i++) {
//                    System.out.printf("%d: %d\n", i, readData.get(i));
//                }
            }
        }
        assertEquals(3 * 48 * randomData.capacity(), readLength);
        assertEquals(2944118306L, readCksum.getValue());
    }

    private static void updateChecksum(Checksum cksum, ByteBuffer data) {
        while (data.hasRemaining()) {
            cksum.update(data.get());
        }
        data.rewind();
//        System.out.printf("cksum : %,d %,d\n", data.remaining(), cksum.getValue());
    }
}
