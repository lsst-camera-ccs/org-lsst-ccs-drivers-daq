package org.lsst.ccs.drivers.daq.ims.example;

import java.util.Date;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
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
    @Command(name="open", description="Open connection to DAQ partition")
    public void open(@Argument(name="partition", description="DAQ partition")
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
    * Print all DAQ Rms statistics data for speified Location
    *
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
            table += String.format("\n" + key + "%-22d", statMap.get(key));
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

}
