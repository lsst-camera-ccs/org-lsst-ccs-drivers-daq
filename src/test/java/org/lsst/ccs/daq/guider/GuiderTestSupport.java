package org.lsst.ccs.daq.guider;

import org.lsst.ccs.daq.guider.Status.State;

/**
 * Test-only helpers that expose the package-private factories on {@link Status}
 * and {@link Config} to tests in other packages (e.g.
 * {@code org.lsst.ccs.daq.ims}). Production code never needs these; instances
 * normally come from the DAQ via JNI.
 *
 * @author tonyj
 */
public class GuiderTestSupport {

    private GuiderTestSupport() {
    }

    public static Status status(State in, State out) {
        return Status.of(in, out);
    }

    public static Config config(Status status) {
        return Config.of(status);
    }
}
