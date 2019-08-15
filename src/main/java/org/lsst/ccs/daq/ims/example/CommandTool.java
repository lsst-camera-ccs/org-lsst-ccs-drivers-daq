package org.lsst.ccs.daq.ims.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ByteChannel;
import java.util.Collections;
import java.util.List;
import org.lsst.ccs.command.annotations.Argument;
import org.lsst.ccs.command.annotations.Command;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.Folder;
import org.lsst.ccs.daq.ims.Image;
import org.lsst.ccs.daq.ims.Source;
import org.lsst.ccs.daq.ims.SourceMetaData;
import org.lsst.ccs.daq.ims.Store;

/**
 *
 * @author tonyj
 */
public class CommandTool {

    private Store store;
    @Command(name="connect", description="Connect to a DAQ store")
    public void connect(@Argument(name="partition", description="Partition name") String partition) throws DAQException
    {
        if (store != null) store.close();
        store = new Store(partition);
    }
    
    @Command(name="close", description="Close DAQ store")
    public void close() throws DAQException {
        if (store != null) {
            store.close();
            store = null;
        }
    }

    @Command(name="ls", description="List folders/files") 
    public void list(@Argument(name="folder", description="Path", defaultValue = "") String folderName) throws DAQException {
        checkStore();
        if (folderName.trim().isEmpty()) {
            List<Folder> list = store.getCatalog().list();
            Collections.sort(list);
            list.forEach((folder) -> {
                System.out.println(folder);
            });
            long capacity = store.getCapacity();
            long remaining = store.getRemaining();
            System.out.printf("%,d/%,d (%g3%%) used\n", capacity-remaining, capacity, 100.0*(capacity-remaining)/capacity);
        } else if (!folderName.contains("/")) {
            Folder folder = store.getCatalog().find(folderName);
            List<Image> images = folder.listImages();
            Collections.sort(images);
            images.forEach((image) -> {
                System.out.printf("%20s %s\n",image.getMetaData().getName(), image.getMetaData().getAnnotation());
            });
        } else {
            Image image = imageFromPath(folderName);
            System.out.printf("%20s %s\n",image.getMetaData().getName(), image.getMetaData().getAnnotation());
            List<Source> sources = image.listSources();
            Collections.sort(sources);
            for (Source source : sources) {
                SourceMetaData smd = source.getMetaData();
                System.out.printf("   %s %s\n",smd.getLocation(), smd.getLength());                
            }
        }
    }

    @Command(name="mkdir", description="Create new folder") 
    public void mkdir(String folderName) throws DAQException {
        checkStore();
        store.getCatalog().insert(folderName);
    }

    @Command(name="rmdir", description="Delete folder") 
    public void rmdir(String folderName) throws DAQException {
        checkStore();
        store.getCatalog().remove(folderName);
    }
    
    @Command(name="rm", description="Delete image") 
    public void rm(String path) throws DAQException {
        checkStore();
        Image image = imageFromPath(path);
        image.delete();
    }

    @Command(name="mv", description="Move image")     
    public void mv(String path, String targetFolderName) throws DAQException {
        checkStore();
        Image image = imageFromPath(path);        
        Folder target = store.getCatalog().find(targetFolderName);
        if (target == null) {
            throw new RuntimeException("No such folder: "+target);
        }
        image.moveTo(targetFolderName);
    }
    
    @Command(name="read", description="Read and decode data in image")
    public void read(String path) throws DAQException, IOException {
        checkStore();
        Image image = imageFromPath(path);        
        List<Source> sources = image.listSources();
        long totalSize = 0;
        for (Source source : sources) {
            totalSize += source.size();
        }
        System.out.printf("Expected size %,d bytes\n", totalSize);

        ByteBuffer buffer = ByteBuffer.allocateDirect(1_000_000);
        long totalReadSize = 0;
        long start = System.nanoTime();
        for (Source source : sources) {
            try (ByteChannel channel = source.openChannel(Source.ChannelMode.READ)) {
                for (;;) {
                    buffer.clear();
                    int l = channel.read(buffer);
                    if (l < 0) {
                        break;
                    }
                    totalReadSize += l;
                }
            }
        }
        long stop = System.nanoTime();
        System.out.printf("Read %,d bytes in %,dns (%d MBytes/second)\n", totalReadSize, (stop - start), 1000 * totalReadSize / (stop - start));        
    }
    
    private Image imageFromPath(String path) throws DAQException, RuntimeException {
        String[] tokens = path.split("/");
        if (tokens.length != 2) {
            throw new RuntimeException("Illegal path: "+path);
        }
        Folder folder = store.getCatalog().find(tokens[0]);
        if (folder == null) {
            throw new RuntimeException("No such folder: "+tokens[0]);
        }
        Image image = folder.find(path);
        if (image == null) {
            throw new RuntimeException("No such image: "+tokens[1]);
        }
        return image;
    }

    private void checkStore() {
        if (store == null) {
            throw new RuntimeException("Please connect to store first");
        }
    }
}