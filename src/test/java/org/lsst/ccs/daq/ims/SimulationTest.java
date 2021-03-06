package org.lsst.ccs.daq.ims;

import java.time.Instant;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Phaser;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 *
 * @author tonyj
 */
public class SimulationTest {

    @BeforeClass
    public static void setupClass() {
        System.setProperty("org.lsst.ccs.run.mode", "simulation");
    }

    @Test
    public void simulationTest() throws DAQException{
        Store store = new Store("test");
        assertEquals("test", store.getPartition());
        assertEquals(500_000_000_000L, store.getCapacity());
        assertEquals(250_000_000_000L, store.getRemaining());
        System.out.println(Store.getClientVersion());
    }
    
    @Test
    public void triggerTest() throws DAQException {
        AtomicInteger lastOpcode = new AtomicInteger();
        List<ImageMetaData> lastMeta = new ArrayList<>();
        Map<Location.LocationType, int[]> lastRegisterLists = new LinkedHashMap<>();
        StoreSimulation sim = StoreSimulation.instance();
        sim.addTriggerListener((int opcode, ImageMetaData meta, Map<Location.LocationType, int[]> registerLists) -> {
            lastOpcode.set(opcode);
            lastMeta.add(meta);
            if (registerLists != null) lastRegisterLists.putAll(registerLists);
        });
        Store store = new Store("test");
        Camera camera = store.getCamera();
        camera.setRegisterList(Location.LocationType.SCIENCE, new int[]{1, 2, 3});
        camera.setRegisterList(Location.LocationType.GUIDER, new int[]{4, 5, 6});
        camera.setRegisterList(Location.LocationType.WAVEFRONT, new int[]{7, 8, 9});
        ImageMetaData meta = new ImageMetaData("name", "folder", "annotation", 7, LocationSet.all());
        Image triggerImage = camera.triggerImage(meta);
        assertEquals(7, lastOpcode.get());
        assertEquals("name", lastMeta.get(0).getName());
        assertEquals("name", triggerImage.getMetaData().getName());
        assertArrayEquals(lastRegisterLists.get(Location.LocationType.SCIENCE), new int[]{1, 2, 3});

        lastMeta.clear();
        lastRegisterLists.clear();
        
        Instant startSequencer = camera.startSequencer(4);
        assertEquals(4, lastOpcode.get());
        assertNull(lastMeta.get(0));
        assertNull(lastRegisterLists.get(Location.LocationType.SCIENCE));
    }
    
    @Test
    public void imageListenerTest() throws DAQException, InterruptedException, TimeoutException {
        Phaser phaser = new Phaser(2);
        Store store = new Store("test");
        store.addImageListener(new ImageListener(){
            @Override
            public void imageCreated(Image image) {
                phaser.arrive();
            }

            @Override
            public void imageComplete(Image image) {
                phaser.arrive();
            }
        });
        Thread.sleep(100);
        Camera camera = store.getCamera();
        ImageMetaData meta = new ImageMetaData("name", "folder", "annotation", 7, LocationSet.all());
        Image triggerImage = camera.triggerImage(meta);
        phaser.awaitAdvanceInterruptibly(0, 10, TimeUnit.SECONDS);
    }
}
