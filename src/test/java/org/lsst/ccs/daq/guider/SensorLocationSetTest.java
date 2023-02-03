package org.lsst.ccs.daq.guider;

import java.util.Iterator;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class SensorLocationSetTest {

    @Test
    public void testLocationSetFromString() {
        final String locations = "R00/RebG/SG1";
        SensorLocationSet sls = new SensorLocationSet(locations);
        assertEquals(1, sls.size());
        assertEquals(locations, sls.iterator().next().toString());
    }

    @Test
    public void testLocationSetFromString2() {
        final String locations = "R00/RebG/SG1, R00/RebG/SG0";
        SensorLocationSet sls = new SensorLocationSet(locations);
        assertEquals(2, sls.size());
        final Iterator<SensorLocation> iterator = sls.iterator();
        assertEquals("R00/RebG/SG1", iterator.next().toString());
        assertEquals("R00/RebG/SG0", iterator.next().toString());
    }

    @Test
    public void testLocationSetFromString3() {
        try {
            final String locations = "R00/RebW/SG1, R00/RebG/SG0";
            SensorLocationSet sls = new SensorLocationSet(locations);
            fail(); // Should not get here
        } catch (IllegalArgumentException x) {
            // Expected
        }
    }
}
