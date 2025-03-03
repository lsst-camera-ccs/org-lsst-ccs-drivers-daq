package org.lsst.ccs.daq.guider;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class ClearParameters {
    private final int delay;
    private final int preRows;
    private final int flushCount;
    private final int readRows;
    public final static String DEFAULT_CLEAR_PARAMETERS_STRING = "{\"delay\":50,\"preRows\":100,\"flushCount\":2,\"readRows\":0}";
    private final static ClearParameters DEFAULT_CLEAR_PARAMETERS = ClearParameters.parse(DEFAULT_CLEAR_PARAMETERS_STRING);

    @JsonCreator(mode = JsonCreator.Mode.PROPERTIES)
    public ClearParameters(@JsonProperty("delay") int delay, @JsonProperty("preRows") int preRows, @JsonProperty("flushCount") int flushCount, @JsonProperty("readRows") int readRows) {
        this.delay = delay;
        this.preRows = preRows;
        this.flushCount = flushCount;
        this.readRows = readRows;
    }

    public int getDelay() {
        return delay;
    }

    public int getPreRows() {
        return preRows;
    }

    public int getFlushCount() {
        return flushCount;
    }

    public int getReadRows() {
        return readRows;
    }

    public static ClearParameters parse(String clearSpec) throws IllegalArgumentException {
        try {
            ObjectMapper om = new ObjectMapper();
            return om.readValue(clearSpec, ClearParameters.class);
        } catch (JsonProcessingException x) {
            throw new IllegalArgumentException("Invalid ClearParameters " + clearSpec, x);
        }
    }

    @Override
    public String toString() {
        try {
            ObjectMapper om = new ObjectMapper();
            return om.writeValueAsString(this);
        } catch (JsonProcessingException ex) {
            throw new RuntimeException("Can't happen!!!", ex);
        }
    }

    public static ClearParameters defaultValue() {
        return DEFAULT_CLEAR_PARAMETERS;
    }
}
