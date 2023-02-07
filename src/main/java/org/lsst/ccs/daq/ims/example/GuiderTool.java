package org.lsst.ccs.daq.ims.example;

import java.io.File;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.FitsWriterFactory;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Guider;
import org.lsst.ccs.daq.guider.ROICommon;
import org.lsst.ccs.daq.guider.ROILocation;
import org.lsst.ccs.daq.guider.SensorLocation;
import org.lsst.ccs.daq.guider.Series;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.daq.ims.Guider.Subscriber;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.Version;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
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
    public Status stop() throws DAQException {
        checkStore();
        return guider.stop();
    }

    @Command(name = "pause", description = "Pause the guider")
    public Status pause() throws DAQException {
        checkStore();
        return guider.pause();
    }

    @Command(name = "resume", description = "Resume the guider")
    public Status resume() throws DAQException {
        checkStore();
        return guider.resume();
    }

    @Command(name = "sleep", description = "Sleep the guider")
    public Status sleep() throws DAQException {
        checkStore();
        return guider.sleep();
    }

    @Command(name = "wake", description = "Wake the guider")
    public Status wake() throws DAQException {
        checkStore();
        return guider.wake();
    }

    @Command(name = "config", description = "Get the guider config")
    public Config config() throws DAQException {
        checkStore();
        return guider.config();
    }

    @Command(name = "series", description = "Get the guider series")
    public Series series() throws DAQException {
        checkStore();
        return guider.series();
    }

    @Command(name = "start", description = "Start the guider")
    public Status start() throws DAQException {
        checkStore();
        List<ROILocation> locations = new ArrayList<>();
        Location R00 = Location.of("R00/RebG");
        SensorLocation sensorLocation0 = new SensorLocation(R00, 0);
        SensorLocation sensorLocation1 = new SensorLocation(R00, 1);
        locations.add(new ROILocation(sensorLocation0, 4, 100, 100));
        locations.add(new ROILocation(sensorLocation1, 5, 200, 200));
        ROICommon roi = new ROICommon(50, 50, 100);
        return guider.start(roi, "MC_C_20230101_000001", locations);
    }

//    @Command(name = "listen", description = "Listen for guider events")
//    public void listen() throws DAQException {
//        checkStore();
//        Thread t = new Thread(() -> {
//            try {
//                Location R00 = Location.of("R00/RebG");
//                guider.listen(R00, 0);
//            } catch (DAQException x) {
//                LOG.log(Level.SEVERE, "Error in listener", x);
//            }
//        });
//        t.start();
//    }
//
    @Command(name = "fits", description = "Write a FITS file")
    public void fitsWrite() throws DAQException {

        FitsHeadersSpecificationsBuilder headerSpecBuilder = new FitsHeadersSpecificationsBuilder();
        headerSpecBuilder.addSpecFile("guider-primary.spec", "primary");
        headerSpecBuilder.addSpecFile("guider-stamp.spec", "stamp");

        Map<String, HeaderSpecification> headerSpecifications = headerSpecBuilder.getHeaderSpecifications();

        checkStore();
        FitsIntWriter.FileNamer namer = (Map<String, Object> props) -> new File(new File("."), String.format("%s_%s_%s.fits", props.get("ImageName"), props.get("RaftBay"), props.get("CCDSlot")));

        Location R00 = Location.of("R00/RebG");
        SensorLocation sensorLocation0 = new SensorLocation(R00, 0);
        SensorLocation sensorLocation1 = new SensorLocation(R00, 1);
        FitsWriterFactory writer0 = new FitsWriterFactory(store.getPartition(), namer, headerSpecifications);
        FitsWriterFactory writer1 = new FitsWriterFactory(store.getPartition(), namer, headerSpecifications);

        Subscriber subscribe0 = guider.subscribe(Collections.singleton(sensorLocation0), ByteOrder.BIG_ENDIAN, writer0);
        Thread t0 = new Thread(() -> {
            for (;;) {
                try {
                    subscribe0.waitForGuider();
                } catch (DAQException x) {
                    LOG.log(Level.SEVERE, "DAQ Exception", x);
                }
            }
        });
        t0.start();
        Subscriber subscribe1 = guider.subscribe(Collections.singleton(sensorLocation1), ByteOrder.BIG_ENDIAN, writer1);//        Thread t = new Thread(() -> {
        Thread t1 = new Thread(() -> {
            for (;;) {
                try {
                    subscribe1.waitForGuider();
                } catch (DAQException x) {
                    LOG.log(Level.SEVERE, "DAQ Exception", x);
                }
            }
        });
        t1.start();
    }
    
    @Command(name = "version", description = "Get version info")
    public Version version() throws DAQException {
        return Store.getClientVersion();
    }

    @Command(name = "locations", description = "List configured locations")
    public LocationSet locations() throws DAQException {
        checkStore();
        return store.getConfiguredLocations();
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }
}
