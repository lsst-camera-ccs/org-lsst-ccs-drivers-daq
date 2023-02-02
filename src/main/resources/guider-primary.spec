# Specification file for guider  primary header
#
DATE-OBS!    Date    ${FileCreationTime}           File creation time

CCDSLOT!     String  ${CCDSlot}                    The CCD Slot
RAFTBAY!     String  ${RaftBay}                    The RAFT Bay
FIRMWARE!    String  ${Firmware}                   DAQ firmware version (Hex)
PLATFORM!    String  ${Platform}                   DAQ platform version
CONTNUM!     String  ${CCDControllerSerial|_NULL_} REB serial # (Hex)
DAQVERS!     String  ${DAQVersion}                 DAQ version
DAQPART!     String  ${DAQPartition}               DAQ partition
#
ROICOLS!     Integer ${ROICols}                    Guider ROI columns
ROIROWS!     Integer ${ROIRows}                    Guider ROI rows
ROISEG!      String  ${ROISegment}                 Guider ROI segment
GDSSEQ!      Integer ${DAQSequence}                Guider sequence number  
GDSSTART!    Date    ${StartTime}                  Guider start time
#
# Elements from the image name, see also TSEIA-79
OBSID!       String  ${ImageName}                  The image name or obs-id
CAMCODE!     String  ${ImageSource}                The "code" for AuxTel | ComCam | Main Camera
CONTRLLR!    String  ${ImageController}            The controller (e.g. O for OCS, C for CCS)
DAYOBS!      String  ${ImageDate}                  The observation day as defined in the image name 
SEQNUM!      Integer ${ImageNumber}                The sequence number from the image name