package org.lsst.ccs.daq.ims;

import java.util.LinkedHashMap;
import java.util.Map;
import org.lsst.ccs.utilities.location.Location;
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
    
    public Map<Location, int[]> readRegisters(LocationSet locations, int... addresses) throws DAQException {
        int[][] values =  store.readRegisters(client, locations.getBitSet(), addresses);
        Map<Location, int[]> result = new LinkedHashMap<>();
        for (Location l : locations) {
            result.put(l, values[l.index()]);
        }
        return result;
    }
}
