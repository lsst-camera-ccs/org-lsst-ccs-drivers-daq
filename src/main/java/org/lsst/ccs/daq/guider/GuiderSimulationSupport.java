package org.lsst.ccs.daq.guider;

import java.util.List;
import org.lsst.ccs.daq.guider.Status.State;
import org.lsst.ccs.utilities.location.SensorLocation;

/**
 * Bridges the package-private factories on {@link Status}, {@link Config} and
 * {@link Series} to the simulated store implementation in
 * {@code org.lsst.ccs.daq.ims}, which needs to fabricate guider results that
 * normally only come from the DAQ via JNI.
 * <p>
 * It exists so the simulated store (and tests) can fabricate these objects
 * without the package-private {@link Status}/{@link Config} factories or the
 * private {@link Series} constructor being widened to {@code public}.
 *
 * @author tonyj
 */
public final class GuiderSimulationSupport {

    private GuiderSimulationSupport() {
    }

    /**
     * Builds a {@link Status} with the given in/out states.
     */
    public static Status status(State in, State out) {
        return Status.of(in, out);
    }

    /**
     * Builds a {@link Config} wrapping the given status (no ROI/clear detail).
     */
    public static Config config(Status status) {
        return Config.of(status);
    }

    /**
     * Builds a {@link Series} with the given fields.
     */
    public static Series series(long timestampNanos, int sequence, int stamps,
            List<SensorLocation> configured, List<SensorLocation> remaining) {
        return Series.of(timestampNanos, sequence, stamps, configured, remaining);
    }
}
