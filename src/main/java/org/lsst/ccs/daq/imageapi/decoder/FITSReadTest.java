package org.lsst.ccs.daq.imageapi.decoder;

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;
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
public class FITSReadTest {

    static {
        FitsFactory.setUseHierarch(true);
    }

    public static void main(String[] args) throws FitsException, IOException {

        List<Segment> segments = new ArrayList<>();
        openFITSFile(segments, "/home/tonyj/Data/pretty/10_Flat_screen_0000_20190322172301.fits");
        openFITSFile(segments, "/home/tonyj/Data/pretty/11_Flat_screen_0000_20190322172301.fits");
        openFITSFile(segments, "/home/tonyj/Data/pretty/12_Flat_screen_0000_20190322172301.fits");

        List<ByteBuffer> buffers = new ArrayList<>();
        for (Segment segment : segments) {
            ByteBuffer bb = ByteBuffer.allocateDirect(segment.getDataSize());
            FileChannel channel = segment.getChannel();
            int len = channel.read(bb, segment.getSeekPosition());
            if (bb.remaining() != 0) {
                throw new IOException("Unexpected length " + len);
            }
            bb.flip();
            buffers.add(bb);
        }
        IntBufferReader[] inputs = new IntBufferReader[segments.size()];
        for (int i=0; i<inputs.length; i++) {
            inputs[i] = new IntBufferReader(buffers.get(i).asIntBuffer());
        }
        MultiplexingIntChannel multiplex = new MultiplexingIntChannel(inputs);
        Compress18BitChannel compress = new Compress18BitChannel(multiplex);
        int i = 0;
        try {
            for (;;i++) {
                compress.read();
            }
        } catch(EOFException x) {
            System.out.printf("End of file after %,d bytes",i*4);
        }
    }

    private static void openFITSFile(List<Segment> segments, String name) throws IOException, TruncatedFileException {
        BufferedFile bf = new BufferedFile(name, "r");
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

    }

}
