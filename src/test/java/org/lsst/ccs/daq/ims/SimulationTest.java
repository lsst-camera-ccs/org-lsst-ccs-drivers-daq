package org.lsst.ccs.daq.ims;

import static org.junit.Assert.assertEquals;
import org.junit.BeforeClass;
import org.junit.Test;

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
    }
}
