package org.lsst.ccs.daq.ims.channel;

import java.util.HashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;

/**
 * Constants used to control conversion of raw data to FITS pixels
 *
 * @author tonyj
 */
class ReadoutConfig {

    /**
     * The order of the Segments in the fits files is: S10->...
     * ->S17->S07->...->S00
     *
     * while the order of the segments coming from the DAQ is
     * S00->...->S07->S10->...->S17
     *
     * So we introduce the array below to describe the needed mapping. Note, the
     * ATS appears to have a different mapping, so this needs to be made
     * settable.
     */
    private static final Map<Location.LocationType, int[]> DATA_SEGMENT_MAP = new HashMap<>();
    private static final Map<Location.LocationType, int[]> DATA_SENSOR_MAP = new HashMap<>();
    private static final Map<Location.LocationType, Integer> XOR_MAP = new HashMap<>();
    private static final Map<Location.LocationType, String[]> DATA_SEGMENT_NAME_MAP = new HashMap<>();

    private static final int[] AUXTEL_SENSOR_MAP = {0};
    private static final int[] AUXTEL_SEGMENT_MAP = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    
    static {
        DATA_SEGMENT_NAME_MAP.put(Location.LocationType.SCIENCE, new String[]{
            "Segment10", "Segment11", "Segment12", "Segment13", "Segment14", "Segment15", "Segment16", "Segment17",
            "Segment07", "Segment06", "Segment05", "Segment04", "Segment03", "Segment02", "Segment01", "Segment00"
        });
        DATA_SEGMENT_NAME_MAP.put(Location.LocationType.GUIDER, new String[]{
            "Segment10", "Segment11", "Segment12", "Segment13", "Segment14", "Segment15", "Segment16", "Segment17",
            "Segment07", "Segment06", "Segment05", "Segment04", "Segment03", "Segment02", "Segment01", "Segment00"
        });
        DATA_SEGMENT_NAME_MAP.put(Location.LocationType.WAVEFRONT, new String[]{
            "Segment10", "Segment11", "Segment12", "Segment13", "Segment14", "Segment15", "Segment16", "Segment17",
        });
        DATA_SEGMENT_MAP.put(Location.LocationType.SCIENCE, new int[]{15, 14, 13, 12, 11, 10, 9, 8, 0, 1, 2, 3, 4, 5, 6, 7});
        DATA_SEGMENT_MAP.put(Location.LocationType.GUIDER, new int[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
        DATA_SEGMENT_MAP.put(Location.LocationType.WAVEFRONT, new int[]{0, 1, 2, 3, 4, 5, 6, 7});

        DATA_SENSOR_MAP.put(Location.LocationType.SCIENCE, new int[]{2, 1, 0});
        DATA_SENSOR_MAP.put(Location.LocationType.GUIDER, new int[]{1, 0});
        DATA_SENSOR_MAP.put(Location.LocationType.WAVEFRONT, new int[]{0, 1});
        
        XOR_MAP.put(Location.LocationType.SCIENCE, 0x1FFFF);
        XOR_MAP.put(Location.LocationType.GUIDER, 0x20000);
        XOR_MAP.put(Location.LocationType.WAVEFRONT, 0x20000);
    }
    private final Location.LocationType sourceType;
    private final boolean isAuxTel;
    
    ReadoutConfig(Location.LocationType sourceType, boolean isAuxTel) {
        this.sourceType = sourceType;
        this.isAuxTel = isAuxTel;
    }

    int[] getDataSegmentMap() {
        if (isAuxTel) {
            return AUXTEL_SEGMENT_MAP;
        }
        return DATA_SEGMENT_MAP.get(sourceType);
    }

    int[] getDataSensorMap() {
        if (isAuxTel) {
            return AUXTEL_SENSOR_MAP;
        }
        return DATA_SENSOR_MAP.get(sourceType);
    }
    
    String[] getDataSegmentNames() {
        if (isAuxTel) {
            return DATA_SEGMENT_NAME_MAP.get(Location.LocationType.SCIENCE);
        }
        return DATA_SEGMENT_NAME_MAP.get(sourceType);
    }

    int getXor() {
        return XOR_MAP.get(sourceType);
    }
}
