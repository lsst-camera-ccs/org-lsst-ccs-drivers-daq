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

#ifndef RDS_FUNCTIONS
#define RDS_FUNCTIONS

#include "dsi/Frame.hh"

#define PROBE    DSI::Frame::F0  // Get configuration information...
#define RELEASE  DSI::Frame::F1  // Get release information...
#define LOOKUP   DSI::Frame::F2  // Return location & address for a specified source

#define TRIGGER  DSI::Frame::F4  // Initiate acquisition on readout servers
#define SEQUENCE DSI::Frame::F5 // Initiate acquisition on readout servers

#define WAKEUP   DSI::Frame::F8  // Initiate acquisition on readout servers

#define STATS    DSI::Frame::F9  // Collect RDS Stats


 
#endif

