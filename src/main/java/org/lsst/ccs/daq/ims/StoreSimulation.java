package org.lsst.ccs.daq.ims;

import java.util.Collections;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;
import org.lsst.ccs.daq.guider.GuiderSimulationSupport;
import org.lsst.ccs.daq.guider.Series;
import org.lsst.ccs.daq.guider.SeriesStatus;
import org.lsst.ccs.daq.guider.Status;
import org.lsst.ccs.daq.guider.Status.State;
import org.lsst.ccs.utilities.location.Location;

/**
 * Utilities for interacting with a simulated store
 *
 * @author tonyj
 */
public class StoreSimulation {

    private final List<TriggerListener> listeners = new CopyOnWriteArrayList<>();
    private RegisterAccess registerAccess;

    /** Current emulated guider state; see {@link #advanceGuider}. */
    private volatile State guiderState = State.IDLECLEARING;
    /** One-shot fault: thrown (and cleared) by the next guider command. */
    private volatile DAQException guiderFault;

    private static final StoreSimulation singleton = new StoreSimulation();

    private StoreSimulation() {

    }

    public static StoreSimulation instance() {
        return singleton;
    }

    // ------------------------------------------------------------------
    // Emulated guider state machine (LSSTCCSDRIVER-498)
    //
    // Transitions follow the GDS States diagram (the "Commanded" solid edges);
    // every (state, command) pair not in the table is one of the diagram's
    // "all other transitions are forbidden" cases and throws an
    // INVALID_TRANSITION DAQException. Note the ambiguous commands are
    // source-state-dependent, not two outcomes of one edge:
    //   pause:  RUNNING -> PAUSED,  IDLECLEARING -> IDLEPAUSE
    //   resume: PAUSED  -> RUNNING, IDLEPAUSE    -> IDLECLEARING
    // Autonomous edges (Finite Loop Count, sensor-timeout -> ERROR) are not
    // modelled; this table is the commanded surface only.
    // ------------------------------------------------------------------

    /** GDS - Invalid transition requested (gds/Exception.hh Id). */
    static final int INVALID_TRANSITION = 2;

    /** Guider commands that drive {@link #advanceGuider}. */
    enum GuiderCommand {
        START, STOP, PAUSE, RESUME, SLEEP, WAKE, CLEAR
    }

    /**
     * Legal commanded transitions, keyed by current state then command. Mirrors
     * the solid edges of the GDS States diagram.
     */
    private static final Map<State, Map<GuiderCommand, State>> TRANSITIONS = buildTransitions();

    private static Map<State, Map<GuiderCommand, State>> buildTransitions() {
        Map<State, Map<GuiderCommand, State>> t = new EnumMap<>(State.class);
        for (State s : State.values()) {
            t.put(s, new EnumMap<>(GuiderCommand.class));
        }
        // Error
        t.get(State.ERROR).put(GuiderCommand.STOP, State.IDLECLEARING);
        t.get(State.ERROR).put(GuiderCommand.SLEEP, State.SLEEPING);
        // IdleClearing
        t.get(State.IDLECLEARING).put(GuiderCommand.SLEEP, State.SLEEPING);
        t.get(State.IDLECLEARING).put(GuiderCommand.CLEAR, State.CLEARING);
        t.get(State.IDLECLEARING).put(GuiderCommand.PAUSE, State.IDLEPAUSE);
        t.get(State.IDLECLEARING).put(GuiderCommand.START, State.PAUSED);
        // Sleeping
        t.get(State.SLEEPING).put(GuiderCommand.WAKE, State.IDLECLEARING);
        // Clearing
        t.get(State.CLEARING).put(GuiderCommand.STOP, State.IDLECLEARING);
        // IdlePaused
        t.get(State.IDLEPAUSE).put(GuiderCommand.RESUME, State.IDLECLEARING);
        // Paused
        t.get(State.PAUSED).put(GuiderCommand.STOP, State.IDLECLEARING);
        t.get(State.PAUSED).put(GuiderCommand.RESUME, State.RUNNING);
        t.get(State.PAUSED).put(GuiderCommand.START, State.PAUSED); // self-loop
        // Running
        t.get(State.RUNNING).put(GuiderCommand.STOP, State.IDLECLEARING);
        t.get(State.RUNNING).put(GuiderCommand.PAUSE, State.PAUSED);
        return t;
    }

    /**
     * Returns the current emulated guider state.
     */
    public State getGuiderState() {
        return guiderState;
    }

    /**
     * Sets the emulated guider state directly. Intended for tests that need to
     * stage a particular starting state before driving a command flow.
     *
     * @param state the state to set (must not be {@code null})
     */
    public void setGuiderState(State state) {
        if (state == null) {
            throw new IllegalArgumentException("guider state must not be null");
        }
        this.guiderState = state;
    }

    /**
     * Resets the emulated guider to its default idle state ({@code IDLECLEARING})
     * and clears any armed fault. Tests sharing the singleton should call this in
     * setup/teardown to avoid state leaking between cases.
     */
    public void resetGuider() {
        this.guiderState = State.IDLECLEARING;
        this.guiderFault = null;
    }

    /**
     * Arms a one-shot fault: the next emulated guider command will throw the
     * given exception instead of transitioning, then disarm. This lets a test
     * exercise a subsystem's guider error-handling path against the simulator
     * (e.g. LSSTCCSRAFTS-834: focal-plane must end integration even if a guider
     * pause fails).
     *
     * @param fault the exception the next guider command should throw
     */
    public void armGuiderFault(DAQException fault) {
        this.guiderFault = fault;
    }

    /**
     * Forces the autonomous sensor-timeout transition into {@code ERROR},
     * modelling the diagram's "One or more Sensors have timed out" edge. This is
     * not a commanded transition, so it is not part of the {@link #advanceGuider}
     * table; it is a test hook that lets a subsystem exercise its ERROR-recovery
     * paths (e.g. focal-plane's recover-from-ERROR-before-pause) against the
     * simulator. In the real GDS this arises while sensors are active
     * (RUNNING/CLEARING); the hook sets ERROR unconditionally so a test can stage
     * it from whatever state it has driven the guider into.
     *
     * @return the status after the transition (in = previous state, out = ERROR)
     */
    public synchronized Status triggerSensorTimeout() {
        State previous = guiderState;
        guiderState = State.ERROR;
        return GuiderSimulationSupport.status(previous, State.ERROR);
    }

    /**
     * Applies {@code command} to the emulated guider and returns a Status
     * reflecting the transition (in = previous state, out = new state), per the
     * GDS States diagram.
     * <p>
     * If a fault has been armed via {@link #armGuiderFault}, it is thrown
     * instead and the state is left unchanged. Otherwise, if {@code command} is
     * not legal from the current state, an {@link #INVALID_TRANSITION}
     * {@link DAQException} is thrown and the state is left unchanged.
     *
     * @param command the guider command being issued
     * @return the status after the transition
     * @throws DAQException if a fault is armed, or the command is illegal from
     * the current state
     */
    synchronized Status advanceGuider(GuiderCommand command) throws DAQException {
        consumeGuiderFault();
        State previous = guiderState;
        State target = TRANSITIONS.get(previous).get(command);
        if (target == null) {
            throw new DAQException(
                    String.format("Guider %s failed, status %d", commandName(command), INVALID_TRANSITION),
                    INVALID_TRANSITION, "GDS - Invalid transition requested");
        }
        guiderState = target;
        return GuiderSimulationSupport.status(previous, target);
    }

    private static String commandName(GuiderCommand command) {
        return command.name().toLowerCase();
    }

    /**
     * Returns a Status for the current guider state without transitioning (in ==
     * out == current state). Used for {@code guiderConfig}-style queries.
     */
    synchronized Status currentGuiderStatus() throws DAQException {
        consumeGuiderFault();
        return GuiderSimulationSupport.status(guiderState, guiderState);
    }

    /**
     * Returns a Status for the current guider state without transitioning and
     * without consuming any armed fault. Used by {@code playGuider}, which does
     * not change the guider state and is not a fault-injection point (and whose
     * API does not declare a checked exception).
     */
    synchronized Status peekGuiderStatus() {
        return GuiderSimulationSupport.status(guiderState, guiderState);
    }

    /**
     * Returns a {@link SeriesStatus} wrapping the current guider state plus a
     * stand-in {@link Series}.
     * <p>
     * PLACEHOLDER: the Series stand-in reports sequence=0, stamps=0 and empty
     * configured/remaining sensor lists. Stamp accumulation across a running
     * series is not modelled — fill in if a test needs a realistic stamp count.
     */
    synchronized SeriesStatus guiderSeriesStatus() throws DAQException {
        consumeGuiderFault();
        Status status = GuiderSimulationSupport.status(guiderState, guiderState);
        Series series = GuiderSimulationSupport.series(0L, 0, 0,
                Collections.emptyList(), Collections.emptyList());
        // TODO(LSSTCCSDRIVER-498): second Series is the "idle" series; reuse the
        // same stand-in until a test needs the two distinguished.
        return new SeriesStatus(status, series, series);
    }

    /**
     * Throws and disarms the armed fault, if any. One-shot.
     */
    private void consumeGuiderFault() throws DAQException {
        DAQException fault = guiderFault;
        if (fault != null) {
            guiderFault = null;
            throw fault;
        }
    }

    public void addTriggerListener(TriggerListener listener) {
        listeners.add(listener);
    }

    public void removeTriggerListener(TriggerListener listener) {
        listeners.remove(listener);
    }

    public void setRegisterAccess(RegisterAccess ra) {
        registerAccess = ra;
    }

    void fireTrigger(int opcode, ImageMetaData meta, Map<Location.LocationType, int[]> registerLists) {
        for (TriggerListener l : listeners) {
            l.trigger(opcode, meta, registerLists);
        }
    }

    int readRegister(Location l, int address) {
        if (registerAccess != null) {
            return registerAccess.readRegister(l, address);
        } else {
            return 0;
        }
    }

    void writeRegister(Location l, int address, int value) {
        if (registerAccess != null) {
            registerAccess.writeRegister(l, address, value);
        }
    }

    public static interface TriggerListener {

        void trigger(int opcode, ImageMetaData meta, Map<Location.LocationType, int[]> registerLists);
    }

    public static interface RegisterAccess {

        int readRegister(Location l, int address);

        void writeRegister(Location l, int address, int value);
    }
}
