package org.lsst.ccs.daq.ims;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import static org.junit.Assert.assertEquals;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class SerializationTest {

    @Test
    public void serializeDAQRmsStats() throws IOException, ClassNotFoundException {
        DAQRmsStats stats = new DAQRmsStats((byte) 0, (byte) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        DAQRmsStats copy = serializeDeserialize(stats);
        assertEquals(stats.getLink_down(), copy.getLink_down());
    }

    private <T extends Serializable> T serializeDeserialize(T stats) throws IOException, ClassNotFoundException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try (ObjectOutputStream oos = new ObjectOutputStream(out)) {
            oos.writeObject(stats);
        }
        out.close();
        byte[] byteArray = out.toByteArray();

        try (ByteArrayInputStream in = new ByteArrayInputStream(byteArray);
                ObjectInputStream ois = new ObjectInputStream(in)) {
            return (T) ois.readObject();
        }
    }
}
