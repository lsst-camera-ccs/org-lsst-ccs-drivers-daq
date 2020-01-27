package org.lsst.ccs.daq.ims.example;

import java.util.Date;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.DAQDriverStats;
import org.lsst.ccs.daq.ims.DAQFirmwareStats;
import org.lsst.ccs.daq.ims.DAQRdsStats;
import org.lsst.ccs.daq.ims.DAQRmsStats;
import org.lsst.ccs.daq.ims.Stats;
import org.lsst.ccs.utilities.location.Location;

public class TestStats {

    private Stats stats;
    private static final Logger LOG = Logger.getLogger(TestStats.class.getName());

   /**
    *  Constructor
    */
    public TestStats() {}

    /**
     * Determines current time
     *
     * @return current time as a String
     */
    @Command(name="timestamp", description="Prints current time")
    public String timestamp() {
	Date now = new Date();
        return now.toString();
    }


   /**
    *  Opens connection to DAQ partition
    * 
    *  @param  Name of DAQ partition
    *  @throws DAQException
    */
    @Command(name="connect", description="Open connection to DAQ partition")
    public void connect(@Argument(name="partition", description="DAQ partition")
                        String partition) throws DAQException
    {
        if (stats != null) {
            stats.close();
        }
        stats = new Stats(partition);
    }

   /**
    *  Closes connection
    * 
    *  @throws  DAQException
    */
    @Command(name="close", description="Closes connection to DAQ partition")
    public void close() throws DAQException {
        if (stats != null) {
            stats.close();
            stats = null;
        }
    }


   /**
    * Print all four sets of statisics for specified location
    *
    * @param   Location in raft/board format, "R<nn>Reb<m>"
    * @return  String reporting all data read 
    * @throws  DAQException
    */
    @Command(name="readAllStats", description="Read all sets of DAQ stats")
    public String readAllStats(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location) throws DAQException {
        return ("\n" + readAllRms(location) +
                "\n" + readAllRds(location) + 
                "\n" + readAllDriver(location) + 
	        "\n" + readAllFirmware(location) + "\n");
    }

   /**
    * Print all DAQ Rms statistics data for specified Location
    * Reads all DAQRmsStatistics values and return them in table format.
    * 
    * @param   Location in raft/board format, "R<nn>Reb<m>"
    * @return  String reporting all data read 
    * @throws  DAQException
    */
    @Command(name="readAllRms", description="Read all DAQ Rms stats")
    public String readAllRms(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location) throws DAQException {
	DAQRmsStats rmsStats = stats.getDAQRmsStats(Location.of(location));
        Map<String,Long> statMap = rmsStats.getRmsStatMap();
        Set<String> keys = statMap.keySet();
        String table = "DAQ Rms Stats for " + location +"\n" + timestamp()+"\n";
        for (String key : keys) {
            table += String.format("\n  %-22s %d", key, statMap.get(key));
        }
        table += "\n";
        return table;
    }

   /**
    * Print all DAQ Rds statistics data for specified Location
    * Reads all DAQRdsStatistics values and return them in table format.
    * 
    * @param   Location in raft/board format, "R<nn>Reb<m>"
    * @return  String reporting all data read 
    * @throws  DAQException
    */
    @Command(name="readAllRds", description="Read all DAQ Rds stats")
    public String readAllRds(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location) throws DAQException {
	DAQRdsStats rdsStats = stats.getDAQRdsStats(Location.of(location));
        Map<String,Long> statMap = rdsStats.getRdsStatMap();
        Set<String> keys = statMap.keySet();
        String table = "DAQ Rds Stats for " + location +"\n" + timestamp()+"\n";
        for (String key : keys) {
            table += String.format("\n  %-22s %d", key, statMap.get(key));
        }
        table += "\n";
        return table;
    }

   /**
    * Print all DAQ Driver statistics data for specified Location
    * Reads all DAQDriverStatistics values and return them in table format.
    * 
    * @param   Location in raft/board format, "R<nn>Reb<m>"
    * @return  String reporting all data read 
    * @throws  DAQException
    */
    @Command(name="readAllDriver", description="Read all DAQ Driver stats")
    public String readAllDriver(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location) throws DAQException {
	DAQDriverStats driverStats = stats.getDAQDriverStats(Location.of(location));
        Map<String,Long> statMap = driverStats.getDriverStatMap();
        Set<String> keys = statMap.keySet();
        String table = "DAQ Driver Stats for " + location +"\n" + timestamp()+"\n";
        for (String key : keys) {
            table += String.format("\n  %-22s %d", key, statMap.get(key));
        }
        table += "\n";
        return table;
    }

   /**
    * Print all DAQ Firmware statistics data for specified Location
    * Reads all DAQFirmwareStatistics values and return them in table format.
    * 
    * @param   Location in raft/board format, "R<nn>Reb<m>"
    * @return  String reporting all data read 
    * @throws  DAQException
    */
    @Command(name="readAllFirmware", description="Read all DAQ Firmware stats")
    public String readAllFirmware(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location) throws DAQException {
	DAQFirmwareStats firmwareStats = stats.getDAQFirmwareStats(Location.of(location));
        Map<String,Long> statMap = firmwareStats.getFirmwareStatMap();
        Set<String> keys = statMap.keySet();
        String table = "DAQ Firmware Stats for " + location +"\n" + timestamp()+"\n";
        for (String key : keys) {
            table += String.format("\n  %-22s %d", key, statMap.get(key));
        }
        table += "\n";
        return table;
    }

   /**
    * Read specified entry in DAQ Rms Stats for specified location
    *
    * @param  Location in raft/board format, "R<nn>Reb<m>"
    * @param  Name of requested quantity
    * @return Value of requested quantity
    * @throws DAQException
    */
    @Command(name="readRmsStat", description="read specified DAQ Rms statistic for specified locaation")
	public String readRmsStat(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location, @Argument(name="statistic name") String quantity) throws DAQException {
	DAQRmsStats rmsStats = stats.getDAQRmsStats(Location.of(location));
        return rmsStats.getRmsStat(quantity).toString();
    }

   /**
    * Read specified entry in DAQ Rds Stats for specified location
    *
    * @param  Location in raft/board format, "R<nn>Reb<m>"
    * @param  Name of requested quantity
    * @return Value of requested quantity
    * @throws DAQException
    */
    @Command(name="readRdsStat", description="read specified DAQ Rds statistic for specified locaation")
	public String readRdsStat(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location, @Argument(name="statistic name") String quantity) throws DAQException {
	DAQRdsStats rdsStats = stats.getDAQRdsStats(Location.of(location));
        return rdsStats.getRdsStat(quantity).toString();
    }

   /**
    * Read specified entry in DAQ Driver Stats for specified location
    *
    * @param  Location in raft/board format, "R<nn>Reb<m>"
    * @param  Name of requested quantity
    * @return Value of requested quantity
    * @throws DAQException
    */
    @Command(name="readDriverStat", description="read specified DAQ Driver statistic for specified locaation")
	public String readDriverStat(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location, @Argument(name="statistic name") String quantity) throws DAQException {
	DAQDriverStats driverStats = stats.getDAQDriverStats(Location.of(location));
        return driverStats.getDriverStat(quantity).toString();
    }

   /**
    * Read specified entry in DAQ Firmware Stats for specified location
    *
    * @param  Location in raft/board format, "R<nn>Reb<m>"
    * @param  Name of requested quantity
    * @return Value of requested quantity
    * @throws DAQException
    */
    @Command(name="readFirmwareStat", description="read specified DAQ Firmware statistic for specified locaation")
	public String readFirmwareStat(@Argument(name="location", description = "readout board in format R<nn>Reb<m>") String location, @Argument(name="statistic name") String quantity) throws DAQException {
	DAQFirmwareStats firmwareStats = stats.getDAQFirmwareStats(Location.of(location));
        return firmwareStats.getFirmwareStat(quantity).toString();
    }

}
