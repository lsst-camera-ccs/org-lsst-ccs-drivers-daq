package org.lsst.ccs.daq.ims.channel;

import java.io.File;
import java.io.IOException;
import java.nio.IntBuffer;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageMetaData;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.SourceMetaData;
import org.lsst.ccs.imagenaming.ImageName;
import org.lsst.ccs.utilities.ccd.CCD;
import org.lsst.ccs.utilities.ccd.Reb;
import org.lsst.ccs.utilities.ccd.image.data.RawImageData;
import org.lsst.ccs.utilities.image.FitsFileWriter;
import org.lsst.ccs.utilities.image.FitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.image.HeaderSpecification;
import org.lsst.ccs.utilities.image.ImageSet;
import org.lsst.ccs.utilities.location.Location.LocationType;
import org.lsst.ccs.utilities.readout.GeometryFitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.readout.PropertiesFitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.readout.ReadOutImageSet;
import org.lsst.ccs.utilities.readout.ReadOutParameters;
import org.lsst.ccs.utilities.readout.ReadOutParametersBuilder;
import org.lsst.ccs.utilities.readout.ReadOutParametersNew;
import org.lsst.ccs.utilities.taitime.CCSTimeStamp;

/**
 * A writable int channel for writing a set of FITS files corresponding to a
 * single source.
 *
 * @author tonyj
 */
public class FitsIntWriter implements WritableIntChannel {

    static {
        FitsFactory.setUseHierarch(true);
    }

    private static final DateTimeFormatter DATE_FORMAT = DateTimeFormatter.ofPattern("yyyy-MM-dd").withZone(ZoneId.of("UTC"));
    private Decompress18BitChannel decompress;
    private final FitsFileWriter[] writers;
    private final File[] files;
    private final Map<String, Object> props;
    private final Reb reb;
    private int expectedDataLength = 0;
    private final ImageMetaData imageMetaData;

    /**
     * A FitsIntWriter constructor that can be called before the source
     * meta-data is available. This must be followed by a call to
     * completeInitialization.
     *
     * @param image The image for which data is to be written
     * @param reb The corresponding REB
     * @param fileNamer An interface for generating the names of the fits files
     * to write
     * @throws IOException
     */
    public FitsIntWriter(Image image, Reb reb, FileNamer fileNamer) throws IOException {
        this(image.getMetaData(), image.getStore().getPartition(), reb, fileNamer);
    }

    FitsIntWriter(ImageMetaData imageMetaData, String partition, Reb reb, FileNamer fileNamer) throws IOException {
        this.imageMetaData = imageMetaData;
        this.reb = reb;
        Location location = reb.getLocation();
        int ccdCount = location.type().getCCDCount();
        files = new File[location.type() == Location.LocationType.WAVEFRONT ? (reb.isAuxtelREB() ? 1 : 2) : ccdCount];
        writers = new FitsFileWriter[files.length];
        ReadoutConfig readoutConfig = new ReadoutConfig(location.type(), reb.isAuxtelREB());
        props = new HashMap<>();
        try {
            ImageName in = new ImageName(imageMetaData.getName());
            props.put("ImageName", in.toString());
            props.put("ImageDate", in.getDateString());
            props.put("ImageNumber", in.getNumberString());
            props.put("ImageController", in.getController().getCode());
            props.put("ImageSource", in.getSource().getCode());
        } catch (IllegalArgumentException x) {
            props.put("ImageName", imageMetaData.getName());
            props.put("ImageDate", DATE_FORMAT.format(imageMetaData.getTimestamp()));
            props.put("ImageNumber", 1);
            props.put("ImageController", "C");
            props.put("ImageSource", "MC");
        }
        props.put("FileCreationTime", CCSTimeStamp.currentTime());
        props.put("RaftBay", location.getRaftName());
        props.put("RebSlot", location.getBoardName());
        props.put("DAQPartition", partition);
        props.put("DAQFolder", imageMetaData.getCreationFolderName());
        props.put("DAQAnnotation", imageMetaData.getAnnotation());
        props.put("DAQTriggerTime", imageMetaData.getTimestamp());
        props.put("Tag", String.format("%x", imageMetaData.getId()));
        try {
            // Open the FITS files (one per CCD)
            for (int i = 0; i < files.length; i++) {
                int sensorIndex = readoutConfig.getDataSensorMap()[i];
                Map<String, Object> ccdProps = new HashMap<>();
                ccdProps.putAll(props);
                ccdProps.put("CCDSlot", reb.isAuxtelREB() ? "S00" : location.getSensorName(sensorIndex));
                // Note image handling has a horrible kludge where it modifies the ccdProps when computerFileName is called
                // to add OriginalFileName. This no longer has any effect since ccdProps are copied here
                files[i] = fileNamer.computeFileName(ccdProps);
                // Ugly workaround for problem described above
                props.put("OriginalFileName", ccdProps.getOrDefault("OriginalFileName", files[i].getName()));
                writers[i] = new FitsFileWriter(files[i]);
            }
        } catch (IOException | RuntimeException t) {
            cleanupOnError(location, t);
        }
    }

    private void cleanupOnError(Location location, Exception t) throws IOException {
        // Cleanup any files which were already opened
        for (FitsFileWriter writer : writers) {
            if (writer != null) {
                try {
                    writer.close();
                } catch (IOException x) {
                    // Silently ignore, so we continue to (try to) close other files
                }
            }
        }
        for (File file : files) {
            if (file != null) {
                // Note: If file cannot be deleted it is silently ignored
                file.delete();
            }
        }
        throw new IOException("Error writing FITS files for location " + location, t);
    }

    public final void completeInitialization(Source source, Map<String, HeaderSpecification> headerSpecifications, PerCCDMetaDataProvider extraMetaDataProvider) throws DAQException, IOException {
        SourceMetaData smd = source.getMetaData();
        props.put("Firmware", String.format("%x", smd.getFirmware()));
        props.put("Platform", smd.getPlatform());
        props.put("CCDControllerSerial", String.format("%x", smd.getSerialNumber() & 0xFFFFFFFFL));
        props.put("DAQVersion", smd.getSoftware().toString());
        //Build the ReadoutParameters
        completeInitialization(source.getSourceType(), source.getLocation(), smd.getRegisterValues(), headerSpecifications, extraMetaDataProvider);
    }

    final void completeInitialization(LocationType locationType, Location sourceLocation, int[] registerValues, Map<String, HeaderSpecification> headerSpecifications, PerCCDMetaDataProvider extraMetaDataProvider) throws DAQException, IOException {

        ReadOutParametersBuilder builder = ReadOutParametersBuilder.create();
        builder.readoutParameterValues(registerValues);
        builder.readoutParameterNames(ReadOutParametersNew.DEFAULT_NAMES);
        ReadOutParameters readoutParameters = builder.build();
        //Set the CCDType on SCIENCE rebs
        //as they are the only ones that can have different types
        if (locationType == Location.LocationType.SCIENCE) {
            reb.setCCDType(readoutParameters.getCCDType());
        }
        ReadoutConfig readoutConfig = new ReadoutConfig(locationType, reb.isAuxtelREB());
        WritableIntChannel[] fileChannels = new WritableIntChannel[locationType.getCCDCount() * 16];
        try {
            for (int i = 0; i < files.length; i++) {
                int sensorIndex = readoutConfig.getDataSensorMap()[i];
                Map<String, Object> ccdProps = new HashMap<>();
                ccdProps.putAll(props);
                ccdProps.put("CCDSlot", reb.isAuxtelREB() ? "S00" : sourceLocation.getSensorName(sensorIndex));
                // NOTE: This call may have the side effect of modifying the ccdProps
                PropertiesFitsHeaderMetadataProvider propsFitsHeaderMetadataProvider = new PropertiesFitsHeaderMetadataProvider(ccdProps);
                CCD ccd = reb.getCCDs().get(sensorIndex);
                if (reb.isAuxtelREB()) {
                    //ccd = CCD.createCCD(CCDType.getCCDType("itl"));
                } else if (!ccd.getName().equals(ccdProps.get("CCDSlot"))) {
                    throw new IOException(String.format("Geometry (%s) inconsistent with DAQ location (%s)",
                            ccd.getName(), ccdProps.get("CCDSlot")));
                }
                ImageSet imageSet = new ReadOutImageSet(Arrays.asList(readoutConfig.getDataSegmentNames()), readoutParameters);
                List<FitsHeaderMetadataProvider> providers = new ArrayList<>();
                providers.add(new GeometryFitsHeaderMetadataProvider(ccd));
                providers.add(propsFitsHeaderMetadataProvider);
                if (extraMetaDataProvider != null) {
                    providers.addAll(extraMetaDataProvider.getMetaDataProvider(ccd));
                }
                writers[i].createHDUs(imageSet, null, providers, RawImageData.BitsPerPixel.BIT32, headerSpecifications);

                int nImageExtensions = imageSet.getImageExtensionNames().size();
                expectedDataLength += nImageExtensions * RawImageData.BitsPerPixel.BIT32.bytes() * imageSet.getParallelPixels() * imageSet.getSerialPixels();

                for (int j = 0; j < nImageExtensions; j++) {
                    fileChannels[i * nImageExtensions + j] = new FitsAsyncWriteChannel(writers[i], readoutConfig.getDataSegmentNames()[readoutConfig.getDataSegmentMap()[j]]);
                }
            }
            DemultiplexingIntChannel demultiplex = new DemultiplexingIntChannel(fileChannels);
            WritableIntChannel pixelFilter = createPixelFilter(demultiplex);
            XORWritableIntChannel xor = new XORWritableIntChannel(pixelFilter, readoutConfig.getXor());
            decompress = new Decompress18BitChannel(xor);
        } catch (IOException | FitsException | RuntimeException t) {
            cleanupOnError(reb.getLocation(), t);
        }
    }

    /**
     * Create a FitsIntWriter. If an exception is thrown by the constructor any
     * files already created will be deleted.
     *
     * @param source The source for which data is to be written
     * @param reb The corresponding REB
     * @param headerSpecifications Fits header specifications
     * @param fileNamer An interface for generating the names of the fits files
     * to write
     * @param extraMetaDataProvider Additional per-ccd fits header meta-data
     * providers
     * @throws DAQException If unable to obtain information about the source.
     * @throws IOException If an error occurs while writing the file
     */
    public FitsIntWriter(Source source, Reb reb, Map<String, HeaderSpecification> headerSpecifications, FileNamer fileNamer, PerCCDMetaDataProvider extraMetaDataProvider) throws DAQException, IOException {
        this(source.getImage(), reb, fileNamer);
        completeInitialization(source, headerSpecifications, extraMetaDataProvider);
    }

    public boolean isInitialized() {
        return isOpen();
    }

    @Override
    public void write(int i) throws IOException {
        decompress.write(i);
    }

    @Override
    public void write(IntBuffer buffer) throws IOException {
        decompress.write(buffer);
    }

    @Override
    public boolean isOpen() {
        return decompress != null && decompress.isOpen();
    }

    @Override
    public void close() throws IOException {
        if (decompress != null) {
            decompress.close();
        }
        for (FitsFileWriter writer : writers) {
            if (writer != null) {
                writer.close();
            }
        }
    }

    /**
     * Access the list of files (to be) written by this instance.
     *
     * @return The list of files.
     */
    public List<File> getFiles() {
        return Collections.unmodifiableList(Arrays.asList(files));
    }

    /***
     * Get the total expected data output length (the data to be written to all segments of all FITS files)
     * @return The length
     */
    public int getExpectedDataLength() {
        return expectedDataLength;
    }

    /**
     * Allows a pixel filter to be inserted in the decompression train.
     * Typically used to check for bad pixels.
     * @param channel
     * @return
     */
    protected WritableIntChannel createPixelFilter(WritableIntChannel channel) {
        return channel;
    }

    /**
     * Interface for computing name of FITS file based on provided properties.
     */
    public static interface FileNamer {

        /*
         * Compute the name of the file, based on the passed in properties.
         * Some implementations may modify the properties passed in.
         */
        File computeFileName(Map<String, Object> props);
    }

    /**
     * Interface for providing additional CCD specific meta-data providers
     */
    public static interface PerCCDMetaDataProvider {

        List<FitsHeaderMetadataProvider> getMetaDataProvider(CCD ccd);
    }

}
