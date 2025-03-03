# Specification file for guider stamps
#
NAXIS       Integer ${NAXIS}            number of axis
NAXIS1      Integer ${NAXIS1}           size of the n'th axis
NAXIS2      Integer ${NAXIS2}           size of the n'th axis
TIMESYS     String  TAI                 The time scale used
STMPTIME!   Date    ${StampTime}        Time stamp readout triggered
STMPTMJD!   MJD     ${StampTime}        MJD derived from STMPTIME
DATE-END    Date    ${StampTime}        End date of the observation
MJD-END     MJD     ${StampTime}        MJD derived from DATE-END

# DM requested stamps
EXTVER!     Integer ${StampCount}
EXTNAME     String  ${ExtName}