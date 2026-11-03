package org.lsst.ccs.daq.ims;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.BeforeClass;
import org.junit.Test;
import org.lsst.ccs.daq.guider.Config;
import org.lsst.ccs.daq.guider.GuiderSimulationSupport;
import org.lsst.ccs.daq.guider.RetryStatus;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.daq.guider.Status.State;

/**
 * Tests the command-timeout reconciliation logic in {@link Guider} without any
 * DAQ hardware (the simulated store implementation never times out).
 * <p>
 * A test {@link Store} is run in simulation mode (so the base constructor's
 * native attach succeeds) and its package-private {@code pauseGuider} and
 * {@code guiderConfig} methods are overridden to drive the three branches:
 * command took effect, state unchanged (retry succeeds), and inconclusive state.
 *
 * @author tonyj
 */
public class GuiderRetryTest {

    @BeforeClass
    public static void setupClass() {
        System.setProperty("org.lsst.ccs.run.mode", "simulation");
    }

    /**
     * A Store whose pause command throws a command timeout for the first
     * {@code timeouts} invocations, then (if reached) returns a success Status,
     * and whose config() reports a configurable current state.
     */
    private static class FakeStore extends Store {

        private final int timeouts;
        private final State reportedState;
        int pauseCalls;
        int configCalls;

        FakeStore(int timeouts, State reportedState) throws DAQException {
            super("test");
            this.timeouts = timeouts;
            this.reportedState = reportedState;
        }

        @Override
        Status pauseGuider(long guider, String comment) throws DAQException {
            pauseCalls++;
            if (pauseCalls <= timeouts) {
                throw new DAQException("pause timed out", -1);
            }
            return GuiderSimulationSupport.status(State.RUNNING, State.PAUSED);
        }

        @Override
        Config guiderConfig(long guider) throws DAQException {
            configCalls++;
            return GuiderSimulationSupport.config(GuiderSimulationSupport.status(reportedState, reportedState));
        }
    }

    /** Command was acted on but the response was lost: state is already at the target. */
    @Test
    public void timeoutButCommandTookEffect() throws DAQException {
        FakeStore store = new FakeStore(1, State.PAUSED);
        Guider guider = new Guider(store, 0L, 1);
        RetryStatus status = guider.pause();
        assertEquals("pause should not be re-issued once the guider is already PAUSED", 1, store.pauseCalls);
        assertTrue("state should have been queried after the timeout", store.configCalls >= 1);
        assertEquals(State.PAUSED, status.getOut());
        assertEquals("the command timed out once", 1, status.getTimeouts());
        assertEquals("the command took effect, so it was never re-issued", 0, status.getRetries());
    }

    /**
     * Command was acted on but landed in the idle-mode twin of the primary
     * target: pause reaches IDLEPAUSE (not PAUSED) when started from
     * IDLECLEARING. This must still be recognised as success.
     */
    @Test
    public void timeoutCommandTookEffectInIdleTwinState() throws DAQException {
        FakeStore store = new FakeStore(1, State.IDLEPAUSE);
        Guider guider = new Guider(store, 0L, 1);
        RetryStatus status = guider.pause();
        assertEquals("pause should not be re-issued once the guider is already IDLEPAUSE", 1, store.pauseCalls);
        assertTrue("state should have been queried after the timeout", store.configCalls >= 1);
        assertEquals(State.IDLEPAUSE, status.getOut());
        assertEquals("the command timed out once", 1, status.getTimeouts());
        assertEquals("the command took effect, so it was never re-issued", 0, status.getRetries());
    }

    /** Command never reached the DAQ: state unchanged, retry succeeds. */
    @Test
    public void timeoutThenRetrySucceeds() throws DAQException {
        FakeStore store = new FakeStore(1, State.RUNNING);
        Guider guider = new Guider(store, 0L, 1);
        RetryStatus status = guider.pause();
        assertEquals("pause should be retried once", 2, store.pauseCalls);
        assertEquals(State.PAUSED, status.getOut());
        assertEquals("the command timed out once", 1, status.getTimeouts());
        assertEquals("the command was re-issued once", 1, status.getRetries());
    }

    /** Inconclusive post-timeout state: no success fabricated, no blind retry. */
    @Test
    public void timeoutWithInconclusiveStateThrows() throws DAQException {
        FakeStore store = new FakeStore(1, State.ERROR);
        Guider guider = new Guider(store, 0L, 1);
        try {
            guider.pause();
            fail("expected a DAQException for an inconclusive post-timeout state");
        } catch (DAQException x) {
            assertEquals(-1, x.rc());
            assertTrue("message should name the observed state", x.getMessage().contains("ERROR"));
            assertEquals("pause must not be re-issued in an inconclusive state", 1, store.pauseCalls);
        }
    }

    /** With retries disabled the timeout propagates even if the state is unchanged. */
    @Test
    public void noRetriesPropagatesTimeout() throws DAQException {
        FakeStore store = new FakeStore(1, State.RUNNING);
        Guider guider = new Guider(store, 0L, 0);
        try {
            guider.pause();
            fail("expected the timeout to propagate when retries are disabled");
        } catch (DAQException x) {
            assertEquals(-1, x.rc());
            assertEquals(1, store.pauseCalls);
        }
    }
}
