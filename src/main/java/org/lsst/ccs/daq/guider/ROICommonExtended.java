package org.lsst.ccs.daq.guider;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * This is an extension of ROICommon which also includes extra parameters which are not
 * part of the common element of ROISpec.
 */
public class ROICommonExtended extends ROICommon {

    private final ROICommonExtendedParameters extended;

    public ROICommonExtended(ROICommon common) {
        super(common);
        this.extended = ROICommonExtendedParameters.defaultValue();
    }

    public ROICommonExtended(ROICommon common, String extended) {
        super(common);
        this.extended = ROICommonExtendedParameters.parse(extended);
    }

    public ROICommonExtended(ROICommon common, ROICommonExtendedParameters extended) {
        super(common);
        this.extended = extended;
    }

    public ROICommonExtended(ROICommon common, int underCols, int overCols, int overRows, int flushCount) {
        super(common);
        extended = new ROICommonExtendedParameters(underCols, overCols, overRows, flushCount);
    }

    public ROICommonExtended(int rows, int cols, int integrationTimeMillis, int underCols, int overCols, int overRows, int flushCount) {
        super(rows, cols, integrationTimeMillis);
        extended = new ROICommonExtendedParameters(underCols, overCols, overRows, flushCount);
    }

    public int getUnderCols() {
        return extended.getUnderCols();
    }

    public int getOverCols() {
        return extended.getOverCols();
    }

    public int getOverRows() {
        return extended.getOverRows();
    }

    public int getFlushCount() {
        return extended.getFlushCount();
    }

    @Override
    public String toString() {
        return "ROICommonExtended [rows=" + getRows() + ", cols=" + getCols() + ", integrationTimeMillis=" + getIntegrationTimeMillis()
                + ", underCols=" + extended.getUnderCols() + ", overCols=" + extended.getOverCols() + ", overRows=" + extended.getOverRows() + ", flushCount="
                + extended.getFlushCount() + "]";
    }

    public static class ROICommonExtendedParameters {
        private final int underCols;
        private final int overCols;
        private final int overRows;
        private final int flushCount;
        public final static String DEFAULT_ROI_COMMON_EXTENDED_STRING = "{\"underCols\":0,\"overCols\":0,\"overRows\":0,\"flushCount\":2}";
        private final static ROICommonExtendedParameters DEFAULT_ROI_COMMON_EXTENDED_PARAMETERS = ROICommonExtendedParameters.parse(DEFAULT_ROI_COMMON_EXTENDED_STRING);

        @JsonCreator(mode = JsonCreator.Mode.PROPERTIES)
        public ROICommonExtendedParameters(@JsonProperty("underCols") int underCols, @JsonProperty("overCols") int overCols,
            @JsonProperty("overRows") int overRows, @JsonProperty("flushCount") int flushCount) {
                this.underCols = underCols;
                this.overCols = overCols;
                this.overRows = overRows;
                this.flushCount = flushCount;
        }

        public static ROICommonExtendedParameters defaultValue() {
            return DEFAULT_ROI_COMMON_EXTENDED_PARAMETERS;
        }

        public int getUnderCols() {
            return underCols;
        }

        public int getOverCols() {
            return overCols;
        }

        public int getOverRows() {
            return overRows;
        }

        public int getFlushCount() {
            return flushCount;
        }

        public static ROICommonExtendedParameters parse(String roiExtendedParameters) throws IllegalArgumentException {
            try {
                ObjectMapper om = new ObjectMapper();
                return om.readValue(roiExtendedParameters, ROICommonExtendedParameters.class);
            } catch (JsonProcessingException x) {
                throw new IllegalArgumentException("Invalid ROICommonExtendedParameters " + roiExtendedParameters, x);
            }
        }

        @Override
        public String toString() {
            try {
                ObjectMapper om = new ObjectMapper();
                return om.writeValueAsString(this);
            } catch (JsonProcessingException ex) {
                throw new RuntimeException("Can't happen!!!", ex);
            }
        }
    }
}
