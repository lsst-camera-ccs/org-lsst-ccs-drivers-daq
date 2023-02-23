package org.lsst.ccs.daq.guider;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;
import nom.tam.fits.BasicHDU;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.FitsUtil;
import nom.tam.fits.Header;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
import org.lsst.ccs.imagenaming.ImageName;
import org.lsst.ccs.utilities.image.FitsCheckSum;
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

    private final String partition;
    private final FitsIntWriter.FileNamer fileNamer;
    private final Map<String, HeaderSpecification> headerSpecifications;
    private FitsWriter currentFitsFileWriter;

    public FitsWriterFactory(String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications) {
        this.partition = partition;
        this.fileNamer = fileNamer;
        this.headerSpecifications = headerSpecifications;
    }

    protected FitsWriter createFitsFileWriter(StateMetaData state, SeriesMetaData series, String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications) throws IOException, FitsException {
        return new FitsWriter(state, series, partition, fileNamer, headerSpecifications, null);
    }

    @Override
    public void start(StateMetaData state, SeriesMetaData series) throws IOException, FitsException {
        currentFitsFileWriter = createFitsFileWriter(state, series, partition, fileNamer, headerSpecifications);
    }

    @Override
    public void stop(StateMetaData state) throws IOException, FitsException {
        if (currentFitsFileWriter != null) {
            currentFitsFileWriter.close();
            currentFitsFileWriter = null;
        }

    }

    @Override
    public void pause(StateMetaData state) {
    }

    @Override
    public void resume(StateMetaData state) {
    }

    @Override
    public void stamp(StateMetaData state, ByteBuffer stamp) throws FitsException, IOException {
        if (currentFitsFileWriter != null) {
            currentFitsFileWriter.stamp(state, stamp);
        }
    }

    @Override
    public void rawStamp(StateMetaData state, ByteBuffer rawStamp) {
        // Ignored for now, probably forever
    }

    public static class FitsWriter implements Closeable {

        private final BufferedFile bufferedFile;
        private final Map<String, Object> properties;
        private final Map<String, HeaderSpecification> headerSpecifications;
        private final ImageName imageName;
        private final Object finalFileName;
        private final File temporaryFileName;

        public FitsWriter(StateMetaData state, SeriesMetaData series, String partition, FitsIntWriter.FileNamer fileNamer, Map<String, HeaderSpecification> headerSpecifications, MetaDataSet extraMetaData) throws IOException, FitsException {
            Map<String, Object> props = new HashMap<>();
            // ToDo: Handle non standard id
            imageName = new ImageName(series.getId());
            props.put("ImageName", imageName.toString());
            props.put("ImageDate", imageName.getDateString());
            props.put("ImageNumber", imageName.getNumberString());
            props.put("ImageController", imageName.getController().getCode());
            props.put("ImageSource", imageName.getSource().getCode());
            ROILocation roiLocation = series.getLocation();
            SensorLocation sensorLocation = roiLocation.getLocation();
            Location rebLocation = sensorLocation.getRebLocation();
            ROICommon common = series.getCommon();
            props.put("FileCreationTime", CCSTimeStamp.currentTime());
            props.put("RaftBay", rebLocation.getRaftName());
            props.put("RebSlot", rebLocation.getBoardName());
            props.put("DAQPartition", partition);
            props.put("IntegrationTime", common.getIntegrationTimeMillis());
            props.put("ROISegment", String.format("Segment%02d", roiLocation.getSegment()));
            props.put("ROICols", common.getCols());
            props.put("ROIRows", common.getRows());
            props.put("Firmware", String.format("%x", series.getFirmware()));
            props.put("CCDControllerSerial", String.format("%x", series.getSerialNumber() & 0xFFFFFFFFL));
            props.put("DAQVersion", series.getVersion().toString());
            props.put("Platform", series.getPlatform());
            props.put("StartTime", state.getTimestamp());
            props.put("DAQSequence", state.getSequence());
            props.put("CCDSlot", rebLocation.getSensorName(sensorLocation.getSensor()));
            File computedFileName = fileNamer.computeFileName(props);
            this.finalFileName = props.get("OriginalFileName");
            this.temporaryFileName = computedFileName;
            this.headerSpecifications = headerSpecifications;
            // Open the file and write the primary header
            BufferedFile bf = new BufferedFile(computedFileName, "rw");
            BasicHDU primary = BasicHDU.getDummyHDU();
            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("primary", props);
            metaDataSet.addMetaDataSet(extraMetaData);
            
            HeaderWriter.addMetaDataToHeader(computedFileName, primary, headerSpecifications.get("primary"), metaDataSet);
            FitsCheckSum.setChecksum(primary);
            primary.write(bf);
            this.bufferedFile = bf;
            this.properties = props;
        }

        @Override
        public void close() throws IOException {
            bufferedFile.close();
        }

        private void stamp(StateMetaData state, ByteBuffer stamp) throws FitsException, IOException {
            Map<String, Object> props = new HashMap<>();
            props.put("StampTime", state.getTimestamp());
            int[][] intDummyData = new int[1][1];
            BasicHDU imageHDU = FitsFactory.hduFactory(intDummyData);
            Header header = imageHDU.getHeader();
            header.setXtension("IMAGE");
            header.setBitpix(32);
            header.setNaxes(2);
            header.setNaxis(1, (int) properties.get("ROIRows"));
            header.setNaxis(2, (int) properties.get("ROICols"));
            MetaDataSet metaDataSet = new MetaDataSet();
            metaDataSet.addMetaDataMap("stamp", props);
            HeaderWriter.addMetaDataToHeader(null, imageHDU, headerSpecifications.get("stamp"), metaDataSet);
            FitsCheckSum.setChecksum(imageHDU);
            //long computeChecksum = FitsCheckSum.computeChecksum(stamp);
            //FitsCheckSum.updateDataSum(header, computeChecksum);
            long imageSize = stamp.remaining();
            header.write(bufferedFile);
            bufferedFile.getChannel().write(stamp);
            FitsUtil.pad(bufferedFile, imageSize);
        }

        public ImageName getImageName() {
            return imageName;
        }

        public File getFileName() {
            return temporaryFileName;
        }
        
    }

}
