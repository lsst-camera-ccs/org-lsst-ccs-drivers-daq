package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Guider;
import org.lsst.ccs.daq.ims.Guider.ROICommon;
import org.lsst.ccs.daq.ims.Guider.ROILocation;
import org.lsst.ccs.daq.ims.Guider.SensorLocation;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.Version;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
import org.lsst.ccs.imagenaming.ImageName;
import org.lsst.ccs.utilities.image.FitsHeadersSpecificationsBuilder;
import org.lsst.ccs.utilities.image.HeaderSpecification;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 *
 * @author tonyj
 */
public class GuiderTool {

    private static final Logger LOG = Logger.getLogger(GuiderTool.class.getName());

    private Store store;
    private Guider guider;

    public GuiderTool() {
    }

    @Command(name = "connect", description = "Connect to a DAQ guider")
    public void connect(
            @Argument(name = "partition", description = "Partition name") String partition) throws DAQException {

        if (store != null) {
            store.close();
        }
        store = new Store(partition);
        guider = store.getGuider();
    }

    @Command(name = "close", description = "Close DAQ store")
    public void close() throws DAQException {
        if (store != null) {
            store.close();
            store = null;
        }
    }

    @Command(name = "stop", description = "Stop the guider")
    public Guider.Status stop() throws DAQException {
        checkStore();
        return guider.stop();
    }

    @Command(name = "pause", description = "Pause the guider")
    public Guider.Status pause() throws DAQException {
        checkStore();
        return guider.pause();
    }

    @Command(name = "resume", description = "Resume the guider")
    public Guider.Status resume() throws DAQException {
        checkStore();
        return guider.resume();
    }

    @Command(name = "sleep", description = "Sleep the guider")
    public Guider.Status sleep() throws DAQException {
        checkStore();
        return guider.sleep();
    }
    
    @Command(name = "wake", description = "Wake the guider")
    public Guider.Status wake() throws DAQException {
        checkStore();
        return guider.wake();
    }
    
    
    @Command(name = "config", description = "Get the guider config")
    public Guider.Config config() throws DAQException {
        checkStore();
        return guider.config();
    }

    @Command(name = "series", description = "Get the guider series")
    public Guider.Series series() throws DAQException {
        checkStore();
        return guider.series();
    }
    
    @Command(name = "start", description = "Start the guider")
    public Guider.Status start() throws DAQException {
        checkStore();
        List<ROILocation> locations = new ArrayList<>();
        Location R00 = Location.of("R00/RebG");
        SensorLocation sensorLocation0 = new SensorLocation(R00, 0);
        SensorLocation sensorLocation1 = new SensorLocation(R00, 1);
        locations.add(new ROILocation(sensorLocation0, 4, 100, 100));
        locations.add(new ROILocation(sensorLocation1, 5, 200, 200));
        ROICommon roi = new ROICommon(50, 50, 100, 1);
        return guider.start(roi, locations);
    }

    @Command(name = "listen", description = "Listen for guider events")
    public void listen() throws DAQException {
        checkStore();
        Thread t = new Thread(() -> {
            try {
                Location R00 = Location.of("R00/RebG");
                guider.listen(R00, 0);
            } catch (DAQException x) {
                LOG.log(Level.SEVERE, "Error in listener", x);
            }
        });
        t.start();
    }

    @Command(name = "fits", description = "Write a FITS file")
    public void fitsWrite() throws DAQException {
        
        FitsHeadersSpecificationsBuilder headerSpecBuilder = new FitsHeadersSpecificationsBuilder();
        headerSpecBuilder.addSpecFile("guider-primary.spec");
        Map<String, HeaderSpecification> headerSpecifications = headerSpecBuilder.getHeaderSpecifications();
        
        checkStore();
        ImageName imageName = new ImageName("MC_C_20230101_000001");
        FitsIntWriter.FileNamer namer = (Map<String, Object> props) -> new File(new File("."), String.format("%s_%s_%s.fits", props.get("ImageName"), props.get("RaftBay"), props.get("CCDSlot")));

        Guider.FitsWriter writer = new Guider.FitsWriter(imageName, store.getPartition(), namer, headerSpecifications);
        guider.addGuiderListener(writer);
        listen();
    }
    
    @Command(name = "version", description = "Get version info")
    public Version version() throws DAQException {
        return Store.getClientVersion();
    }

    @Command(name = "locations", description = "List configured locations")
    public LocationSet locations() throws DAQException {
        checkStore();
        return store.getConfiguredSources();
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }
}
