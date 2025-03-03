BLANK Basic
BLANK Basic ---- Date, night and basic image information ----
BLANK CommCamInfo
BLANK CommCamInfo ---- Information from Camera (Common block) ----
BLANK CamInfo
BLANK CamInfo ---- Information from Camera ----
#
BLANK Guider
BLANK Guider ---- Guider ROI Info ----
Guider:ROICOL!      Integer ${ROIStartCol}                Guider ROI start column
Guider:ROIROW!      Integer ${ROIStartRow}                Guider ROI start row
Guider:ROICOLS!     Integer ${ROICols}                    Guider ROI columns
Guider:ROIROWS!     Integer ${ROIRows}                    Guider ROI rows
Guider:ROISEG!      String  ${ROISegment}                 Guider ROI segment
Guider:GDSSEQ!      Integer ${DAQSequence}                Guider sequence number
Guider:GDSSTART!    Date    ${StartTime}                  Guider start time
Guider:ROIUNDRC     Integer ${ROIUnderCols}               Guider ROI undercols
Guider:ROIOVERC     Integer ${ROIOverCols}                Guider ROI overcols
Guider:ROIOVERR     Integer ${ROIOverRows}                Guider ROI overrows
Guider:ROIFLUSH     Integer ${ROIFlushCount}              Guider ROI flushcount
Guider:ROISPLIT     Boolean ${ROISplit}                   Guider ROI split?
Guider:ROICCDTY     Integer ${ROICCDType}                 Guider CCD type
Guider:ROIMILLI     Integer ${IntegrationTime}            Guider Integration time millis
#
# Additional headers requested by DM (https://jira.slac.stanford.edu/browse/LSSTCCSDRIVER-437)
BLANK GuideDM
BLANK GuideDM ---- Guider DM Info ----
GuideDM:N_STAMPS!    Integer ${StampCount}                 The number of stamps in file
GuideDM:HIERARCH.HAS_ARCHIVE  Boolean false
GuideDM:HAS_MASK     Boolean false
GuideDM:HIERARCH.HAS_VARIANCE Boolean false
GuideDM:STAMPCLS     String lsst.meas.algorithms.Stamps
#
# Override ! on headers that will not appear
Basic:DATE-OBS    Date    ${startIntegrationTime}     Time at the start of integration
Basic:MJD-OBS     MJD     ${startIntegrationTime}     Modified Julian Date derived from DATE-OBS
Basic:DATE-TRG    Date    ${DAQTriggerTime}           TAI Date of the image trigger (readout)
Basic:MJD-TRG     MJD     ${DAQTriggerTime}           Modified Julian Date of image trigger
#
CommCamInfo:IMAGETAG    String  ${Tag}                      DAQ Image id (Hex)
CommCamInfo:OBSANNOT    String  ${DAQAnnotation}            DAQ image annotation
#
CamInfo:DAQFOLD     String  ${DAQFolder}              DAQ folder the image was initially created in
