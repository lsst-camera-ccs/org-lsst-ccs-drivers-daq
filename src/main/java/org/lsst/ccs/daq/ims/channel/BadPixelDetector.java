package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;

/**
 * A writer which counts pixel values below a threshold.
 * @author tonyj
 */
public class BadPixelDetector extends FilterWritableIntChannel {

    private int nTotal = 0;
    private int nBad = 0;
    private final int threshold;

    public BadPixelDetector(WritableIntChannel input) {
        this(input, 20000);
    }
    
    public BadPixelDetector(WritableIntChannel input, int threshold) {
        super(input);
        this.threshold = threshold;
    }

    @Override
    public void write(int i) throws IOException {
        if (i < threshold) {
            nBad++;
        }
        nTotal++;
    }

    public int getTotalPixels() {
        return nTotal;
    }

    public int getBadPixels() {
        return nBad;
    }

    public int getThreshold() {
        return threshold;
    }

}
