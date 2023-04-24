package org.lsst.ccs.daq.guider;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

/**
 * ROI parameters which are shared by all ROIs.
 */
public class ROICommon {

    final int rows;
    final int cols;
    final int integrationTimeMillis;

    /**
     * Create an instance of ROICommand
     *
     * @param rows The number of rows
     * @param cols The number of columns
     * @param integrationTimeMillis The integration time in
     * milliseconds
     */
    @JsonCreator
    public ROICommon(@JsonProperty("rows") int rows, @JsonProperty("cols") int cols, @JsonProperty("integrationTimeMillis") int integrationTimeMillis) {
        this.rows = rows;
        this.cols = cols;
        this.integrationTimeMillis = integrationTimeMillis;
    }

    public int getRows() {
        return rows;
    }

    public int getCols() {
        return cols;
    }

    public int getIntegrationTimeMillis() {
        return integrationTimeMillis;
    }

    @Override
    public String toString() {
        return "ROICommon{" + "rows=" + rows + ", cols=" + cols + ", integrationTimeMillis=" + integrationTimeMillis + '}';
    }
    
}
