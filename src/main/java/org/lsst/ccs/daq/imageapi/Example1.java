package org.lsst.ccs.daq.imageapi;

import java.util.Collections;
import java.util.List;

/**
 *
 * @author tonyj
 */
public class Example1 {

    public static void main(String[] args) throws DAQException {
        Store store = new Store("dev");
        System.out.printf("capacity %,d\n", store.getCapacity());
        System.out.printf("remaining %,d\n", store.getCurrent());
        final Catalog catalog = store.getCatalog();
        List<Folder> list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder insert = catalog.insert("test3");
        list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder find = catalog.find("test3");
        System.out.println(find.equals(insert));
        catalog.remove("test3");
        list = catalog.list();
        Collections.sort(list);
        for (Folder folder : list) {
            System.out.println(folder.getName());
        }
        Folder folder = catalog.find("emu");
        List<Image> images = folder.listImages();
        Collections.sort(images);
        images.forEach((image) -> {
            System.out.println(image.getMetaData());
        });
        
        Image image = images.get(0);
        List<Source> sources = image.listSources();
        Collections.sort(sources);
        for (Source source : sources) {
           System.out.println(source.getMetaData());
        }
    }
}
