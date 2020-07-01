package org.lsst.ccs.daq.ims;

import java.util.BitSet;
import java.util.List;

/**
 *
 * @author tonyj
 */
public class StoreSimulatedImplementation implements StoreImplementation {

    @Override
    public long attachStore(String partition) throws DAQException {
        return 100;
    }

    @Override
    public void detachStore(long store) throws DAQException {
        
    }

    @Override
    public long capacity(long store) throws DAQException {
        return 500_000_000_000L;
    }

    @Override
    public long remaining(long store) throws DAQException {
        return 250_000_000_000L;
    }

    @Override
    public void listFolders(long store, List<String> result) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int insertFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int removeFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean findFolder(long store, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void listImages(long store, String folderName, List<ImageMetaData> result) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int moveImageToFolder(long store, long id, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int deleteImage(long store, long id) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public SourceMetaData findSource(long store, long id, int location) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public ImageMetaData addImageToFolder(long store, String imageName, String folderName, String annotation, int opcode, BitSet elements) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public ImageMetaData findImage(long store, String imageName, String folderName) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public long openSourceChannel(long store, long id, int index, boolean write) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public SourceMetaData addSourceToImage(long store, long id, int index, byte type, String platform, int[] registerValues) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int waitForImage(long store, int imageTimeoutMicros, int sourceTimeoutMicros) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public String decodeException(int rc) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public BitSet getConfiguredSources(long store) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public Version getClientVersion() throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void setRegisterList(long store, boolean science, boolean guider, int[] registerAddresses) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public ImageMetaData triggerImage(long store, String metaData, String name, String annotation, int opcode, BitSet locationBitSet) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public long startSequencer(long store, int opcode) throws DAQException {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
