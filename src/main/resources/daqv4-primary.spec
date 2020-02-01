# Specification file for primary header, DAQv4 specific items
#
# Temporary fix for LSSTCCSRAFTS-488
DATE-OBS!    Date    ${DAQTriggerTime}           Date of the image trigger (readout), UTC for now
MJD-OBS!     MJD     ${DAQTriggerTime}           Modified Julian Date of image trigger
#
DATE-TRG!    Date    ${DAQTriggerTime}           Date of the image trigger (readout), UTC for now
MJD-TRG!     MJD     ${DAQTriggerTime}           Modified Julian Date of image trigger
IMAGETAG!    String  ${Tag}                      DAQ Image id (Hex)
CCDSLOT!     String  ${CCDSlot}                  The CCD Slot
RAFTBAY!     String  ${RAFTBay}                  The RAFT Bay
FIRMWARE!    String  ${Firmware}                 DAQ firmware version (Hex)
PLATFORM!    String  ${Platform}                 DAQ platform version
CONTNUM!     String  ${CCDControllerSerial}      REB serial # (Hex)
DAQVERS!     String  ${DAQVersion}               DAQ version
DAQPART!     String  ${DAQPartition}             DAQ partition
DAQFOLD!     String  ${DAQFolder}                DAQ folder the image was initially created in
DAQANNOT!    String  ${DAQAnnotation}            DAQ image annotation
# Elements from the image name, see also TSEIA-79
OBSID!       String  ${ImageName}                The image name or obs-id
TELCODE!     String  ${ImageSource}              The "code" for AuxTel
CONTRLLR!    String  ${ImageController}          The controller (e.g. O for OCS, C for CCS)
DAYOBS!      String  ${ImageDate}                The observation day as defined in the image name 
SEQNUM!      Integer ${ImageNumber}              The sequence number from the image name