package org.lsst.ccs.daq.ims;

import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;
import org.lsst.ccs.utilities.location.Location;

/**
 * Utilities for interacting with a simulated store
 *
 * @author tonyj
 */
public class StoreSimulation {

    private final List<TriggerListener> listeners = new CopyOnWriteArrayList<>();
    private RegisterAccess registerAccess;

    private static final StoreSimulation singleton = new StoreSimulation();

    private StoreSimulation() {

    }

    public static StoreSimulation instance() {
        return singleton;
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
