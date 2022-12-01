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

#ifndef PPS_FUNCTIONS
#define PPS_FUNCTIONS

#include "dsi/Header.hh"

#define PAIRING       DSI::Header::F0   //
#define SET           DSI::Header::F1   // 
#define RESET         DSI::Header::F2   // 
#define DUMP_COUNTERS DSI::Header::F3   // 
#define START         DSI::Header::F4   // 
#define STOP          DSI::Header::F5   // 
#define DUMP_RESULTS  DSI::Header::F6   // 
#define HAMMER        DSI::Header::F7   // 
#define PING          DSI::Header::F8   // 
#define PONG          DSI::Header::F9   // 

#endif

