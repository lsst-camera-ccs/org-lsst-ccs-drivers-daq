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
import org.lsst.ccs.daq.guider.ClearParameters;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.FitsWriterFactory;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Guider;
import org.lsst.ccs.daq.guider.ROISpec;
import org.lsst.ccs.utilities.location.SensorLocation;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.daq.ims.Guider.Subscriber;
import org.lsst.ccs.daq.ims.Store;
import org.lsst.ccs.daq.ims.Version;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter;
import org.lsst.ccs.utilities.image.FitsHeadersSpecificationsBuilder;
import org.lsst.ccs.utilities.image.HeaderSpecification;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 *
 * @author tonyj
 */
public class GuiderTool {

    private static final Logger LOG = Logger.getLogger(GuiderTool.class.getName());

    private Store store;
    private Guider guider;
    private List<Subscriber> subscribers= new ArrayList<>();

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
    public Status resume(String comment) throws DAQException {
        checkStore();
        return guider.resume(comment);
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
    public SeriesStatus series() throws DAQException {
        checkStore();
        return guider.series();
    }

    @Command(name = "start", description = "Start the guider")
    public Status start(String roiSpec) throws DAQException {
        checkStore();
        ROISpec spec = ROISpec.parse(roiSpec);
        spec.sanityCheck(guider.getConfiguredLocations());
        return guider.start(spec.getCommon(), spec.getLocations());
    }

    @Command(name = "clear", description = "Clear the guider")
    public Status clear(int delay, int preRows, int flushCount, int readRows) throws DAQException {
        checkStore();
        ClearParameters cp = new ClearParameters(delay, preRows, flushCount, readRows);
        return guider.clear(cp);
    }

    @Command(name = "validate", description = "Validate the roi")
    public void validate(String roiSpec) throws DAQException {
        checkStore();
        ROISpec spec = ROISpec.parse(roiSpec);
        spec.sanityCheck(guider.getConfiguredLocations());
        guider.validate(spec.getCommon(), spec.getLocations());
    }


    @Command(name = "fits", description = "Subscribe to notifications to write a FITS file")
    public void fitsWrite(String location, @Argument(defaultValue = "false") boolean includeRawStamp) throws DAQException {

        FitsHeadersSpecificationsBuilder headerSpecBuilder = new FitsHeadersSpecificationsBuilder();
        headerSpecBuilder.addSpecFile("guider-primary.spec", "primary");
        headerSpecBuilder.addSpecFile("guider-stamp.spec", "stamp");

        Map<String, HeaderSpecification> headerSpecifications = headerSpecBuilder.getHeaderSpecifications();

        checkStore();
        FitsIntWriter.FileNamer namer = (Map<String, Object> props) -> new File(new File("."), String.format("%s_%s_%s.fits", props.get("ImageName"), props.get("RaftBay"), props.get("CCDSlot")));

        SensorLocation sensorLocation = SensorLocation.of(location);
        FitsWriterFactory writer = new FitsWriterFactory(store.getPartition(), namer, headerSpecifications, includeRawStamp);

        Subscriber subscriber = guider.subscribe(Collections.singleton(sensorLocation), ByteOrder.BIG_ENDIAN, writer);
        subscribers.add(subscriber);
        Thread t0 = new Thread(() -> {
            for (;;) {
                try {
                    subscriber.waitForGuider();
                } catch (DAQException x) {
                    LOG.log(Level.SEVERE, "DAQ Exception", x);
                }
            }
        });
        t0.start();
    }

    @Command(name="unsubscribe", description = "Unsubscribe ")
    public void unsubscribe() throws DAQException {
        for (Subscriber subscriber : subscribers) {
            subscriber.close();
        }
    }

    @Command(name = "version", description = "Get version info")
    public Version version() throws DAQException {
        return Store.getClientVersion();
    }

    @Command(name = "platform", description = "Get platform name")
    public String platform() throws DAQException {
        checkStore();
        return guider.getClientPlatform();
    }

    @Command(name = "locations", description = "List configured locations")
    public LocationSet locations() throws DAQException {
        checkStore();
        return guider.getConfiguredLocations();
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }
}
