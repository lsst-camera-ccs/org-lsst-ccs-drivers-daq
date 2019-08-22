package org.lsst.ccs.daq.ims.channel;

import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.util.Random;
import java.util.zip.CRC32;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 *
 * @author tonyj
 */
public class IntFileChannelReaderTest {

    private static File tempFile;
    private static long checkSum;

    @BeforeClass
    public static void setUpClass() throws IOException {
        tempFile = Files.createTempFile("test", "data").toFile();
        tempFile.deleteOnExit();
        ByteBuffer bb = ByteBuffer.allocateDirect(100000);
        IntBuffer ib = bb.asIntBuffer();
        Random random = new Random(12345);
        while (ib.hasRemaining()) {
            ib.put(random.nextInt(1000000));
        }
        bb.limit(ib.position() * 4);
        try (FileChannel fc = new FileOutputStream(tempFile).getChannel()) {
            fc.write(bb);
        }
        bb.rewind();
        CRC32 crc32 = new CRC32();
        crc32.update(bb);
        checkSum = crc32.getValue();
        assertEquals(100000, tempFile.length());
    }

    @Test
    public void readTest1() throws FileNotFoundException, IOException {
        try (FileChannel fc = new FileInputStream(tempFile).getChannel()) {
            ByteBuffer bb = ByteBuffer.allocateDirect(9000);
            IntBuffer ib = bb.asIntBuffer();
            CRC32 crc32 = new CRC32();

            try (MemoryMappedIntFileReader mmifr = new MemoryMappedIntFileReader(fc, 0, 100000)) {
                for (;;) {
                    ib.clear();
                    final int read = mmifr.read(ib);
                    if (read < 0) {
                        break;
                    }
                    bb.position(0);
                    bb.limit(ib.position() * 4);
                    crc32.update(bb);
                }
            }

            assertEquals(checkSum, crc32.getValue());
        }
    }

    @Test
    public void readTest2() throws FileNotFoundException, IOException {
        try (FileChannel fc = new FileInputStream(tempFile).getChannel()) {
            ByteBuffer bb = ByteBuffer.allocateDirect(9000);
            IntBuffer ib = bb.asIntBuffer();
            CRC32 crc32 = new CRC32();

            try (IntFileChannelReader mmifr = new IntFileChannelReader(fc, 0, 100000)) {
                for (;;) {
                    ib.clear();
                    final int read = mmifr.read(ib);
                    if (read < 0) {
                        break;
                    }
                    bb.position(0);
                    bb.limit(ib.position() * 4);
                    crc32.update(bb);
                }
            }

            assertEquals(checkSum, crc32.getValue());
        }
    }

    @Test
    public void readTest3() throws FileNotFoundException, IOException {
        try (FileChannel fc = new FileInputStream(tempFile).getChannel()) {
            ByteBuffer bb = ByteBuffer.allocateDirect(9000);
            CRC32 crc32 = new CRC32();

            try (MemoryMappedIntFileReader mmifr = new MemoryMappedIntFileReader(fc, 0, 100000)) {
                for (;;) {
                    bb.clear();
                    while (bb.hasRemaining()) {
                        final int read = mmifr.read();
                        bb.putInt(read);
                    }
                    bb.flip();
                    crc32.update(bb);
                }
            } catch (EOFException x) {
                bb.flip();
                crc32.update(bb);
            }

            assertEquals(checkSum, crc32.getValue());
        }
    }

    @Test
    public void readTest4() throws FileNotFoundException, IOException {
        try (FileChannel fc = new FileInputStream(tempFile).getChannel()) {
            ByteBuffer bb = ByteBuffer.allocateDirect(9000);
            CRC32 crc32 = new CRC32();

            try (IntFileChannelReader mmifr = new IntFileChannelReader(fc, 0, 100000)) {
                for (;;) {
                    bb.clear();
                    while (bb.hasRemaining()) {
                        final int read = mmifr.read();
                        bb.putInt(read);
                    }
                    bb.flip();
                    crc32.update(bb);
                }
            } catch (EOFException x) {
                bb.flip();
                crc32.update(bb);
            }

            assertEquals(checkSum, crc32.getValue());
        }
    }
}
