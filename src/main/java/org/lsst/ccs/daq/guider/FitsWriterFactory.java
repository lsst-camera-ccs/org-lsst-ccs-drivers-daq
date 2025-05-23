package org.lsst.ccs.daq.guider;

import org.lsst.ccs.utilities.location.SensorLocation;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionException;
import java.util.logging.Level;
import java.util.logging.Logger;

import nom.tam.fits.BasicHDU;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.FitsUtil;
import nom.tam.fits.Header;
import nom.tam.fits.HeaderCard;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
import org.lsst.ccs.imagenaming.ImageName;
import org.lsst.ccs.utilities.image.FitsCheckSum;
import org.lsst.ccs.utilities.image.FitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.image.HeaderSpecification;
import org.lsst.ccs.utilities.image.HeaderWriter;
import org.lsst.ccs.utilities.image.MetaDataSet;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.taitime.CCSTimeStamp;

/**
 * Creates a new FitsWriter when it receives a start event from the guider
 *
 * @author tonyj
 */
public class FitsWriterFactory implements GuiderListener {

    private static final Logger LOG = Logger.getLogger(FitsWriterFactory.class.getName());

    private final String partition;
    private final FitsIntWriter.FileNamer fileNamer;
    private final Map<String, HeaderSpecification> headerSpecifications;
    private FitsWriter currentFitsFileWriter;
    private final boolean includeRawStamp;
    private SeriesMetaData series;

    static {
        FitsFactory.setUseHierarch(true);
        FitsFactory.setLongStringsEnabled(true);
    }

    public FitsWriterFactory(String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications) {
        this(partition, fileNamer, headerSpecifications, false);
    }

    public FitsWriterFactory(String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications, boolean includeRawStamp) {
        this.partition = partition;
        this.fileNamer = fileNamer;
        this.headerSpecifications = headerSpecifications;
        this.includeRawStamp = includeRawStamp;
    }

    protected FitsWriter createFitsFileWriter(StateMetaData state, SeriesMetaData series, String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications) throws IOException, FitsException {
        return new FitsWriter(state, series, partition, fileNamer, headerSpecifications, Collections.emptyList());
    }

    @Override
    public void start(StateMetaData state, SeriesMetaData series) {
        this.series = series;
    }

    @Override
    public void stop(StateMetaData state) {
    }

    @Override
    public void pause(StateMetaData state) throws IOException, FitsException {
        if (currentFitsFileWriter != null) {
            // Testing on TS8 shows that closing the File can take a suprisingly long time (~1 second)
            // To avoid having the pause callback take so long, we perform the close asynchronously.
            // TODO: Get final number of DAQstamps and insert them into the header
            // int DAQStamp = state.getStamp();
            currentFitsFileWriter.closeAsync()
            .exceptionally((t) -> {
                LOG.log(Level.SEVERE, "Async close failed", t);
                return null;
            });
            currentFitsFileWriter = null;
        }
    }

    @Override
    public void resume(StateMetaData state) throws IOException, FitsException {
        currentFitsFileWriter = createFitsFileWriter(state, series, partition, fileNamer, headerSpecifications);
    }

    @Override
    public void stamp(StateMetaData state, ByteBuffer stamp) throws FitsException, IOException {
        if (currentFitsFileWriter != null) {
            currentFitsFileWriter.stamp(state, stamp);
        }
    }

    @Override
    public void rawStamp(StateMetaData state, ByteBuffer rawStamp) throws FitsException, IOException {
        if (currentFitsFileWriter != null && includeRawStamp) {
            currentFitsFileWriter.rawStamp(state, rawStamp);
        }
    }

    public static class FitsWriter implements AutoCloseable {

        private final BufferedFile bufferedFile;
        private final Map<String, Object> properties;
        private final Map<String, HeaderSpecification> headerSpecifications;
        private final String obsid;
        private final Object finalFileName;
        private final File temporaryFileName;
        private int stampCount = 0;
        private int rawStampCount = 0;
        private final BasicHDU<?> primary;
        private int lastDAQStamp;

        public FitsWriter(StateMetaData state, SeriesMetaData series, String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications, List<FitsHeaderMetadataProvider> metaDataProviders) throws IOException, FitsException {
            // The OBSID used to be provided in series meta-data, but now in state meta-data comment.
            String seriesId = series.getId();
            String comment = state.getComment();
            this.obsid = comment == null ? seriesId : comment;
            if (this.obsid == null) {
                throw new IOException("Missing OBSID");
            }
            Map<String, Object> props = new HashMap<>();
            try {
                ImageName imageName = new ImageName(obsid);
                props.put("ImageName", imageName.toString());
                props.put("ImageDate", imageName.getDateString());
                props.put("ImageNumber", imageName.getNumberString());
                props.put("ImageController", imageName.getController().getCode());
                props.put("ImageSource", imageName.getSource().getCode());
            } catch (IllegalArgumentException x) {
                throw new IOException("Bad OBSID, rejected: "+obsid, x);
            }
            ROILocation roiLocation = series.getLocation();
            SensorLocation sensorLocation = roiLocation.getLocation();
            Location rebLocation = sensorLocation.getRebLocation();
            ROICommonExtended common = series.getCommon();
            props.put("FileCreationTime", CCSTimeStamp.currentTime());
            props.put("RaftBay", rebLocation.getRaftName());
            props.put("RebSlot", rebLocation.getBoardName());
            props.put("DAQPartition", partition);
            props.put("IntegrationTime", common.getIntegrationTimeMillis());
            props.put("ROISegment", String.format("Segment%02d", roiLocation.getSegment()));
            props.put("ROIStartCol", roiLocation.getStartCol());
            props.put("ROIStartRow", roiLocation.getStartRow());
            props.put("ROICols", common.getCols());
            props.put("ROIRows", common.getRows());
            props.put("Firmware", String.format("%x", series.getFirmware()));
            props.put("CCDControllerSerial", String.format("%x", series.getSerialNumber() & 0xFFFFFFFFL));
            props.put("DAQVersion", series.getVersion().toString());
            props.put("Platform", series.getPlatform());
            props.put("ROISplit", series.isSplitROI());
            props.put("ROIUnderCols", common.getUnderCols());
            props.put("ROIOverCols", common.getOverCols());
            props.put("ROIOverRows", common.getOverRows());
            props.put("ROIFlushCount", common.getFlushCount());
            props.put("ROICCDType", series.getCcdType());
            props.put("StartTime", state.getTimestamp());
            props.put("DAQSequence", state.getSequence());
            props.put("CCDSlot", rebLocation.getSensorName(sensorLocation.getSensor()));
            props.put("StampCount", 0);
            File computedFileName = fileNamer.computeFileName(props);
            this.finalFileName = props.get("OriginalFileName");
            this.temporaryFileName = computedFileName;
            this.headerSpecifications = headerSpecifications;
            // Open the file and write the primary header
            BufferedFile bf = new BufferedFile(computedFileName, "rw");
            primary = BasicHDU.getDummyHDU();
            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("primary", props);
            for (FitsHeaderMetadataProvider provider : metaDataProviders) {
                metaDataSet.addMetaDataSet(provider.getPrimaryHeaderMetadata());
            }
            HeaderWriter.addMetaDataToHeader(computedFileName, primary, headerSpecifications.get("primary"), metaDataSet);
            FitsCheckSum.setChecksum(primary);
            primary.write(bf);
            this.bufferedFile = bf;
            this.properties = props;
        }

        private void fixupStampCount() throws IOException, FitsException {
            // DAQStamp is zero based
            if (stampCount != lastDAQStamp+1) {
                LOG.log(Level.WARNING, "{2}: DAQ stamp count {0} not equal to number of stamps received {1}", new Object[]{lastDAQStamp+1, stampCount, obsid});
            }
            Header header = primary.getHeader();
            HeaderCard stampsCard = header.findCard("N_STAMPS");
            stampsCard.setValue(stampCount);
            FitsCheckSum.setChecksum(primary);
            bufferedFile.seek(0);
            primary.write(bufferedFile);
        }

        public CompletableFuture<Void> closeAsync() {
            return CompletableFuture.runAsync(() -> {
                try {
                    close();
                } catch (IOException | FitsException x) {
                    throw new CompletionException(x);
                }
            });
        }

        @Override
        public void close() throws IOException, FitsException {
            long start = System.nanoTime();
            fixupStampCount();
            long start2 = System.nanoTime();
            bufferedFile.close();
            long stop = System.nanoTime();
            LOG.log(Level.INFO, "Closing file {0} took {1}ns fixupStampCount took {2}ns", new Object[]{temporaryFileName, stop-start, start2-start});
        }

        private void stamp(StateMetaData state, ByteBuffer stamp) throws FitsException, IOException {
            Map<String, Object> props = new HashMap<>();
            props.put("StampTime", state.getTimestamp());
            lastDAQStamp =  state.getStamp();
            props.put("DAQStamp", state.getStamp());
            props.put("StampCount",++stampCount); // 1 based count used for EXTVER
            props.put("ExtName", "IMAGE");
            int[][] intDummyData = new int[1][1];
            BasicHDU imageHDU = FitsFactory.hduFactory(intDummyData);
            Header header = imageHDU.getHeader();
            header.setXtension("IMAGE");
            header.setBitpix(32);
            header.setNaxes(2);
            // Which is naxis1 vs naxis 2? For science images we use
            // extendedMetadata.addMetaData("extension", "NAXIS1", images.getSerialPixels());
            // extendedMetadata.addMetaData("extension", "NAXIS2", images.getParallelPixels());
            // I think cols = serial, rows = parallel, so
            header.setNaxis(1, (int) properties.get("ROICols"));
            header.setNaxis(2, (int) properties.get("ROIRows"));
            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("stamp", props);
            HeaderWriter.addMetaDataToHeader(null, imageHDU, headerSpecifications.get("stamp"), metaDataSet);
            FitsCheckSum.setChecksum(imageHDU);
            //long computeChecksum = FitsCheckSum.computeChecksum(stamp);
            //FitsCheckSum.updateCheckSum(header, computeChecksum);
            long imageSize = stamp.remaining();
            header.write(bufferedFile);
            bufferedFile.getChannel().write(stamp);
            FitsUtil.pad(bufferedFile, imageSize);
        }

        private void rawStamp(StateMetaData state, ByteBuffer rawStamp) throws FitsException, IOException {
            Map<String, Object> props = new HashMap<>();
            props.put("StampTime", state.getTimestamp());
            props.put("DAQStamp", state.getStamp());
            props.put("StampCount",++rawStampCount); // 1 based count used for EXTVER
            props.put("ExtName", "RAWSTAMP");


            int[][] intDummyData = new int[1][1];
            BasicHDU binaryTableHDU = FitsFactory.hduFactory(intDummyData);
            Header header = binaryTableHDU.getHeader();
            header.setXtension("BINTABLE");
            header.setBitpix(8);
            header.setNaxes(2);
            header.setNaxis(1, rawStamp.remaining());
            header.setNaxis(2, 1);
            header.addValue("PCOUNT", 0,"");
            header.addValue("GCOUNT", 1, "");
            header.addValue("TFIELDS", 1, "");
            header.addValue("TFORM1", (rawStamp.remaining()/4)+"J", "");
            header.addValue("TTYPE1", "rawStamp", "");

            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("stamp", props);
            HeaderWriter.addMetaDataToHeader(null, binaryTableHDU, headerSpecifications.get("stamp"), metaDataSet);
            FitsCheckSum.setChecksum(binaryTableHDU);
            //long computeChecksum = FitsCheckSum.computeChecksum(rawStamp);
            //FitsCheckSum.updateCheckSum(header, computeChecksum);
            long imageSize = rawStamp.remaining();
            header.write(bufferedFile);
            bufferedFile.getChannel().write(rawStamp);
            FitsUtil.pad(bufferedFile, imageSize);
        }

        public String getImageName() {
            return obsid;
        }

        public File getFileName() {
            return temporaryFileName;
        }

    }

}
