package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.stream.Collectors;
import org.lsst.ccs.daq.ims.Location;

/**
 *
 * @author tonyj
 */
public class FitsFile implements Comparable<FitsFile> {

    private final File file;
    private final String obsId;
    private final String raftBay;
    private final String ccdSlot;

    public FitsFile(File file, String obsId, String raftBay, String ccdSlot) {
        this.file = file;
        this.obsId = obsId;
        this.raftBay = raftBay;
        this.ccdSlot = ccdSlot;
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

    @Override
    public String toString() {
        return "FitsFile{" + "file=" + file + ", obsId=" + obsId + ", raftBay=" + raftBay + ", ccdSlot=" + ccdSlot + '}';
    }

    @Override
    public int compareTo(FitsFile other) {
        int result = this.obsId.compareTo(other.obsId);
        if (result != 0) return result;
        result = this.raftBay.compareTo(other.raftBay);
        if (result != 0) return result;
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
            Location location = Location.of(fitsFile.getRaftBay()+"/Reb"+ccdSlot.substring(1, 2));
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
            return sources.keySet().stream().collect(Collectors.toSet());
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