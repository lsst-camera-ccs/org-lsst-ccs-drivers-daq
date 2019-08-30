package org.lsst.ccs.daq.ims.example;

import org.lsst.ccs.daq.ims.channel.XORWritableIntChannel;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.ByteChannel;
import java.nio.file.FileSystems;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.PathMatcher;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.zip.CRC32;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.Header;
import nom.tam.fits.TruncatedFileException;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageMetaData;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Source.ChannelMode;
import org.lsst.ccs.daq.ims.SourceMetaData;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.Utils;
import org.lsst.ccs.daq.ims.channel.Decompress18BitChannel;
import org.lsst.ccs.daq.ims.channel.DemultiplexingIntChannel;
import org.lsst.ccs.daq.ims.channel.FitsIntReader;
import org.lsst.ccs.daq.ims.channel.FitsWriteChannel;
import org.lsst.ccs.daq.ims.channel.WritableIntChannel;
import org.lsst.ccs.daq.ims.example.FitsFile.ObsId;
import org.lsst.ccs.utilities.ccd.CCD;
import org.lsst.ccs.utilities.ccd.CCDType;
import org.lsst.ccs.utilities.ccd.CCDTypeUtils;
import org.lsst.ccs.utilities.ccd.FocalPlane;
import org.lsst.ccs.utilities.ccd.Reb;
import org.lsst.ccs.utilities.image.FitsFileWriter;
import org.lsst.ccs.utilities.image.FitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.image.FitsHeadersSpecificationsBuilder;
import org.lsst.ccs.utilities.image.ImageSet;
import org.lsst.ccs.utilities.readout.GeometryFitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.readout.PropertiesFitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.readout.ReadOutImageSet;
import org.lsst.ccs.utilities.readout.ReadOutParameters;
import org.lsst.ccs.utilities.readout.ReadOutParametersBuilder;

/**
 *
 * @author tonyj
 */
public class CommandTool {

    private static final Pattern PATH_PATTERN = Pattern.compile("([0-9a-zA-Z\\-\\_]*)/?([0-9a-zA-Z\\-\\_]*)");

    private static final FitsHeadersSpecificationsBuilder HEADER_SPEC_BUILDER = new FitsHeadersSpecificationsBuilder();

    static {
        FitsFactory.setUseHierarch(true);
        HEADER_SPEC_BUILDER.addSpecFile("primary.spec");
        HEADER_SPEC_BUILDER.addSpecFile("extended.spec");
    }

    private Store store;

    private final FocalPlane focalPlane = FocalPlane.createFocalPlane();

    public CommandTool() {
        // TODO: Temporary fix
        CCDTypeUtils.changeCCDTypeForGeometry(focalPlane.getChild(2, 2), CCDType.getCCDType("itl"));
    }

    @Command(name = "connect", description = "Connect to a DAQ store")
    public void connect(@Argument(name = "partition", description = "Partition name") String partition) throws DAQException {
        if (store != null) {
            store.close();
        }
        store = new Store(partition);
    }

    @Command(name = "close", description = "Close DAQ store")
    public void close() throws DAQException {
        if (store != null) {
            store.close();
            store = null;
        }
    }

    @Command(name = "ls", description = "List folders/files")
    public void list(@Argument(name = "folder", description = "Path", defaultValue = "") String path) throws DAQException {
        checkStore();
        Matcher matcher = PATH_PATTERN.matcher(path);
        if (!matcher.matches()) {
            throw new RuntimeException("Illegal path: " + path);
        } else if (path.trim().isEmpty()) {
            List<Folder> list = store.getCatalog().list();
            Collections.sort(list);
            list.forEach((folder) -> {
                System.out.println(folder.getName());
            });
            long capacity = store.getCapacity();
            long remaining = store.getRemaining();
            System.out.printf("%s/%s (%3.3g%%) bytes used\n", Utils.humanReadableByteCount(capacity - remaining, false), Utils.humanReadableByteCount(capacity, false), 100.0 * (capacity - remaining) / capacity);
        } else if (matcher.matches() && matcher.group(2).isEmpty()) {
            Folder folder = store.getCatalog().find(matcher.group(1));
            if (folder == null) {
                throw new RuntimeException("No such folder: " + matcher.group(1));
            }
            List<Image> images = folder.listImages();
            Collections.sort(images);
            for (Image image : images) {
                System.out.printf("%s %s %s %s\n", image.getMetaData().getName(), imageSize(image), image.getMetaData().getTimestamp(), image.getMetaData().getAnnotation());
            }
        } else {
            Image image = imageFromPath(matcher);
            System.out.printf("%s %s %s %s\n", image.getMetaData().getName(), imageSize(image), image.getMetaData().getTimestamp(), image.getMetaData().getAnnotation());
            List<Source> sources = image.listSources();
            Collections.sort(sources);
            for (Source source : sources) {
                SourceMetaData smd = source.getMetaData();
                System.out.printf("   %s %s %s\n", smd.getLocation(), Utils.humanReadableByteCount(smd.getLength(), false),
                        Arrays.toString(smd.getRegisterValues()));
            }
        }
    }

    @Command(name = "mkdir", description = "Create new folder")
    public void mkdir(String folderName) throws DAQException {
        checkStore();
        store.getCatalog().insert(folderName);
    }

    @Command(name = "rmdir", description = "Delete folder")
    public void rmdir(String folderName) throws DAQException {
        checkStore();
        store.getCatalog().remove(folderName);
    }

    @Command(name = "rm", description = "Delete image")
    public void rm(String path) throws DAQException {
        checkStore();
        Image image = imageFromPath(path);
        image.delete();
    }

    @Command(name = "mv", description = "Move image")
    public void mv(String path, String targetFolderName) throws DAQException {
        checkStore();
        Image image = imageFromPath(path);
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: " + target);
        }
        image.moveTo(targetFolderName);
    }

    @Command(name = "read", description = "Read and decode data in image")
    public void read(String path,
            @Argument(defaultValue = "1048576") int bufferSize) throws DAQException, IOException, FitsException {
        checkStore();
        Image image = imageFromPath(path);
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ByteBuffer buffer = ByteBuffer.allocateDirect(bufferSize);
        long totalReadSize = 0;
        CRC32 cksum = new CRC32();
        long start = System.nanoTime();
        for (Source source : sources) {
            int ccdCount = source.getSourceType().getCCDCount();
            SourceMetaData smd = source.getMetaData();
            // Note, we are not using a single map for both the FileNameProperties and
            // for writing FITS file headers
            Map<String, Object> props = new HashMap<>();
            // TOOD: DO we need to handle the case where the image name does not match
            // the LSST conventions?
//            ImageName in = new ImageName(source.getImage().getMetaData().getName());
//            props.put("ImageName", in.toString());
//            props.put("ImageDate", in.getDateString());
//            props.put("ImageNumber", in.getNumberString());
//            props.put("ImageController", in.getController().getCode());
//            props.put("ImageSource", in.getSource().getCode());
            props.put("ImageName", source.getImage().getMetaData().getName());
            props.put("FileCreationTime", new Date());
            props.put("Tag", source.getImage().getMetaData().getId());
            props.put("RaftName", source.getLocation().getRaftName());
            props.put("RebName", source.getLocation().getBoardName());
            props.put("Firmware", smd.getFirmware());
            props.put("Platform", smd.getPlatform());
            props.put("SerialNumber", smd.getSerialNumber());
            props.put("DAQVersion", smd.getSoftware().toString());

            Reb reb = focalPlane.getReb(source.getLocation().getRaftName() + "/" + source.getLocation().getBoardName());

            PropertiesFitsHeaderMetadataProvider propsFitsHeaderMetadataProvider = new PropertiesFitsHeaderMetadataProvider(props);
            // Open the FITS files (one per CCD) and write headers.
            File[] files = new File[ccdCount];
            FitsFileWriter[] writers = new FitsFileWriter[ccdCount];
            try {
                // TODO: 16 should not be hard-wired here
                WritableIntChannel[] fileChannels = new WritableIntChannel[ccdCount * 16];
                for (int i = 0; i < ccdCount; i++) {
                    props.put("SensorName", source.getLocation().getSensorName(i));
                    files[i] = new File(String.format("%s_%s_%s.fits", props.get("ImageName"), props.get("RaftName"), props.get("SensorName")));
                    //files[i] = config.getFitsFile(props);
                    CCD ccd = reb.getCCDs().get(i);
                    //If the type of the CCD needs to be changed, use CCDTypeUtils::changeCCDTypeForGeometry
                    // TODO: Readout parameters are currently hard-wired to old meta-data convention
                    ReadOutParameters readoutParameters = ReadOutParametersBuilder.create(ccd.getType(), smd.getRegisterValues()).build();
                    ImageSet imageSet = new ReadOutImageSet(ccd, readoutParameters);

                    List<FitsHeaderMetadataProvider> providers = new ArrayList<>();
                    //providers.add(rebNode.getFitsService().getFitsHeaderMetadataProvider(ccd.getUniqueId()));
                    providers.add(new GeometryFitsHeaderMetadataProvider(ccd, readoutParameters));
                    providers.add(propsFitsHeaderMetadataProvider);
//                    ImageSet imageSet = new DefaultImageSet(16, 512 + 64, 2048);
                    writers[i] = new FitsFileWriter(files[i], imageSet, HEADER_SPEC_BUILDER.getHeaderSpecifications(), providers);

                    //TO-DO: use imageSet.getNumberOfImages() rather than hardwiring 16?
                    for (int j = 0; j < 16; j++) {
                        fileChannels[i * 16 + j] = new FitsWriteChannel(writers[i], j);
                    }
                }
                try (ByteChannel channel = source.openChannel(Source.ChannelMode.READ);
                        DemultiplexingIntChannel demultiplex = new DemultiplexingIntChannel(fileChannels);
                        XORWritableIntChannel xor = new XORWritableIntChannel(demultiplex, 0x1fff);
                        Decompress18BitChannel decompress = new Decompress18BitChannel(xor)) {
                    for (;;) {
                        buffer.clear();
                        int l = channel.read(buffer);
                        if (l < 0) {
                            break;
                        }
                        totalReadSize += l;
                        buffer.flip();
                        cksum.update(buffer);
                        buffer.rewind();
                        int leftover = buffer.remaining() % (4 * 9);
                        if (leftover != 0) {
                            buffer.limit(buffer.position() + buffer.remaining() - leftover);
                            decompress.write(buffer.asIntBuffer());
                            buffer.limit(buffer.limit() + leftover);
                            buffer.compact();
                        } else {
                            decompress.write(buffer.asIntBuffer());
                            buffer.clear();
                        }
                    }
                }
            } finally {
                for (FitsFileWriter writer : writers) {
                    if (writer != null) {
                        writer.close();
                    }
                }
            }
        }
        long stop = System.nanoTime();
        System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));
        System.out.printf("Checksum %,d\n", cksum.getValue());
    }

    @Command(name = "write", description = "Write a set of FITS files to the store")
    public void write(String targetFolderName, File dir,
            @Argument(defaultValue = "*.fits") String pattern) throws IOException, TruncatedFileException, DAQException {
        checkStore();
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: " + target);
        }
        Path dirPath = dir.toPath();
        PathMatcher matcher = FileSystems.getDefault().getPathMatcher("glob:" + pattern);
        SortedMap<String, ObsId> obsIds = new TreeMap<>();

        Files.walkFileTree(dirPath, new SimpleFileVisitor<Path>() {

            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                if (matcher.matches(dirPath.relativize(file))) {
                    try (BufferedFile bf = new BufferedFile(file.toFile(), "r")) {
                        Header primary = new Header(bf);
                        Header image = new Header(bf);
                        FitsFile ff = new FitsFile(file.toFile(), primary, image);
                        ObsId id = obsIds.get(ff.getObsId());
                        if (id == null) {
                            id = new ObsId(ff.getObsId());
                            obsIds.put(ff.getObsId(), id);
                        }
                        id.add(ff);

                    } catch (FitsException x) {
                        throw new IOException("Error reading FITS file: " + file, x);
                    }
                }
                return FileVisitResult.CONTINUE;
            }
        });
        for (ObsId id : obsIds.values()) {
            System.out.println(id.getObsId());
            ImageMetaData meta = new ImageMetaData(id.getObsId(), "Image Annotation", 0, id.getLocations());
            Image image = target.insert(meta);
            for (FitsFile.Source ffSource : id.getSources().values()) {
                System.out.println("\t" + ffSource.getLocation());
                int[] registerValues = ffSource.getFiles().first().getReadOutParameters();
                Source source = image.addSource(ffSource.getLocation(), registerValues);
                File[] files = ffSource.getFiles().stream().map(FitsFile::getFile).toArray(File[]::new);
                try (FitsIntReader reader = new FitsIntReader(files);
                        ByteChannel channel = source.openChannel(ChannelMode.WRITE)) {
                    ByteBuffer buffer = ByteBuffer.allocateDirect(1024 * 1024);
                    IntBuffer intBuffer = buffer.asIntBuffer();
                    for (;;) {
                        intBuffer.clear();
                        int len = reader.read(intBuffer);
                        if (len < 0) {
                            break;
                        }
                        buffer.position(0);
                        buffer.limit(4 * len);
                        channel.write(buffer);
                    }
                }
            }
        }

    }

    private Image imageFromPath(String path) throws DAQException, RuntimeException {
        final Matcher matcher = PATH_PATTERN.matcher(path);
        if (!matcher.matches()) {
            throw new RuntimeException("Illegal path: " + path);
        }
        return imageFromPath(matcher);
    }

    private Image imageFromPath(Matcher matcher) throws DAQException {
        Folder folder = store.getCatalog().find(matcher.group(1));
        if (folder == null) {
            throw new RuntimeException("No such folder: " + matcher.group(1));
        }
        Image image = folder.find(matcher.group(2));
        if (image == null) {
            throw new RuntimeException("No such image: " + matcher.group(2));
        }
        return image;
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }

    private String imageSize(Image image) throws DAQException {
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.getMetaData().getLength();
        }
        return String.format("%s(%d)", Utils.humanReadableByteCount(totalSize, false), sources.size());
    }
}
