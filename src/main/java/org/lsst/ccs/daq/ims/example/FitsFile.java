package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.io.IOException;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import nom.tam.fits.FitsException;
import nom.tam.fits.Header;
import org.lsst.ccs.daq.ims.Location;

/**
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
    private int[] datasec = new int[4];

    FitsFile(File file, Header primary) throws FitsException {
        this.file = file;
        this.ccdSlot = getNonNullHeader(primary, "CCDSLOT");
        this.raftBay = getNonNullHeader(primary, "RAFTBAY");
        this.obsId = getNonNullHeader(primary, "OBSID");
        this.naxis1 = primary.getIntValue("NAXIS1");
        this.naxis2 = primary.getIntValue("NAXIS2");
        String dataSecString = getNonNullHeader(primary, "DATSEC");
        Matcher matcher = DATASEC_PATTERN.matcher(dataSecString);
        if (!matcher.matches()) {
            throw new FitsException("Invalid datasec: " + dataSecString);
        }
        this.datasec[0] = Integer.parseInt(matcher.group(1)) - 1;
        this.datasec[1] = Integer.parseInt(matcher.group(2));
        this.datasec[2] = Integer.parseInt(matcher.group(3)) - 1;
        this.datasec[3] = Integer.parseInt(matcher.group(4));
    }

    private static String getNonNullHeader(Header primary, String name) throws FitsException {
        String result = primary.getStringValue(name);
        if (result == null) {
            throw new FitsException("Missing " + name + " in FITS header");
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
        // TODO: This works for science rafts, and the old meta-data scheme.
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
        result[REG_PRE_ROWS] = datasec[0];
        result[REG_PRE_COLS] = datasec[2];
        result[REG_READ_ROWS] = datasec[1] - datasec[0];
        result[REG_READ_COLS] = datasec[3] - datasec[2];
        result[REG_OVER_ROWS] = naxis1 - result[REG_READ_ROWS] - result[REG_PRE_ROWS];
        result[REG_OVER_COLS] = naxis2 - result[REG_READ_COLS] - result[REG_PRE_COLS];
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
            Source source = sources.get(location);
            if (source == null) {
                source = new Source(location);
                sources.put(location, source);
            }
            source.add(fitsFile);
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

        private final Location location;
        private final SortedSet<FitsFile> files;

        Source(Location location) {
            this.location = location;
            files = new TreeSet<>();
        }

        private void add(FitsFile fitsFile) {
            files.add(fitsFile);
        }

        public Location getLocation() {
            return location;
        }

        public SortedSet<FitsFile> getFiles() {
            return files;
        }

        @Override
        public String toString() {
            return "Source{" + "location=" + location + ", files=" + files + '}';
        }
    }
}
