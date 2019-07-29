package org.lsst.ccs.daq.ims.channel;

import java.io.File;
import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.List;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.FitsUtil;
import nom.tam.fits.Header;
import nom.tam.fits.TruncatedFileException;
import nom.tam.fits.header.Standard;
import nom.tam.util.BufferedFile;

/**
 *
 * @author tonyj
 */
public class FitsIntReader implements ReadableIntChannel {

    static {
        FitsFactory.setUseHierarch(true);
    }
    private final Compress18BitChannel input;
    private final List<Segment> segments;
    
    public FitsIntReader(File... files) throws IOException, TruncatedFileException {
        segments = new ArrayList<>();
        for (File file : files) {
            openFITSFile(segments, file);
        }

        IntBufferReader[] inputs = new IntBufferReader[segments.size()];
        int j = 0;
        for (Segment segment : segments) {
            FileChannel channel = segment.getChannel();
            inputs[j++] = new IntFileChannelReader(channel, segment.getSeekPosition(), segment.getDataSize());
        }

        MultiplexingIntChannel multiplex = new MultiplexingIntChannel(inputs);
        input = new Compress18BitChannel(multiplex);
    }

    @Override
    public void close() throws IOException {
        input.close();
        for (Segment segment : segments) {
            segment.close();
        }
    }

    public static void main(String[] args) throws FitsException, IOException {
        try (FitsIntReader reader = new FitsIntReader(
                new File("/home/tonyj/Data/pretty/10_Flat_screen_0000_20190322172301.fits"),
                new File("/home/tonyj/Data/pretty/11_Flat_screen_0000_20190322172301.fits"),
                new File("/home/tonyj/Data/pretty/12_Flat_screen_0000_20190322172301.fits")
        )) {
            IntBuffer buffer = IntBuffer.allocate(1000000);
            int i = 0;
            for (;;) {
                int len = reader.read(buffer);
                if (len<0) break;
                System.out.println(buffer.remaining());
                i += len;
            }
            System.out.printf("End of file after %,d bytes\n", i * 4);
        } 
    }

    private static void openFITSFile(List<Segment> segments, File file) throws IOException, TruncatedFileException {
        BufferedFile bf = new BufferedFile(file, "r");
        for (int i = 0; i < 17; i++) {
            Header header = new Header(bf);
            // Skip primary header, assumes file contains 16 image extensions
            String ccdSlot = null;
            if (i == 0) {
                ccdSlot = header.getStringValue("CCDSLOT");
            }
            if (i > 0) {
                Segment segment = new Segment(header, bf, bf.getFilePointer(), ccdSlot);
                // Skip the data (for now)
                final int dataSize = segment.getDataSize();
                int pad = FitsUtil.padding(dataSize);
                bf.skip(dataSize + pad);
                segments.add(segment);
            }
        }
    }

    @Override
    public int read() throws IOException {
        return input.read();
    }

    @Override
    public int read(IntBuffer buffer) throws IOException {
        return input.read(buffer);
    }

    @Override
    public boolean isOpen() {
        return input.isOpen();
    }

    private static class Segment {

        private final Header header;
        private final BufferedFile bf;
        private final long filePointer;
        private final String ccdSlot;
        private final int nAxis1;
        private final int nAxis2;
        private final FileChannel channel;

        public Segment(Header header, BufferedFile bf, long filePointer, String ccdSlot) {
            this.header = header;
            this.bf = bf;
            this.channel = bf.getChannel();
            this.filePointer = filePointer;
            this.ccdSlot = ccdSlot;
            nAxis1 = header.getIntValue(Standard.NAXIS1);
            nAxis2 = header.getIntValue(Standard.NAXIS2);
        }

        private int getDataSize() {
            return nAxis1 * nAxis2 * 4;
        }

        private FileChannel getChannel() {
            return channel;
        }

        private long getSeekPosition() {
            return filePointer;
        }

        private void close() throws IOException {
            channel.close();
        }
    }
}