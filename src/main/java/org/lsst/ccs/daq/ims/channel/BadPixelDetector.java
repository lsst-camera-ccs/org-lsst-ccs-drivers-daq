package org.lsst.ccs.daq.ims.channel;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A writer which counts pixel values below a lowThreshold.
 * @author tonyj
 */
public class BadPixelDetector extends FilterWritableIntChannel {

    private int nTotal = 0;
    private int nBad = 0;
    private final int lowThreshold;
    private final int highThreshold;
    private static final Logger LOG = Logger.getLogger(BadPixelDetector.class.getName());

    public BadPixelDetector(WritableIntChannel input) {
        // Defaults based on https://lsstc.slack.com/team/U2LU22P4N
        this(input, 10000, 240000);
    }
    
    public BadPixelDetector(WritableIntChannel input, int lowThreshold, int highThreshold) {
        super(input);
        this.lowThreshold = lowThreshold;
        this.highThreshold = highThreshold;
    }

    @Override
    public void write(int i) throws IOException {
        if (i < lowThreshold || i > highThreshold) {
            nBad++;
            LOG.log(Level.FINER, () -> String.format("Bad pixel %d at %d\n", i, nTotal));
        }
        input.write(i);
        nTotal++;
    }

    public int getTotalPixels() {
        return nTotal;
    }

    public int getBadPixels() {
        return nBad;
    }

    public int getLowThreshold() {
        return lowThreshold;
    }

    public int getHighThreshold() {
        return highThreshold;
    }
}
