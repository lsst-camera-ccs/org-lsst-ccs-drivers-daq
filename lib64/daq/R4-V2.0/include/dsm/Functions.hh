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

#include "dsi/Frame.hh"

#define PARTITION_GET_CURRENT    DSI::Frame::F4   // Return current NFS mount-point 
#define PARTITION_GET_PENDING    DSI::Frame::F5   // Return pending NFS mount-point 
#define PARTITION_SET            DSI::Frame::F6   // Assign NFS mount-point 

#define MOUNTPOINT_GET_CURRENT   DSI::Frame::F7   // Return current NFS mount-point 
#define MOUNTPOINT_GET_PENDING   DSI::Frame::F8   // Return pending NFS mount-point 
#define MOUNTPOINT_SET           DSI::Frame::F9   // Assign NFS mount-point 

#define RELEASEPOINT_GET_CURRENT DSI::Frame::F10   // Get (current) release directory 
#define RELEASEPOINT_GET_PENDING DSI::Frame::F11   // Get (pending) release directory 
#define RELEASEPOINT_SET         DSI::Frame::F12   // Assign release directory

#define MODE_GET_CURRENT         DSI::Frame::F13   // Get (current) mode
#define MODE_GET_PENDING         DSI::Frame::F14   // Get (pending) mode
#define MODE_SET                 DSI::Frame::F15  // Assign mode

#define ROLE_GET_CURRENT         DSI::Frame::F16  // Return (current) role
#define ROLE_GET_PENDING         DSI::Frame::F17  // Return (pending) role
#define ROLE_SET                 DSI::Frame::F18  // Assign role
 
#define PATH_GET_CURRENT         DSI::Frame::F20  // Get (current) paths
#define PATH_GET_PENDING         DSI::Frame::F21  // Get (pending) paths
#define PATH_SET                 DSI::Frame::F22  // Assign path (to a single index)

#define SOURCE_GET_CURRENT       DSI::Frame::F24  // Return (current) sources
#define SOURCE_GET_PENDING       DSI::Frame::F25  // Return (pending) sources
#define SOURCE_SET               DSI::Frame::F26  // Assign source (to a single index)
 
#endif

