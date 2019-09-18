package org.lsst.ccs.daq.ims;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class LocationTest {

    @Test
    public void serializableTest() throws IOException, ClassNotFoundException {
        Location l = Location.of("R22/Reb1");
        Object result = serializeDeserilize(l);
        assertTrue(result instanceof Location);
        assertEquals(result, l);
    }

    @Test
    public void locationSetSerializableTest() throws IOException, ClassNotFoundException {
        LocationSet ls = LocationSet.of("R22/Reb1", "R11/Reb0");
        Object result = serializeDeserilize(ls);
        assertTrue(result instanceof LocationSet);
        assertEquals(result, ls);
    }
    
    @Test
    public void illegalTest() {
        try {
            Location l = Location.of("R22/RebX");
            fail("Should not get here");
        } catch (IllegalArgumentException x) {
            // OK;
        }
    }

    private Object serializeDeserilize(Object in) throws IOException, ClassNotFoundException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try (ObjectOutputStream oos = new ObjectOutputStream(baos)) {
            oos.writeObject(in);
        }
        ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
        try (ObjectInputStream ois = new ObjectInputStream(bais)) {
            return ois.readObject();
        }
    }
}
