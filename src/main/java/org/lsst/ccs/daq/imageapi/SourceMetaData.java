package org.lsst.ccs.daq.imageapi;

/**
 *
 * @author tonyj
 */
public class SourceMetaData {
    private final Location location;
    private final byte sensor;
    private final byte lane;
    private final String platform;
    private final Version software;
    private final int firmware;
    private final long serialNumber;
    private final int length;

    SourceMetaData(byte sensor, byte lane, String platform, Version software, int firmware, long serialNumber, int length, byte bay, byte board) {
        this.sensor = sensor;
        this.lane = lane;
        this.platform = platform;
        this.software = software;
        this.firmware = firmware;
        this.serialNumber = serialNumber;
        this.length = length;
        this.location = new Location(bay,board);
    }

    public byte getSensor() {
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

    @Override
    public String toString() {
        return "SourceMetaData{" + "location=" + location + ", sensor=" + sensor + ", lane=" + lane + ", platform=" + platform + ", software=" + software + ", firmware=" + firmware + ", serialNumber=" + serialNumber + ", length=" + length + '}';
    }
}
