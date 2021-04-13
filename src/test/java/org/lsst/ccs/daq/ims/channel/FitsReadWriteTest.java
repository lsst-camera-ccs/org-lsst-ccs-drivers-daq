package org.lsst.ccs.daq.ims.channel;

import java.io.File;
import java.io.IOException;
import java.nio.IntBuffer;
import java.util.Collections;
import java.util.Map;
import nom.tam.fits.FitsException;
import nom.tam.fits.FitsFactory;
import nom.tam.fits.Header;
import nom.tam.fits.TruncatedFileException;
import nom.tam.util.BufferedFile;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.daq.ims.ImageMetaData;
import org.lsst.ccs.daq.ims.channel.FitsIntWriter.PerCCDMetaDataProvider;
import org.lsst.ccs.daq.ims.example.FitsFile;
import org.lsst.ccs.utilities.ccd.CCD;
import org.lsst.ccs.utilities.ccd.FocalPlane;
import org.lsst.ccs.utilities.ccd.Reb;
import org.lsst.ccs.utilities.image.FitsHeaderMetadataProvider;
import org.lsst.ccs.utilities.image.FitsHeadersSpecificationsBuilder;
import org.lsst.ccs.utilities.image.MetaDataSet;
import org.lsst.ccs.utilities.location.Location;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * Not a real test
 * @author tonyj
 */
public class FitsReadWriteTest {

    static {
        FitsFactory.setUseHierarch(true);
    }

    public static void main(String[] args) throws IOException, TruncatedFileException, FitsException, DAQException {
        // We are going to read 3 FITS files
        File[] files = new File[3];
        files[0] = new File("/home/tonyj/Data/ComCam/tiago/09006001-R22-S10-det093-000.fits");
        files[1] = new File("/home/tonyj/Data/ComCam/tiago/09006001-R22-S11-det094-000.fits");
        files[2] = new File("/home/tonyj/Data/ComCam/tiago/09006001-R22-S12-det095-000.fits");
        int[] registerValues = {};
        int i = 0;
        for (File file : files) {
            try (BufferedFile bf = new BufferedFile(file, "r")) {
                Header primary = new Header(bf);
                Header image = new Header(bf);
                FitsFile fitsFile = new FitsFile(file, primary, image);
                registerValues = fitsFile.getReadOutParameters();
            }
        }
        IntBuffer buffer = IntBuffer.allocate(522 * 2010 * 48 * 9 / 16);
        int length = 0;

        try (FitsIntReader reader = new FitsIntReader(Location.LocationType.SCIENCE, false, files)) {
            for (;;) {
                int l = reader.read(buffer);
                if (l <= 0) {
                    break;
                }
                length += l;
            }
        }
        System.out.println(length);
        System.out.println(buffer.position());
        buffer.flip();
        System.out.println(buffer.remaining());

        FitsHeadersSpecificationsBuilder headerSpecBuilder = new FitsHeadersSpecificationsBuilder();
        headerSpecBuilder.addSpecFile("primary.spec");
        headerSpecBuilder.addSpecFile("daqv4-primary.spec", "primary");
        headerSpecBuilder.addSpecFile("extended.spec");

        // OK, now lets try writing it out again!
        FocalPlane focalPlane = FocalPlane.createFocalPlane();
        Reb reb = focalPlane.getReb("R22/Reb1");
        FitsIntWriter.FileNamer namer = (Map<String, Object> props) -> new File(String.format("/home/tonyj/Data/ComCam/tiago/test-%s-%s.fits", props.get("RaftBay"), props.get("CCDSlot")));
        ImageMetaData imd = new ImageMetaData("testName", "folder", "annotation", length, LocationSet.of("R22/Reb1"));
        PerCCDMetaDataProvider extra = (CCD ccd) -> {
            FitsHeaderMetadataProvider provider = new FitsHeaderMetadataProvider(){
                @Override
                public MetaDataSet getPrimaryHeaderMetadata() {
                    MetaDataSet mds = new MetaDataSet();
                    mds.addMetaData("primary", "Firmware", "99");
                    mds.addMetaData("primary", "Platform", "DAQ=99");
                    mds.addMetaData("primary", "CCDControllerSerial", "123456789");
                    mds.addMetaData("primary", "DAQVersion", "99.999");
                    return mds;
                }
                
                @Override
                public MetaDataSet getDataExtendedHeaderMetadata(String imageExtensionName) {
                    return getPrimaryHeaderMetadata();
                }
                
                @Override
                public MetaDataSet getAdditionalExtendedHeaderMetadata(String extendedKeyword) {
                    return null;
                }
            };
            return Collections.singletonList(provider);
        };
        try (FitsIntWriter decompress = new FitsIntWriter(imd, "test", reb, namer)) {
            decompress.completeInitialization(Location.LocationType.SCIENCE, Location.of("R22/Reb1"), registerValues, headerSpecBuilder.getHeaderSpecifications(), extra);
            decompress.write(buffer);
        }

    }
}
