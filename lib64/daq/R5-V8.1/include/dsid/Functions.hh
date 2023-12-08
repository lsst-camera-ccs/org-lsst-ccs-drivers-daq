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

#include "dsi/Header.hh"

#define PROBE_ADDRESS   DSI::Header::F0  // Return server network address
#define PROBE_RELEASE   DSI::Header::F1  // Get release information
#define ASSIGN          DSI::Header::F4  // Create an (empty) partition) 
#define LOOKUP          DSI::Header::F5  // Return ID (lookup by name)
#define REVERSE         DSI::Header::F6  // Return name (lookup by ID) 
#define GATHER          DSI::Header::F7  // Return ALL IDs 
#define REMOVE          DSI::Header::F8  // Delete an existing name

#endif

