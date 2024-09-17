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

#ifndef EMU_FUNCTIONS
#define EMU_FUNCTIONS

#include "dsi/Header.hh"

#define XTRIGGER DSI::Header::F0  // Initiate transmission of playback image
#define XPLAY    DSI::Header::F4  // Load a playlist...
#define XSTOP    DSI::Header::F9  // Pause playback...
#define XRESTART DSI::Header::F10 // Continue playback...

#endif

