package org.lsst.ccs.daq.ims;

import org.lsst.ccs.utilities.location.Location;
import java.io.Serializable;
import java.util.Arrays;
import org.lsst.ccs.utilities.location.Location.LocationType;

/**
 *
 * @author tonyj
 */
public class SourceMetaData implements Serializable {

    private static final long serialVersionUID = 397370667275465591L;

    private final Location location;
    private final LocationType sensor;
    private final byte lane;
    private final String platform;
    private final Version software;
    private final int firmware;
    private final long serialNumber;
    private final long length;
    private final int[] registerValues;

    SourceMetaData(byte sensor, byte lane, String platform, Version software, int firmware, long serialNumber, long length, byte bay, byte board, int[] registerValues) {
        this.sensor = Location.LocationType.values()[sensor - 1];
        this.lane = lane;
        this.platform = platform;
        this.software = software;
        this.firmware = firmware;
        this.serialNumber = serialNumber;
        this.length = length;
        this.location = new Location(bay, board);
        this.registerValues = registerValues;
    }

    public LocationType getSensor() {
        return sensor;
    }

    public byte getLane() {
        return lane;
    }

    public String getPlatform() {
        return platform;
    }

    public Version getSoftware() {
        return software;
    }

    public int getFirmware() {
        return firmware;
    }

    public long getSerialNumber() {
        return serialNumber;
    }

    public long getLength() {
        return length;
    }

    public Location getLocation() {
        return location;
    }

    public int[] getRegisterValues() {
        return registerValues;
    }

    @Override
    public String toString() {
        return String.format("SourceMetaData{" + "location=%s, sensor=%s, lane=%d, platform=%s, software=%s, firmware=%x, serialNumber=%x, length=%s, registerValues=%s}",
                location, sensor, lane, platform, software, firmware, serialNumber, Utils.humanReadableByteCount(length), Arrays.toString(registerValues));
    }
}
