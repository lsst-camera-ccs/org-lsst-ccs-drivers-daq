package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.LocationSet;

/**
 * A getRegisterClient for reading and writing registers
 * @author tonyj
 */
public class RegisterClient {

    private final long client;
    private final Store store;

    RegisterClient(Store store, long client) {
        this.store = store;
        this.client = client;
    }
    
    void detach() throws DAQException {
        store.detachClient(this.client);
    }
    
    public int[] readRegisters(LocationSet locations, int address) throws DAQException {
        return store.readRegisters(client, locations.getBitSet(), address);
    }
}
