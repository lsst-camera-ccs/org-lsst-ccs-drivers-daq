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

#ifndef DSID_FUNCTIONS
#define DSID_FUNCTIONS

#include "dsi/Frame.hh"

#define PROBE_ADDRESS   DSI::Frame::F0  // Return server network address
#define PROBE_RELEASE   DSI::Frame::F1  // Get release information
#define ASSIGN          DSI::Frame::F4  // Create an (empty) partition) 
#define LOOKUP          DSI::Frame::F5  // Return ID (lookup by name)
#define MAP             DSI::Frame::F6  // Return name (lookup by ID) 
#define FIND            DSI::Frame::F7  // Return ALL IDs 
#define REMOVE          DSI::Frame::F8  // Delete an existing name

#endif

