# Specification file for primary header, DAQv4 specific items
#
IMAGETAG    String  ${Tag}                      Image tag (Hex)
CCD_MANU    String  ${CCDManufacturer}          CCD Manufacturer
CCD_TYPE    String  ${CCDModel}                 CCD Model Number
CCD_SERN    String  ${CCDSerialManufacturer}    Manufacturers’ CCD Serial Number
CCDSLOT     String  ${CCDSlot}                  The CCD Slot
RAFTBAY     String  ${RAFTBay}                  The RAFT Bay
FIRMWARE    String  ${Firmware}                 DAQ firmware version (Hex)
PLATFORM    String  ${Platform}                 DAQ platform version
SERIAL      String  ${SerialNumber}             ?
DAQVERS     String  ${DAQVersion}               DAQ version
DAQPART     String  ${DAQPartition}             DAQ partition
DAQFOLD     String  ${DAQFolder}                DAQ folder the image was initially created in
DAQANNOT    String  ${DAQAnnotation}            DAQ image annotation
# Elements from the image name, see also TSEIA-79
OBSID       String  ${ImageName}                The image name or obs-id
TELCODE     String  ${ImageSource}              The "code" for AuxTel
CONTRLLR    String  ${ImageController}          The controller (e.g. O for OCS, C for CCS)
DAYOBS      String  ${ImageDate}                The observation day as defined in the image name 
SEQNUM      Integer ${ImageNumber}              The sequence number from the image name