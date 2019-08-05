package org.lsst.ccs.daq.ims;

/**
 * An exception thrown to indicate a problem with the connection to the DAQ
 * @author tonyj
 */
public class DAQException extends Exception {

    private final int rc;
    DAQException(String message) {
        super(message);
        rc = -1;
    }
    
    DAQException(String message, int rc) {
        this(message, rc, Store.decodeException(rc));
    }

    DAQException(String message, int rc, String decoded) {
        super(String.format("%s (rc=%d %s)", message, rc, decoded));
        this.rc = rc;
    }

    public int rc() {
        return rc;
    }
}
