package org.lsst.ccs.daq.ims;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
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
            lastRegisterLists.putAll(registerLists);
        });
        Store store = new Store("test");
        store.setRegisterList(Location.LocationType.SCIENCE, new int[]{1, 2, 3});
        store.setRegisterList(Location.LocationType.GUIDER, new int[]{4, 5, 6});
        store.setRegisterList(Location.LocationType.WAVEFRONT, new int[]{7, 8, 9});
        ImageMetaData meta = new ImageMetaData("name", "folder", "annotation", 7, LocationSet.all());
        ImageMetaData triggerImage = store.triggerImage(meta);
        assertEquals(7, lastOpcode.get());
        assertEquals("name", lastMeta.get(0).getName());
        assertEquals("name", triggerImage.getName());
        assertArrayEquals(lastRegisterLists.get(Location.LocationType.SCIENCE), new int[]{1, 2, 3});

        lastMeta.clear();
        lastRegisterLists.clear();
        
        long startSequencer = store.startSequencer(4);
        assertEquals(4, lastOpcode.get());
        assertNull(lastMeta.get(0));
        assertArrayEquals(lastRegisterLists.get(Location.LocationType.SCIENCE), new int[]{1, 2, 3});
    }
}
