/*
** ++
**  Package:
**  
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC, (415) 926-4269
**
**  Creation Date:
**	000 - June 20 1,1997
**
**  Revision History:
**	None.
**
** --
*/

#ifndef DSM_FUNCTIONS
#define DSM_FUNCTIONS

#include "dsi/Header.hh"

#define PARTITION      DSI::Header::F0  // Return either current or pending partition name 
#define MOUNTPOINT     DSI::Header::F1  // Return either current or pending NFS mount-point 
#define RELEASEPOINT   DSI::Header::F2  // Return either current or pending release directory 
#define ROLE           DSI::Header::F4  // Return either current or pending role
#define PATH           DSI::Header::F5  // Return either current or pending paths
#define SOURCE         DSI::Header::F6  // Return either current or pending sources
#define SEQUENCER      DSI::Header::F7  // Return either current or pending GDS Sequencer
#define VENDOR         DSI::Header::F15 // Return either current or pending sensor vendor
#define SENSORS        DSI::Header::F3  // Return either current or pending GDS Sensors

#define SET_PARTITION      DSI::Header::F8   // Assign partition name
#define SET_MOUNTPOINT     DSI::Header::F9   // Assign NFS mount-point 
#define SET_RELEASEPOINT   DSI::Header::F10  // Assign release directory
#define SET_ROLE           DSI::Header::F12  // Assign role
#define SET_PATH           DSI::Header::F13  // Assign path (to one or interfaces)
#define SET_SOURCE         DSI::Header::F14  // Assign source (to one or interfaces)
#define SET_SEQUENCER      DSI::Header::F23  // Assign GDS Sequencer
#define SET_VENDOR         DSI::Header::F24  // Assign Sensor vendor
#define SET_SENSORS        DSI::Header::F11  // Assign GDS Sensors

#define RESET_PARTITION    DSI::Header::F16  // Deassign partition name
#define RESET_MOUNTPOINT   DSI::Header::F17  // Deassign NFS mount-point 
#define RESET_RELEASEPOINT DSI::Header::F18  // Deassign release directory
#define RESET_ROLE         DSI::Header::F20  // Deassign role
#define RESET_PATH         DSI::Header::F21  // Deassign path (to one or interfaces)
#define RESET_SOURCE       DSI::Header::F22  // Deassign source (to one or interfaces)
#define RESET_SEQUENCER    DSI::Header::F25  // Deassign GDS Sequencer
#define RESET_VENDOR       DSI::Header::F26  // Deassign Sensor vendor
#define RESET_SENSORS      DSI::Header::F19  // Deassign GDS Sensors
 
#endif

