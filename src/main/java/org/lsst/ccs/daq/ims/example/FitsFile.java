package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import nom.tam.fits.FitsException;
import nom.tam.fits.Header;
import org.lsst.ccs.utilities.location.Location;

/**
 * Utilities for reading FITS and raw files
 *
 * @author tonyj
 */
class FitsFile implements Comparable<FitsFile> {

    private static final Pattern DATASEC_PATTERN = Pattern.compile("\\[(\\d+):(\\d+),(\\d+):(\\d+)\\]");

    private final File file;
    private final String obsId;
    private final String raftBay;
    private final String ccdSlot;
    private final int naxis1;
    private final int naxis2;
    private final int[] datasec = new int[4];

    FitsFile(File file, Header primary, Header image) throws FitsException {
        this.file = file;
        this.ccdSlot = getNonNullHeader(primary, "CCDSLOT");
        this.raftBay = getNonNullHeader(primary, "RAFTBAY");
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

    int[] getReadOutParameters() {
        // TODO: This works for science rafts, and the old meta-data scheme only.
        int REG_READ_ROWS = 0,
                REG_READ_COLS = 1,
                REG_PRE_ROWS = 2,
                REG_PRE_COLS = 3,
                REG_POST_ROWS = 4,
                REG_POST_COLS = 5,
                REG_READ_COLS2 = 6,
                REG_OVER_ROWS = 7,
                REG_OVER_COLS = 8,
                NUM_REGISTERS = 9;

        int[] result = new int[NUM_REGISTERS];
        result[REG_PRE_COLS] = datasec[0];
        result[REG_PRE_ROWS] = datasec[2];
        result[REG_READ_COLS] = datasec[1] - datasec[0];
        result[REG_READ_ROWS] = datasec[3] - datasec[2];
        result[REG_OVER_COLS] = naxis1 - result[REG_READ_COLS] - result[REG_PRE_COLS];
        result[REG_OVER_ROWS] = naxis2 - result[REG_READ_ROWS] - result[REG_PRE_ROWS];
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

        void add(FitsFile fitsFile) {
            String ccdSlot = fitsFile.getCcdSlot();
            Location location = Location.of(fitsFile.getRaftBay() + "/Reb" + ccdSlot.substring(1, 2));
            FitsSource source = (FitsSource) sources.get(location);
            if (source == null) {
                source = new FitsSource(location);
                sources.put(location, source);
            }
            source.add(fitsFile);
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
        private final static int[] NOMETA = new int[0];
        
        RawSource(Location location, Path raw, Path meta) throws IOException {
            super(location);
            this.raw = raw;
            if (meta == null) {
                this.meta = NOMETA;
            } else {
                String line = Files.newBufferedReader(meta).readLine();
                this.meta = Arrays.stream(line.substring(1,line.length()-1).split(",")).map(String::trim).mapToInt(Integer::parseInt).toArray();
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

        private final SortedSet<FitsFile> files;

        FitsSource(Location location) {
            super(location);
            files = new TreeSet<>();
        }

        private void add(FitsFile fitsFile) {
            files.add(fitsFile);
        }

        public SortedSet<FitsFile> getFiles() {
            return files;
        }

        @Override
        public String toString() {
            return "FitsSource{location=" + location + ", files=" + files + '}';
        }
    }
}
