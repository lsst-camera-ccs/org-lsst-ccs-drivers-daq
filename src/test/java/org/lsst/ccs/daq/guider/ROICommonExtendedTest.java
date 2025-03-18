package org.lsst.ccs.daq.guider;

import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.lsst.ccs.daq.guider.ROICommonExtended.ROICommonExtendedParameters;

/**
 *
 * @author tonyj
 */
public class ROICommonExtendedTest {

    @Test
    public void parseTest() {
        ROICommonExtendedParameters roiExtended = ROICommonExtendedParameters.parse(ROICommonExtendedParameters.DEFAULT_ROI_COMMON_EXTENDED_STRING);
        assertEquals(2, roiExtended.getFlushCount());
        assertEquals(0, roiExtended.getOverCols());
        assertEquals(0, roiExtended.getOverRows());
        assertEquals(0, roiExtended.getUnderCols());

    }

    @Test
    public void testToString() {
        ROICommonExtendedParameters roiExtended = ROICommonExtendedParameters.defaultValue();
        assertEquals(ROICommonExtendedParameters.DEFAULT_ROI_COMMON_EXTENDED_STRING, roiExtended.toString());
    }

}
