package org.lsst.ccs.daq.guider;

public class SeriesStatus {
    private final Status status;
    private final Series series;
    private final Series idle;

    public SeriesStatus(Status status, Series series, Series idle) {
        this.status = status;
        this.series = series;
        this.idle = idle;
    }

    public Status getStatus() {
        return status;
    }

    public Series getSeries() {
        return series;
    }

    public Series getIdle() {
        return idle;
    }

    @Override
    public String toString() {
        return "SeriesStatus [status=" + status + ", series=" + series + ", idle=" + idle + "]";
    }

}
