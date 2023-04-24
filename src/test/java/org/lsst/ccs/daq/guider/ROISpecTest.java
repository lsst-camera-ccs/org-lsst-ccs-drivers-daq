package org.lsst.ccs.daq.guider;

import java.util.ArrayList;
import java.util.List;
import org.junit.Test;
import static org.junit.Assert.*;
import org.lsst.ccs.utilities.location.Location;

/**
 *
 * @author tonyj
 */
public class ROISpecTest {
    

    /**
     * Test of parse method, of class ROISpec.
     */
    @Test
    public void testParse() {
        String testData = "{ \"common\": { \"rows\": 50, \"cols\": 100, \"integrationTimeMillis\": 100}, "
                + "\"R00SG0\": { \"segment\": 10, \"startRow\": 100, \"startCol\": 200},"
                + "\"R00SG1\": {\"segment\": 7, \"startRow\": 50, \"startCol\": 150} }  ";
        ROISpec spec = ROISpec.parse(testData);
        assertEquals(50, spec.getCommon().getRows());
        assertEquals(100, spec.getCommon().getCols());
        assertEquals(100, spec.getCommon().getIntegrationTimeMillis());
        assertEquals(2, spec.getLocations().size());
        
        assertEquals("R00SG0", spec.getLocations().get(0).getLocation().toString());
        assertEquals(10, spec.getLocations().get(0).getSegment());
    }

    /**
     * Test of toString method, of class ROISpec.
     */
    @Test
    public void testToString() {
        List<ROILocation> locations = new ArrayList<>();
        Location R00 = Location.of("R00/RebG");
        SensorLocation sensorLocation0 = new SensorLocation(R00, 0);
        SensorLocation sensorLocation1 = new SensorLocation(R00, 1);
        locations.add(new ROILocation(sensorLocation0, 4, 100, 100));
        locations.add(new ROILocation(sensorLocation1, 5, 200, 200));
        ROICommon common = new ROICommon(50, 50, 100);
        ROISpec spec = new ROISpec(common, locations);
        assertEquals("{\"common\":{\"rows\":50,\"cols\":50,\"integrationTimeMillis\":100},\"R00SG0\":{\"segment\":4,\"startRow\":100,\"startCol\":100},\"R00SG1\":{\"segment\":5,\"startRow\":200,\"startCol\":200}}",spec.toString());
    }
    
}
