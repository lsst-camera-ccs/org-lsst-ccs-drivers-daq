package org.lsst.ccs.daq.guider;

/**
 * ROI parameters which are shared by all ROIs.
 */
public class ROICommon {

    final int nRows;
    final int nCols;
    final int integrationTimeMilliSeconds;

    /**
     * Create an instance of ROICommand
     *
     * @param nRows The number of rows
     * @param nCols The number of columns
     * @param integrationTimeMilliSeconds The integration time in
     * milliseconds
     */
    public ROICommon(int nRows, int nCols, int integrationTimeMilliSeconds) {
        this.nRows = nRows;
        this.nCols = nCols;
        this.integrationTimeMilliSeconds = integrationTimeMilliSeconds;
    }

    public int getnRows() {
        return nRows;
    }

    public int getnCols() {
        return nCols;
    }

    public int getIntegrationTimeMillis() {
        return integrationTimeMilliSeconds;
    }

    @Override
    public String toString() {
        return "ROICommon{" + "nRows=" + nRows + ", nCols=" + nCols + ", integrationTimeMilliSeconds=" + integrationTimeMilliSeconds + '}';
    }
    
}
