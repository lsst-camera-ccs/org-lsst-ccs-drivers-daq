package org.lsst.ccs.daq.ims;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;

/**
 *
 * @author tonyj
 */
public class Utils {

    private static final Pattern PATH_PATTERN = Pattern.compile("([0-9a-zA-Z\\-\\_]*)/?([0-9a-zA-Z\\-\\_]*)");

    public static String humanReadableByteCount(long bytes) {
        return humanReadableByteCount(bytes, true);
    }

    public static String humanReadableByteCount(long bytes, boolean si) {
        int unit = si ? 1000 : 1024;
        if (bytes < unit) {
            return bytes + " B";
        }
        int exp = (int) (Math.log(bytes) / Math.log(unit));
        String pre = (si ? "kMGTPE" : "KMGTPE").charAt(exp - 1) + (si ? "" : "i");
        return String.format("%.1f %sB", bytes / Math.pow(unit, exp), pre);
    }

    private static String listSummary(Store store) throws DAQException {
        long capacity = store.getCapacity();
        long remaining = store.getRemaining();
        return String.format("%s/%s (%3.3g%%) used", Utils.humanReadableByteCount(capacity - remaining), Utils.humanReadableByteCount(capacity), 100.0 * (capacity - remaining) / capacity);
    }

    /**
     * Utility to list the contents of the 2-day store
     *
     * @param store The store to list
     * @param path The path to list
     * @return A stream of strings corresponding to the listed items
     * @throws org.lsst.ccs.daq.ims.DAQException If something goes wrong
     */
    public static Stream<String> list(Store store, String path) throws DAQException {
        Matcher matcher = PATH_PATTERN.matcher(path);
        if (!matcher.matches()) {
            throw new RuntimeException("Illegal path: " + path);
        } else if (path.trim().isEmpty()) {
            List<Folder> list = store.getCatalog().list();
            Collections.sort(list);
            return Stream.concat(list.stream().map(f -> f.getName()), Stream.of(listSummary(store)));
        } else if (matcher.matches() && matcher.group(2).isEmpty()) {
            Folder folder = store.getCatalog().find(matcher.group(1));
            if (folder == null) {
                throw new RuntimeException("No such folder: " + matcher.group(1));
            }
            List<Image> images = folder.listImages();
            Collections.sort(images);
            return images.stream().map(image -> String.format("%s %s %s %s", image.getMetaData().getName(), imageSize(image), image.getMetaData().getTimestamp(), image.getMetaData().getAnnotation()));
        } else {
            Image image = imageFromPath(store, matcher);
            String summary = String.format("%s %s %s %s", image.getMetaData().getName(), imageSize(image), image.getMetaData().getTimestamp(), image.getMetaData().getAnnotation());
            List<Source> sources = image.listSources();
            Collections.sort(sources);
            return Stream.concat(Stream.of(summary), sources.stream().map(source -> {
                try {
                    SourceMetaData smd = source.getMetaData();
                    return String.format("   %s %s %s", smd.getLocation(), Utils.humanReadableByteCount(smd.getLength()),
                            Arrays.toString(smd.getRegisterValues()));
                } catch (DAQException x) {
                    return String.format("   Bad source %s", x.getMessage());
                }
            }));
        }

    }

    public static Image imageFromPath(Store store, String path) throws DAQException, RuntimeException {
        final Matcher matcher = PATH_PATTERN.matcher(path);
        if (!matcher.matches()) {
            throw new RuntimeException("Illegal path: " + path);
        }
        return imageFromPath(store, matcher);
    }

    public static Image imageFromPath(Store store, Matcher matcher) throws DAQException {
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

    public static String imageSize(Image image){
        try {
            List<Source> sources = image.listSources();
            long totalSize = 0;
            int nBad = 0;
            for (Source source : sources) {
                try {
                    totalSize += source.getMetaData().getLength();
                } catch (DAQException x) {
                    nBad++;
                }
            }
            return nBad == 0
                    ? String.format("%s (%d sources)", Utils.humanReadableByteCount(totalSize), sources.size())
                    : String.format("%s (%d sources (%d bad))", Utils.humanReadableByteCount(totalSize), sources.size(), nBad);
        } catch (DAQException x) {
            return "???";
        }
    }
}
