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

#include "dsi/Header.hh"

#define PROBE    DSI::Header::F0  // Get configuration information...
#define RELEASE  DSI::Header::F1  // Get release information...
#define LOOKUP   DSI::Header::F2  // Return location & address for a specified source

#define TRIGGER  DSI::Header::F4  // Initiate acquisition on readout servers
#define SEQUENCE DSI::Header::F5 // Initiate acquisition on readout servers

#define WAKEUP   DSI::Header::F8  // Initiate acquisition on readout servers

#define STATS    DSI::Header::F9  // Collect RDS Stats

#define FLUSH    DSI::Header::F10 // Flush RDS Event Queue


 
#endif

