package org.lsst.ccs.daq.guider;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class ClearParameterTest {

    @Test
    public void parseTest() {
        ClearParameters clearParameters = ClearParameters.parse("{\"delay\":50,\"preRows\":100,\"flushCount\":2,\"readRows\":100}");
        assertEquals(2, clearParameters.getFlushCount());
        assertEquals(100, clearParameters.getReadRows());
        assertEquals(50, clearParameters.getDelay());
        assertEquals(100, clearParameters.getPreRows());
        assertEquals(0, clearParameters.getPostRows());
        assertEquals(0, clearParameters.getOverRows());
    }

    @Test
    public void testToString() {
        ClearParameters clearParameters = ClearParameters.defaultValue();
        assertEquals(ClearParameters.DEFAULT_CLEAR_PARAMETERS_STRING, clearParameters.toString());
    }

}
