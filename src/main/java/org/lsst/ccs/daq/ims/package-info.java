/**
 * An interface to the DAQ image services (ims) library. 
 * <p>
 * The goal of this package is to provide a lightweight wrapper around the corresponding DAQ library. 
 * All of the DAQ features are exposed, but using Java like idioms rather than C++ idiom. 
 * <p>
 * This package allows:
 * <ul>
 * <li>To connect to a DAQ store associated with a named partition</li>
 * <li>To access the Catalog associated with the store, to view and modify folders.</li>
 * <li>To access Images and their corresponding Sources</li>
 * <li>To create new Images and write them into the store</li>
 * <li>To listen for new Images</li>
 * <li>To read and write data associated with image sources, including the ability to stream data as
 * it is written to the store. Note that this package only deals with reading and writing raw (multiplexed, packed 18 bit) data. 
 * Utilities in the decoder package provide support for packing and unpacking this data.</li>
 * </ul>
 * 
 * @see org.lsst.ccs.daq.ims.Store
 * @see org.lsst.ccs.daq.ims.Catalog
 * @see org.lsst.ccs.daq.ims.Image
 * @see org.lsst.ccs.daq.ims.Source
 */
package org.lsst.ccs.daq.ims;