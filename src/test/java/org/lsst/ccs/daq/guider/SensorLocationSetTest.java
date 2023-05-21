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
        final String locations = "R00SG1";
        SensorLocationSet sls = SensorLocationSet.of(locations);
        assertEquals(1, sls.size());
        assertEquals(locations, sls.iterator().next().toString());
    }

    @Test
    public void testLocationSetFromString2() {
        final String locations = "R00SG1, R00SG0";
        SensorLocationSet sls = SensorLocationSet.of(locations);
        assertEquals(2, sls.size());
        final Iterator<SensorLocation> iterator = sls.iterator();
        assertEquals("R00SG0", iterator.next().toString());
        assertEquals("R00SG1", iterator.next().toString());
    }

    @Test
    public void testLocationSetFromString3() {
        try {
            final String locations = "R00SG2, R00SG0";
            SensorLocationSet sls = SensorLocationSet.of(locations);
            fail(); // Should not get here
        } catch (IllegalArgumentException x) {
            // Expected
        }
    }
}
