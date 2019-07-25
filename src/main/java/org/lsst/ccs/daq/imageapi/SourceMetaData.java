package org.lsst.ccs.daq.imageapi;

import java.util.Arrays;
import org.lsst.ccs.daq.imageapi.Source.SourceType;

/**
 *
 * @author tonyj
 */
public class SourceMetaData {
    private final Location location;
    private final SourceType sensor;
    private final byte lane;
    private final String platform;
    private final Version software;
    private final int firmware;
    private final long serialNumber;
    private final int length;
    private final int[] registerValues;

    SourceMetaData(byte sensor, byte lane, String platform, Version software, int firmware, long serialNumber, int length, byte bay, byte board, int[] registerValues) {
        this.sensor = Source.SourceType.values()[sensor-1];
        this.lane = lane;
        this.platform = platform;
        this.software = software;
        this.firmware = firmware;
        this.serialNumber = serialNumber;
        this.length = length;
        this.location = new Location(bay,board);
        this.registerValues = registerValues;
    }

    public SourceType getSensor() {
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

    public int getLength() {
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
        return "SourceMetaData{" + "location=" + location + ", sensor=" + sensor + ", lane=" + lane + ", platform=" + platform + ", software=" + software + ", firmware=" + firmware + ", serialNumber=" + serialNumber + ", length=" + length + ", registerValues=" + Arrays.toString(registerValues) + '}';
    }
}
