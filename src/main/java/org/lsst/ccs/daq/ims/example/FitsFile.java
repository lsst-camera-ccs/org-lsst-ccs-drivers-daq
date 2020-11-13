package org.lsst.ccs.daq.ims.example;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import nom.tam.fits.FitsException;
import nom.tam.fits.Header;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.readout.ReadOutParametersNew;

/**
 * Utilities for reading FITS and raw files
 *
 * @author tonyj
 */
public class FitsFile implements Comparable<FitsFile> {

    private static final Pattern DATASEC_PATTERN = Pattern.compile("\\[(\\d+):(\\d+),(\\d+):(\\d+)\\]");

    private final File file;
    private final String obsId;
    private final String raftBay;
    private final String ccdSlot;
    private final int naxis1;
    private final int naxis2;
    private final int[] datasec = new int[4];

    public FitsFile(File file, Header primary, Header image) throws FitsException {
        this.file = file;
        if (primary.containsKey("CCDSLOT")) {
            this.ccdSlot = getNonNullHeader(primary, "CCDSLOT");
            this.raftBay = getNonNullHeader(primary, "RAFTBAY");
        } else {
            // Phosim compatibility
            String chipId = getNonNullHeader(primary, "CHIPID");
            String[] split = chipId.split("_");
            this.raftBay = split[0];
            this.ccdSlot = split[1];
        }
        this.obsId = getNonNullHeader(primary, "OBSID");
        this.naxis1 = image.getIntValue("NAXIS1");
        this.naxis2 = image.getIntValue("NAXIS2");
        String dataSecString = getNonNullHeader(image, "DATASEC");
        Matcher matcher = DATASEC_PATTERN.matcher(dataSecString);
        if (!matcher.matches()) {
            throw new FitsException("Invalid datasec: " + dataSecString);
        }
        this.datasec[0] = Integer.parseInt(matcher.group(1)) - 1;
        this.datasec[1] = Integer.parseInt(matcher.group(2));
        this.datasec[2] = Integer.parseInt(matcher.group(3)) - 1;
        this.datasec[3] = Integer.parseInt(matcher.group(4));
    }

    private static String getNonNullHeader(Header header, String itemName) throws FitsException {
        String result = header.getStringValue(itemName);
        if (result == null) {
            throw new FitsException("Missing " + itemName + " in FITS header");
        }
        return result;
    }

    public File getFile() {
        return file;
    }

    public String getObsId() {
        return obsId;
    }

    public String getRaftBay() {
        return raftBay;
    }

    public String getCcdSlot() {
        return ccdSlot;
    }

    public int[] getReadOutParameters() {
        List<String> DEFAULT_NAMES = Arrays.asList(ReadOutParametersNew.DEFAULT_NAMES);

        int[] result = new int[DEFAULT_NAMES.size()];
        result[DEFAULT_NAMES.indexOf("UnderCols")] = datasec[0];
        result[DEFAULT_NAMES.indexOf("PreRows")] = datasec[2];
        result[DEFAULT_NAMES.indexOf("ReadCols")] = datasec[1] - datasec[0];
        result[DEFAULT_NAMES.indexOf("ReadRows")] = datasec[3] - datasec[2];
        result[DEFAULT_NAMES.indexOf("OverCols")] = naxis1 - result[DEFAULT_NAMES.indexOf("ReadCols")] - result[DEFAULT_NAMES.indexOf("UnderCols")];
        result[DEFAULT_NAMES.indexOf("OverRows")] = naxis2 - result[DEFAULT_NAMES.indexOf("ReadRows")] - result[DEFAULT_NAMES.indexOf("PreRows")];
        result[DEFAULT_NAMES.indexOf("OpFlags")] = datasec[0] == 3 ? 1 : 2;
        return result;
    }

    @Override
    public String toString() {
        return "FitsFile{" + "file=" + file + ", obsId=" + obsId + ", raftBay=" + raftBay + ", ccdSlot=" + ccdSlot + '}';
    }

    @Override
    public int compareTo(FitsFile other) {
        int result = this.obsId.compareTo(other.obsId);
        if (result != 0) {
            return result;
        }
        result = this.raftBay.compareTo(other.raftBay);
        if (result != 0) {
            return result;
        }
        return this.ccdSlot.compareTo(other.ccdSlot);
    }

    static class ObsId {

        private final String obsId;
        private final Map<Location, Source> sources;

        ObsId(String obsId) {
            this.obsId = obsId;
            this.sources = new TreeMap<>();
        }

        void add(FitsFile fitsFile, Path meta) throws IOException {
            String ccdSlot = fitsFile.getCcdSlot();
            Location location = Location.of(fitsFile.getRaftBay() + "/Reb" + ccdSlot.substring(1, 2));
            FitsSource source = (FitsSource) sources.get(location);
            if (source == null) {
                source = new FitsSource(location);
                sources.put(location, source);
            }
            source.add(fitsFile, meta);
        }

        void add(Location location, Path raw, Path meta) throws IOException {
            sources.put(location, new RawSource(location, raw, meta));
        }

        public String getObsId() {
            return obsId;
        }

        public Set<Location> getLocations() {
            return sources.keySet();
        }

        public Map<Location, Source> getSources() {
            return sources;
        }

        @Override
        public String toString() {
            return "ObsId{" + "obsId=" + obsId + ", sources=" + sources + '}';
        }
    }

    static class Source {

        protected final Location location;
        protected final static int[] NOMETA = new int[0];

        Source(Location location) {
            this.location = location;
        }

        public Location getLocation() {
            return location;
        }

    }

    static class RawSource extends Source {

        private final Path raw;
        private final int[] meta;

        RawSource(Location location, Path raw, Path meta) throws IOException {
            super(location);
            this.raw = raw;
            if (meta == null) {
                this.meta = NOMETA;
            } else {
                String line = Files.newBufferedReader(meta).readLine();
                this.meta = Arrays.stream(line.substring(1, line.length() - 1).split(",")).map(String::trim).mapToInt(Integer::parseInt).toArray();
            }
        }

        @Override
        public String toString() {
            return "RawSource{location=" + location + ", raw=" + raw + ", meta=" + Arrays.toString(meta) + '}';
        }

        int[] getMetaData() {
            return meta;
        }

        Path getRaw() {
            return raw;
        }

    }

    static class FitsSource extends Source {

        private final TreeMap<FitsFile, int[]> files;

        FitsSource(Location location) {
            super(location);
            files = new TreeMap<>();
        }

        private void add(FitsFile fitsFile, Path meta) throws IOException {
            int[] metaData;
            if (meta == null) {
                metaData = NOMETA;
            } else {
                try (BufferedReader reader = Files.newBufferedReader(meta)) {
                    String line = reader.readLine();
                    metaData = Arrays.stream(line.substring(1, line.length() - 1).split(",")).map(String::trim).mapToInt(Integer::parseInt).toArray();
                } catch (IOException x) {
                    throw new IOException("Error reading: "+meta);
                }
            }
            files.put(fitsFile, metaData);
        }

        public TreeMap<FitsFile, int[]> getFiles() {
            return files;
        }

        @Override
        public String toString() {
            return "FitsSource{location=" + location + ", files=" + files + '}';
        }
    }
}
