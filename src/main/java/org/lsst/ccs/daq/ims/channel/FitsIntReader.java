package org.lsst.ccs.daq.ims.channel;

import java.io.File;
import java.io.IOException;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.FitsUtil;
import nom.tam.fits.Header;
import nom.tam.fits.TruncatedFileException;
import nom.tam.fits.header.Standard;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.utilities.location.Location.LocationType;

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

    public FitsIntReader(LocationType sourceType, File... files) throws IOException, TruncatedFileException {
        ReadoutConfig config = new ReadoutConfig(sourceType);
        List<String> segmentNames = Arrays.asList(config.getDataSegmentNames());
        int[] dataSegmentMap = config.getDataSegmentMap();
        int[] inverseSegmentMap = new int[dataSegmentMap.length];
        int nSegmentsPerCCD = dataSegmentMap.length;
        for (int i=0; i<dataSegmentMap.length; i++) {
            inverseSegmentMap[dataSegmentMap[i]] = i;
        }
        segments = new ArrayList<>();
        for (File file : files) {
            openFITSFile(segments, file);
        }

        ReadableIntChannel[] inputs = new ReadableIntChannel[segments.size()];
        int j = 0;
        for (Segment segment : segments) {
            FileChannel channel = segment.getChannel();
            String extName = segment.getExtensionName();
            int index = segmentNames.indexOf(extName);
            if (index < 0) {
                throw new IOException("Invalid segment name "+extName);
            }
            int channelNumber = inverseSegmentMap[index] + nSegmentsPerCCD * config.getDataSensorMap()[j / nSegmentsPerCCD];
            inputs[channelNumber] = new IntFileChannelReader(channel, segment.getSeekPosition(), segment.getDataSize());
            if (segment.getBZero() != 0) {
               inputs[channelNumber] = new SubtractingReadableIntChannel(inputs[channelNumber], segment.getBZero());
            }
            j++;
        }
        
        MultiplexingIntChannel multiplex = new MultiplexingIntChannel(inputs);
        XORReadableIntChannel xor = new XORReadableIntChannel(multiplex, config.getXor());
        input = new Compress18BitChannel(xor);
    }

    @Override
    public void close() throws IOException {
        input.close();
        for (Segment segment : segments) {
            segment.close();
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
        private final int bzero;

        public Segment(Header header, BufferedFile bf, long filePointer, String ccdSlot) {
            this.header = header;
            this.bf = bf;
            this.channel = bf.getChannel();
            this.filePointer = filePointer;
            this.ccdSlot = ccdSlot;
            nAxis1 = header.getIntValue(Standard.NAXIS1);
            nAxis2 = header.getIntValue(Standard.NAXIS2);
            bzero = header.getIntValue(Standard.BZERO);
        }
        
        private String getExtensionName() {
            return header.getStringValue(Standard.EXTNAME);
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

        private int getBZero() {
            return bzero;
        }
    }
}
