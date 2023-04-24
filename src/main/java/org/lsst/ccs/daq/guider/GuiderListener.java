package org.lsst.ccs.daq.guider;

import java.nio.ByteBuffer;

/**
 * A listener for guider events
 */
public interface GuiderListener {

    /**
     * Guider is started
     *
     * @param state The meta-data at start time
     * @param series The meta-data corresponding to the series of postage
     * stamps
     * @throws java.lang.Exception
     */
    void start(StateMetaData state, SeriesMetaData series) throws Exception;

    void stop(StateMetaData state) throws Exception;

    void pause(StateMetaData state) throws Exception;

    void resume(StateMetaData state) throws Exception;

    /**
     * Called each time there is a single postage stamp
     *
     * @param state The current state, including the timestamp of the stamp
     * @param stamp The data corresponding to the stamp (in what format??)
     */
    void stamp(StateMetaData state, ByteBuffer stamp) throws Exception;

    /**
     * Called each time there is a new postage stamp
     *
     * @param state The current state, including the timestamp of the stamp
     * @param rawStamp The data corresponding the the raw postage stamp.
     */
    void rawStamp(StateMetaData state, ByteBuffer rawStamp) throws Exception;
    
}
