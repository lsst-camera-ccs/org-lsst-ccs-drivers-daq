package org.lsst.ccs.daq.imageapi;

/**
 * An interface to be implemented by code which wants to be notified about creation 
 * of new images.
 * <p>
 * Note: Calling of image listeners is done on a single thread, so implementations 
 * should not perform any time consuming work in the callback.
 * </p>
 * 
 * @see Store#addImageListener(ImageListener) 
 * @see Store#removeImageListener(ImageListener)
 * @author tonyj
 */
public interface ImageListener {
    /**
     * Called when an image is created in the Store, but before all of the data
     * associated with the image has been persisted in the store. This is useful
     * when using streaming, as it is possible to open the sources associated with
     * the image for streaming data once the image is created.
     * @param image The image being delivered.
     * @see Source#openChannel(Source.ChannelMode) 
     */
    void imageCreated(Image image);
    /**
     * Called when the newly created image is complete, including all associated data.
     * @param image The image being delivered.
     */
    void imageComplete(Image image);
}
