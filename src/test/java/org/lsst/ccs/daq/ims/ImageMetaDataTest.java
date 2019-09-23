package org.lsst.ccs.daq.ims;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import org.lsst.ccs.utilities.location.LocationSet;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import org.junit.Assert;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import org.junit.Test;

/**
 *
 * @author tonyj
 */
public class ImageMetaDataTest {

    @Test
    public void imageMetaDataSerializableTest() throws IOException, ClassNotFoundException {
        ImageMetaData imd = new ImageMetaData("test", "testAnnnotation", 5, LocationSet.of("R22/Reb1"));
        Object result = serializeDeserialize(imd);
        assertTrue(result instanceof ImageMetaData);
        ImageMetaData imd2 = (ImageMetaData) result;
        assertEquals(imd.getAnnotation(), imd2.getAnnotation());
        assertEquals(imd.getName(), imd2.getName());
        assertEquals(imd.getOpcode(), imd2.getOpcode());
        assertEquals(imd.getLocations(), imd2.getLocations());
    }

    @Test
    public void sourceMetaDataSerializableTest() throws IOException, ClassNotFoundException {
        SourceMetaData smd = new SourceMetaData((byte) 2, (byte) 3, "platform", new Version("tag", 1000000, false, 99), 99, 0, 0, (byte) 0, (byte) 0, new int[]{1, 2, 3, 4, 5});
        Object result = serializeDeserialize(smd);
        assertTrue(result instanceof SourceMetaData);
        SourceMetaData smd2 = (SourceMetaData) result;
        assertEquals(smd.getFirmware(), smd2.getFirmware());
        assertEquals(smd.getLength(), smd2.getLength());
        Assert.assertArrayEquals(smd.getRegisterValues(), smd2.getRegisterValues());
        assertEquals(smd.getSerialNumber(), smd2.getSerialNumber());
    }

    static Object serializeDeserialize(Object in) throws IOException, ClassNotFoundException {
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
