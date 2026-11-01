package org.lsst.ccs.daq.guider;

/**
 * A {@link Status} returned by a state-changing guider command, augmented with
 * how the command fared against the command-timeout reconciliation logic in
 * {@code Guider}.
 * <p>
 * {@link #getTimeouts()} and {@link #getRetries()} differ: a command whose
 * response was lost but which had actually taken effect reports at least one
 * timeout but zero retries (it was never re-issued), whereas a command that
 * genuinely did not reach the DAQ is re-issued and reports both.
 *
 * @author tonyj
 */
public class RetryStatus extends Status {

    private final int timeouts;
    private final int retries;

    /**
     * @param status the underlying status (from the DAQ, or from the state
     * query used to reconcile a timeout)
     * @param timeouts the number of times the command timed out
     * @param retries the number of times the command was re-issued
     */
    public RetryStatus(Status status, int timeouts, int retries) {
        super(status);
        this.timeouts = timeouts;
        this.retries = retries;
    }

    /**
     * @return the number of times the command timed out (a timeout does not
     * necessarily lead to a retry; see the class description)
     */
    public int getTimeouts() {
        return timeouts;
    }

    /**
     * @return the number of times the command was re-issued after a timeout
     */
    public int getRetries() {
        return retries;
    }

    @Override
    public String toString() {
        return "RetryStatus{" + "timeouts=" + timeouts + ", retries=" + retries + ", " + super.toString() + '}';
    }

}
