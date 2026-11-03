package org.lsst.ccs.daq.ims;

import java.util.Collections;
import java.util.List;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.lsst.ccs.daq.guider.ROICommon;
import org.lsst.ccs.daq.guider.ROILocation;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.Status.State;

/**
 * Exercises the emulated guider state machine and fault-injection hook in the
 * simulated store (LSSTCCSDRIVER-498). These drive the real
 * {@link Guider}/{@link Store} command path against
 * {@link StoreSimulatedImplementation}, which previously returned {@code null}
 * for every guider command.
 *
 * @author tonyj
 */
public class GuiderSimulationTest {

    // The simulator ignores ROI arguments; any valid ROICommon / empty location
    // list is enough to drive the start command through the real Guider path.
    private static final ROICommon ROI_COMMON = new ROICommon(10, 10, 100);
    private static final List<ROILocation> NO_LOCATIONS = Collections.emptyList();

    @BeforeClass
    public static void setupClass() {
        System.setProperty("org.lsst.ccs.run.mode", "simulation");
    }

    @Before
    public void resetGuider() {
        StoreSimulation.instance().resetGuider();
    }

    private Guider guider() throws DAQException {
        return new Store("test").getGuider();
    }

    @Test
    public void commandsDriveStateTransitions() throws DAQException {
        Guider guider = guider();

        // The normal focal-plane guiding flow, all diagram-legal edges.
        assertEquals(State.PAUSED, guider.start(ROI_COMMON, NO_LOCATIONS).getOut()); // IDLECLEARING -> PAUSED
        assertEquals(State.RUNNING, guider.resume().getOut());                       // PAUSED -> RUNNING
        assertEquals(State.PAUSED, guider.pause().getOut());                         // RUNNING -> PAUSED
        assertEquals(State.IDLECLEARING, guider.stop().getOut());                    // PAUSED -> IDLECLEARING
        assertEquals(State.SLEEPING, guider.sleep().getOut());                       // IDLECLEARING -> SLEEPING
        assertEquals(State.IDLECLEARING, guider.wake().getOut());                    // SLEEPING -> IDLECLEARING

        assertEquals("sim state should track the last command",
                State.IDLECLEARING, StoreSimulation.instance().getGuiderState());
    }

    @Test
    public void pauseFromIdleClearingGoesToIdlePaused() throws DAQException {
        Guider guider = guider();
        // From IDLECLEARING, pause is the idle-pause edge (-> IDLEPAUSE), distinct
        // from the running-pause edge (RUNNING -> PAUSED) exercised above.
        assertEquals(State.IDLEPAUSE, guider.pause().getOut());
        assertEquals(State.IDLECLEARING, guider.resume().getOut()); // IDLEPAUSE -> IDLECLEARING
    }

    @Test
    public void statusInIsPreviousStateOutIsNew() throws DAQException {
        Guider guider = guider();
        guider.start(ROI_COMMON, NO_LOCATIONS); // IDLECLEARING -> PAUSED
        var resumed = guider.resume();          // PAUSED -> RUNNING
        assertEquals(State.PAUSED, resumed.getIn());
        assertEquals(State.RUNNING, resumed.getOut());
    }

    @Test
    public void seriesAndConfigReflectCurrentState() throws DAQException {
        Guider guider = guider();
        guider.start(ROI_COMMON, NO_LOCATIONS); // -> PAUSED

        assertEquals(State.PAUSED, guider.config().getStatus().getOut());

        SeriesStatus series = guider.series();
        assertEquals(State.PAUSED, series.getStatus().getOut());
        // Series stand-in is non-null so downstream getSeries().getStamps() works.
        assertNotNull(series.getSeries());
        assertEquals(0, series.getSeries().getStamps());
    }

    @Test
    public void sensorTimeoutDrivesErrorThenRecovers() throws DAQException {
        Guider guider = guider();
        guider.start(ROI_COMMON, NO_LOCATIONS); // IDLECLEARING -> PAUSED
        guider.resume();                        // PAUSED -> RUNNING

        // Autonomous: one or more sensors time out while running.
        var timedOut = StoreSimulation.instance().triggerSensorTimeout();
        assertEquals(State.RUNNING, timedOut.getIn());
        assertEquals(State.ERROR, timedOut.getOut());
        assertEquals(State.ERROR, StoreSimulation.instance().getGuiderState());

        // ERROR has two commanded exits in the diagram: Stop and Sleep.
        assertEquals(State.IDLECLEARING, guider.stop().getOut());
    }

    @Test
    public void illegalTransitionThrowsInvalidTransition() throws DAQException {
        Guider guider = guider();
        // From the power-up/idle IDLECLEARING state, resume is forbidden
        // ("all other transitions are forbidden" in the GDS States diagram).
        try {
            guider.resume();
            fail("resume from IDLECLEARING should be an invalid transition");
        } catch (DAQException x) {
            assertEquals(StoreSimulation.INVALID_TRANSITION, x.rc());
            assertTrue("message should name the invalid transition: " + x.getMessage(),
                    x.getMessage().contains("Invalid transition requested"));
        }
        // State is unchanged by the rejected command.
        assertEquals(State.IDLECLEARING, StoreSimulation.instance().getGuiderState());
    }

    @Test
    public void armedFaultIsThrownOnceThenCleared() throws DAQException {
        Guider guider = guider();
        // rc=2 (INVALID_TRANSITION): a hard failure, not a COMMAND_TIMEOUT (rc=-1),
        // so Guider.withRetry does NOT reconcile/retry it. This is the shape of
        // fault LSSTCCSRAFTS-834 needs: a pause that genuinely fails.
        DAQException injected = new DAQException("injected pause failure", 2);
        StoreSimulation.instance().armGuiderFault(injected);

        try {
            guider.pause();
            fail("armed fault should have been thrown");
        } catch (DAQException x) {
            assertSame(injected, x);
        }

        // Fault is one-shot: the next command succeeds and the state is intact.
        assertEquals("state must be unchanged by a faulted command",
                State.IDLECLEARING, StoreSimulation.instance().getGuiderState());
        // pause from IDLECLEARING is the idle-pause edge -> IDLEPAUSE.
        assertEquals(State.IDLEPAUSE, guider.pause().getOut());
    }
}
