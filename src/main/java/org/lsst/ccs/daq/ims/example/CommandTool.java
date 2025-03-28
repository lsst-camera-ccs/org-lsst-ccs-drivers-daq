package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.channels.ByteChannel;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.PathMatcher;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import java.util.zip.CRC32;
import nom.tam.fits.FitsException;
import nom.tam.fits.Header;
import nom.tam.fits.TruncatedFileException;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Emulator;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.ImageListener;
import org.lsst.ccs.daq.ims.ImageMetaData;
import org.lsst.ccs.daq.ims.Playlist;
import org.lsst.ccs.daq.ims.RegisterClient;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.Source.ChannelMode;
import org.lsst.ccs.daq.ims.SourceMetaData;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.Utils;
import org.lsst.ccs.daq.ims.Version;
import org.lsst.ccs.daq.ims.channel.FitsIntReader;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
import org.lsst.ccs.daq.ims.example.FitsFile.ObsId;
import org.lsst.ccs.daq.ims.example.FitsFile.RawSource;
import org.lsst.ccs.utilities.image.direct.DirectByteBufferCache;
import org.lsst.ccs.utilities.ccd.FocalPlane;
import org.lsst.ccs.utilities.ccd.Reb;
import org.lsst.ccs.utilities.image.FitsHeadersSpecificationsBuilder;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 *
 * @author tonyj
 */
public class CommandTool {

    private static final FitsHeadersSpecificationsBuilder HEADER_SPEC_BUILDER = new FitsHeadersSpecificationsBuilder();

    static {
        HEADER_SPEC_BUILDER.addSpecFile("primary.spec");
        HEADER_SPEC_BUILDER.addSpecFile("daqv4-primary.spec", "primary");
        HEADER_SPEC_BUILDER.addSpecFile("extended.spec");
    }
    private static final Logger LOG = Logger.getLogger(CommandTool.class.getName());

    private Store store;
    private FocalPlane focalPlane = FocalPlane.createFocalPlane();
    private ImageListener imageListener;
    private final static DirectByteBufferCache byteBufferCache = DirectByteBufferCache.instance();

    public CommandTool() {
    }

    @Command(name = "connect", description = "Connect to a DAQ store")
    public void connect(
            @Argument(name = "partition", description = "Partition name") String partition,
            @Argument(name = "geometry", description = "Override default geometry", defaultValue = "") String geometry) throws DAQException {
        if (store != null) {
            store.close();
        }
        store = new Store(partition);
        if (!geometry.isEmpty()) {
            focalPlane = FocalPlane.createFocalPlane(geometry);
        } else if (partition.equals("ats") || partition.equals("lat")) {
            focalPlane = FocalPlane.createFocalPlane("AUXTEL");
        } else {
            focalPlane = FocalPlane.createFocalPlane();
        }
    }

    @Command(name = "close", description = "Close DAQ store")
    public void close() throws DAQException {
        if (store != null) {
            store.close();
            store = null;
        }
    }

    @Command(name = "list", alias = "ls", description = "List folders/files")
    public void list(@Argument(name = "folder", description = "Path", defaultValue = "") String path) throws DAQException {
        checkStore();
        Utils.list(store, path).forEach(System.out::println);
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
        Image image = Utils.imageFromPath(store, path);
        image.delete();
    }

    @Command(name = "mv", description = "Move image")
    public void mv(String path, String targetFolderName) throws DAQException {
        checkStore();
        Image image = Utils.imageFromPath(store, path);
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: " + target);
        }
        image.moveTo(targetFolderName);
    }

    @Command(name = "showPlaylist", alias="sp", description = "List contents of playlist")
    public void pl(String playlistFile) throws DAQException, IOException {
        File file = new File(playlistFile);
        checkStore();
        Emulator emulator = store.getEmulator();
        try (Playlist playlist = emulator.openPlaylist(file)) {
            List<Image> images = playlist.getImages();
            images.stream().map(image -> String.format("%s %s %s %s", image.getMetaData().getName(), Utils.imageSize(image), image.getMetaData().getTimestamp(), image.getMetaData().getAnnotation())).forEach(System.out::println);
        }
    }

    @Command(name = "addPlaylistImage", alias="ap", description = "Add an image to a playlist")
    public void pla(String playlistFile, String imagePath) throws DAQException, IOException {
        checkStore();
        Image image = Utils.imageFromPath(store, imagePath);
        Emulator emulator = store.getEmulator();
        File file = new File(playlistFile);
        try (Playlist playlist = emulator.openPlaylist(file)) {
            playlist.add(image);
        }
    }

    @Command(name = "emulators", description = "List configured emulators")
    public LocationSet emulators() throws DAQException {
        checkStore();
        Emulator emulator = store.getEmulator();
        return emulator.getLocations();
    }

    @Command(name = "play", description = "Start a playlist")
    public void play(String playlistFile, @Argument(name = "repeat", description = "Repeat", defaultValue = "false") boolean repeat) throws DAQException, IOException {
        checkStore();
        File file = new File(playlistFile);
        Emulator emulator = store.getEmulator();
        try (Playlist playlist = emulator.openPlaylist(file)) {
            playlist.start(repeat);
        }
    }

    @Command(name = "halt", description = "Halt a playlist")
    public void halt() throws DAQException {
        checkStore();
        Emulator emulator = store.getEmulator();
        emulator.halt();
    }

    @Command(name = "stop", description = "Stop a playlist")
    public void stop() throws DAQException {
        checkStore();
        Emulator emulator = store.getEmulator();
        emulator.stop();
    }

    @Command(name = "restart", description = "Restart a playlist")
    public void restart() throws DAQException {
        checkStore();
        Emulator emulator = store.getEmulator();
        emulator.restart();
    }

    @Command(name = "locations", description = "List configured locations")
    public LocationSet locations() throws DAQException {
        checkStore();
        return store.getConfiguredSources();
    }

    @Command(name = "platform", description = "Get platform name")
    public String platform() throws DAQException {
        checkStore();
        return store.getClientPlatform();
    }

    private static class ReadThread extends Thread {

        private final ByteBuffer buffer;
        private final Store store;
        private static int n = 0;

        ReadThread(Runnable r, int bufferSize, String partition) {
            super(r);
            this.setName("DAQ_read_thread_" + n++);
            this.setDaemon(true);
            buffer = byteBufferCache.allocateDirect(bufferSize);
            buffer.order(ByteOrder.LITTLE_ENDIAN);
            try {
                store = new Store(partition);
            } catch (DAQException x) {
                throw new RuntimeException("Error creating store for read thread", x);
            }
        }

        @Override
        @SuppressWarnings("CallToThreadRun")
        public void run() {
            try {
                super.run();
            } finally {
                byteBufferCache.free(buffer);
                try {
                    store.close();
                } catch (DAQException x) {
                    throw new RuntimeException("Error closing store", x);
                }
            }
        }
    }

    @Command(name = "version", description = "Get version info")
    public Version version() throws DAQException {
        return Store.getClientVersion();
    }

    @Command(name = "checkRaw")
    public void checkRaw(String path,
            @Argument(defaultValue = "1950720") int bufferSize,
            @Argument(defaultValue = "4") int nThreads) throws DAQException, IOException, FitsException, InterruptedException, ExecutionException {
        checkStore();
        Image image = Utils.imageFromPath(store, path);
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ThreadFactory readThreadFactory = (Runnable r) -> new ReadThread(r, bufferSize, image.getStore().getPartition());

        ThreadPoolExecutor executor = new ThreadPoolExecutor(nThreads, nThreads, 60L,
                TimeUnit.SECONDS, new LinkedBlockingQueue<>(), readThreadFactory);
        try {
            executor.prestartAllCoreThreads();
            List<Future<Object[]>> futures = new ArrayList<>();
            long start = System.nanoTime();
            for (Source source : sources) {
                Callable<Object[]> callable = () -> {

                    CRC32 cksum = new CRC32();
                    ReadThread thread = (ReadThread) Thread.currentThread();
                    ByteBuffer buffer = thread.buffer;
                    try (ByteChannel channel = source.openChannel(thread.store, Source.ChannelMode.READ)) {
                        long readSize = 0;
                        for (;;) {
                            int l = channel.read(buffer);
                            if (l < 0) {
                                break;
                            }
                            readSize += l;
                            buffer.flip();
                            cksum.update(buffer);
                            buffer.clear();
                        }
                        return new Object[]{source, readSize, cksum.getValue()};
                    }
                };
                futures.add(executor.submit(callable));
            }
            Map<Source, Long> checksums = new HashMap<>();
            long totalReadSize = 0;
            for (Future<Object[]> future : futures) {
                Object[] result = future.get();
                totalReadSize += (Long) result[1];
                Source source = (Source) result[0];
                checksums.put(source, (Long) result[2]);
            }
            long stop = System.nanoTime();

            System.out.printf(
                    "Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));

            List<Source> sourceList = new ArrayList<>(checksums.keySet());
            Collections.sort(sourceList);
            for (Source source : sourceList) {
                System.err.printf("Source %s crc32 %d\n", source.getLocation(), checksums.get(source));
            }

        } finally {
            executor.shutdown();
        }
    }

    @Command(name = "readRaw")
    public void readRaw(String path,
            @Argument(defaultValue = ".", description = "Folder where .raw (and .meta) files will be written") File dir,
            @Argument(defaultValue = "1950720") int bufferSize,
            @Argument(defaultValue = "4") int nThreads) throws DAQException, IOException, FitsException, InterruptedException, ExecutionException {
        checkStore();
        Image image = Utils.imageFromPath(store, path);
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ThreadFactory readThreadFactory = (Runnable r) -> new ReadThread(r, bufferSize, image.getStore().getPartition());

        ThreadPoolExecutor executor = new ThreadPoolExecutor(nThreads, nThreads, 60L,
                TimeUnit.SECONDS, new LinkedBlockingQueue<>(), readThreadFactory);
        try {
            executor.prestartAllCoreThreads();
            List<Future<Long>> futures = new ArrayList<>();
            long start = System.nanoTime();
            for (Source source : sources) {
                Callable<Long> callable = () -> {
                    // Now write .meta file
                    File metaFile = new File(dir, String.format("%s_%s.meta", source.getImage().getMetaData().getName(), source.getLocation().toString().replace("/", "_")));
                    try (PrintWriter metaWriter = new PrintWriter(metaFile)) {
                        metaWriter.println(Arrays.toString(source.getMetaData().getRegisterValues()));
                    }

                    File file = new File(dir, String.format("%s_%s.raw", source.getImage().getMetaData().getName(), source.getLocation().toString().replace("/", "_")));
                    ReadThread thread = (ReadThread) Thread.currentThread();
                    ByteBuffer buffer = thread.buffer;
                    try (ByteChannel channel = source.openChannel(thread.store, Source.ChannelMode.READ);
                            FileChannel out = new FileOutputStream(file).getChannel()) {
                        long readSize = 0;
                        for (;;) {
                            int l = channel.read(buffer);
                            if (l < 0) {
                                break;
                            }
                            readSize += l;
                            buffer.flip();
                            out.write(buffer);
                            buffer.clear();
                        }
                        return readSize;
                    }
                };
                futures.add(executor.submit(callable));
            }
            long totalReadSize = 0;
            for (Future<Long> future : futures) {
                totalReadSize += future.get();
            }
            long stop = System.nanoTime();

            System.out.printf(
                    "Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));
        } finally {
            executor.shutdown();
        }
    }

    @Command(name = "writeRaw", description = "Write a set of .raw images into the 2 day store")
    public void writeRaw(File dir,
            @Argument(defaultValue = "emu") String targetFolderName,
            @Argument(defaultValue = "(\\w+)_(R\\d\\d)_(Reb[0-2|W|G]).raw") String pattern) throws DAQException, IOException {
        checkStore();
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: " + targetFolderName);
        }
        Path dirPath = dir.toPath();
        Pattern compiled = Pattern.compile(pattern);
        SortedMap<String, ObsId> obsIds = new TreeMap<>();
        Files.walkFileTree(dirPath, new SimpleFileVisitor<Path>() {

            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                Matcher matcher = compiled.matcher(dirPath.relativize(file).getFileName().toString());
                if (matcher.matches()) {
                    // We will need to guess the obsid from the file name,
                    ObsId id = obsIds.get(matcher.group(1));
                    if (id == null) {
                        id = new ObsId(matcher.group(1));
                        obsIds.put(matcher.group(1), id);
                    }
                    Location location = Location.of(matcher.group(2) + "/" + matcher.group(3));
                    // Look for corresponding .meta file
                    Path meta = file.resolveSibling(file.getFileName().toString().replace(".raw", ".meta"));
                    if (Files.exists(meta)) {
                        id.add(location, file, meta);
                    } else {
                        id.add(location, file, null);
                    }
                }
                return FileVisitResult.CONTINUE;
            }
        });
        for (ObsId id : obsIds.values()) {
            System.out.println(id.getObsId());
            ImageMetaData meta = new ImageMetaData(id.getObsId(), "raw", "Image Annotation", 0, id.getLocations());
            Image image = target.insert(meta);
            for (FitsFile.Source rsource : id.getSources().values()) {
                RawSource rawSource = (RawSource) rsource;
                System.out.println("\t" + rawSource.getLocation());
                int[] registerValues = rawSource.getMetaData();
                Source source = image.addSource(rawSource.getLocation(), registerValues);
                Path file = rawSource.getRaw();
                try (FileChannel in = new FileInputStream(file.toFile()).getChannel();
                        ByteChannel channel = source.openChannel(ChannelMode.WRITE)) {
                    ByteBuffer buffer = byteBufferCache.allocateDirect(1950720);
                    try {
                        for (;;) {
                            buffer.clear();
                            int len = in.read(buffer);
                            if (len < 0) {
                                break;
                            }
                            buffer.flip();
                            channel.write(buffer);
                        }
                    } finally {
                        byteBufferCache.free(buffer);
                    }
                }
            }
        }
    }

    @Command(name = "listen", description = "Listen for images")
    public void listen() {
        checkStore();
        imageListener = new ImageListener() {
            @Override
            public void imageCreated(Image image) {
                System.out.println("Image created " + image.getMetaData().getName());
            }

            @Override
            public void imageComplete(Image image) {
                try {
                    System.out.println("Image complete " + image.getMetaData().getName());
                    List<Source> sources = image.listSources();
                    for (Source source : sources) {
                        SourceMetaData smd = source.getMetaData();
                        System.out.printf("Source location: %s length: %s\n", smd.getLocation(), Utils.humanReadableByteCount(smd.getLength()));
                    }
                } catch (DAQException ex) {
                    LOG.log(Level.SEVERE, "Exception in imageComplete listener", ex);
                }
            }
        };
        store.addImageListener(imageListener);
    }

    @Command(name = "unlisten", description = "Stop listen for images")
    public void unlisten() {
        checkStore();
        store.removeImageListener(imageListener);
    }

    @Command(name = "read", description = "Read and decode data in image")
    public void read(String path,
            @Argument(defaultValue = ".", description = "Folder where FITS files will be written") File dir,
            @Argument(defaultValue = "1950720") int bufferSize,
            @Argument(defaultValue = "4") int nThreads) throws DAQException, IOException, FitsException, InterruptedException, ExecutionException {
        checkStore();
        Image image = Utils.imageFromPath(store, path);
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ThreadFactory readThreadFactory = (Runnable r) -> new ReadThread(r, bufferSize, image.getStore().getPartition());

        ThreadPoolExecutor executor = new ThreadPoolExecutor(sources.size(), sources.size(), 60L,
                TimeUnit.SECONDS, new LinkedBlockingQueue<>(), readThreadFactory);
        try {
            executor.prestartAllCoreThreads();
            List<Future<Long>> futures = new ArrayList<>();
            long start = System.nanoTime();
            Semaphore semaphore = new Semaphore(nThreads);
            for (Source source : sources) {
                Callable<Long> callable = () -> {
                    ReadThread thread = (ReadThread) Thread.currentThread();
                    Reb reb = focalPlane.getReb(source.getLocation().getRaftName() + "/" + source.getLocation().getBoardName());
                    FitsIntWriter.FileNamer namer = (Map<String, Object> props) -> new File(dir, String.format("%s_%s_%s.fits", props.get("ImageName"), props.get("RaftBay"), props.get("CCDSlot")));
                    try (ByteChannel channel = source.openChannel(thread.store, Source.ChannelMode.READ);
                            FitsIntWriter decompress = new FitsIntWriter(source, reb, HEADER_SPEC_BUILDER.getHeaderSpecifications(), namer, null)) {
                        long readSize = 0;
                        ByteBuffer buffer = thread.buffer;
                        for (;;) {
                            semaphore.acquire();
                            try {
                                int l = channel.read(buffer);
                                if (l < 0) {
                                    break;
                                }
                                readSize += l;
                            } finally {
                                semaphore.release();
                            }
                            buffer.flip();
                            decompress.write(buffer.asIntBuffer());
                            buffer.clear();
                        }
                        return readSize;
                    }
                };
                futures.add(executor.submit(callable));
            }
            long totalReadSize = 0;
            for (Future<Long> future : futures) {
                totalReadSize += future.get();
            }
            long stop = System.nanoTime();
            System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));
        } finally {
            executor.shutdown();
        }
    }

    @Command(name = "write", description = "Write a set of FITS files to the store")
    public void write(String targetFolderName, File dir,
            @Argument(defaultValue = "*.fits") String pattern) throws IOException, TruncatedFileException, DAQException {
        checkStore();
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: " + targetFolderName);
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
                        Path meta = file.resolveSibling(file.getFileName().toString().replace(".fits", ".meta"));
                        if (Files.exists(meta)) {
                            id.add(ff, meta);
                        } else {
                            id.add(ff, null);
                        }
                    } catch (FitsException x) {
                        throw new IOException("Error reading FITS file: " + file, x);
                    }
                }
                return FileVisitResult.CONTINUE;
            }
        });
        for (ObsId id : obsIds.values()) {
            System.out.println(id.getObsId());
            ImageMetaData meta = new ImageMetaData(id.getObsId(), "raw", "Image Annotation", 0, id.getLocations());
            Image image = target.insert(meta);
            for (FitsFile.Source fSource : id.getSources().values()) {
                FitsFile.FitsSource ffSource = (FitsFile.FitsSource) fSource;
                Reb reb = focalPlane.getReb(ffSource.getLocation().getRaftName() + "/" + ffSource.getLocation().getBoardName());
                System.out.println("\t" + ffSource.getLocation());
                Location.LocationType locationType = reb.isAuxtelREB() ? Location.LocationType.SCIENCE : reb.getLocation().type();
                Map.Entry<FitsFile, int[]> firstEntry = ffSource.getFiles().firstEntry();
                int[] registerValues = firstEntry.getValue();
                Source source = image.addSource(reb.getLocation(), registerValues);
                File[] files = ffSource.getFiles().keySet().stream().map(FitsFile::getFile).toArray(File[]::new);
                try (FitsIntReader reader = new FitsIntReader(locationType, reb.isAuxtelREB(), files);
                        ByteChannel channel = source.openChannel(ChannelMode.WRITE)) {
                    ByteBuffer buffer = byteBufferCache.allocateDirect(1950720);
                    try {
                        buffer.order(ByteOrder.LITTLE_ENDIAN);
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
                    } finally {
                        byteBufferCache.free(buffer);
                    }
                }
            }
        }
    }

    @Command(name = "purge", description = "Purge files in a folder older than some delta to make space")
    public void purge(String folderName, String delta) throws DAQException {
        checkStore();
        Folder folder = store.getCatalog().find(folderName);
        if (folder == null) {
            throw new IllegalArgumentException("Invalid folder: " + folder);
        }
        List<Image> images = folder.listImages();
        images.sort((Image i1, Image i2) -> i1.getMetaData().getTimestamp().compareTo(i2.getMetaData().getTimestamp()));
        Instant cutOff = Instant.now().minus(Duration.parse(delta));
        for (Image image : images) {
            if (image.getMetaData().getTimestamp().isBefore(cutOff)) {
                System.out.println("Deleting: " + image.getMetaData().getName());
                image.delete();
            } else {
                break;
            }
        }
    }

    @Command(name = "readReg", description = "Read registers")
    public void readReg(int... address) throws DAQException {
        checkStore();
        RegisterClient registerClient = store.getRegisterClient();
        Map<Location, int[]> result = registerClient.readRegisters(locations(), address);
        for (Location location : locations()) {
            System.out.printf("%s: %s\n", location, Arrays.stream(result.get(location)).mapToObj(i -> String.format("%08x", i)).collect(Collectors.joining(",")));
        }
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }
}
