package org.lsst.ccs.daq.ims.example;

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
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
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
import org.lsst.ccs.daq.ims.channel.FitsIntReader;
import org.lsst.ccs.daq.ims.example.FitsFile.ObsId;

/**
 *
 * @author tonyj
 */
public class CommandTool {

    private static final Pattern PATH_PATTERN = Pattern.compile("([0-9a-zA-Z\\-\\_]*)/?([0-9a-zA-Z\\-\\_]*)");

    static {
        FitsFactory.setUseHierarch(true);
    }

    private Store store;

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
            @Argument(defaultValue = "1048576") int bufferSize) throws DAQException, IOException {
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
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.READ)) {
                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                    buffer.flip();
                    cksum.update(buffer);
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
                        FitsFile ff = new FitsFile(file.toFile(), primary);
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
