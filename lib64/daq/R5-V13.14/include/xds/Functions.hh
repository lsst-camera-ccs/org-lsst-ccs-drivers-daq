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

#ifndef XDS_FUNCTIONS
#define XDS_FUNCTIONS

#include "dsi/Header.hh"

#define PROBE       DSI::Header::F0  // Return system configuration
#define RESET       DSI::Header::F1  // Reformat system configuration
#define REMAINING   DSI::Header::F2  // Return remaining storage size (in pages)
#define WAKEUP      DSI::Header::F3  // Wakeup SATA drives
#define WRITE       DSI::Header::F4  // Write first pagelet of a page
#define READ        DSI::Header::F7  // Read a page from a bucket
#define ERASE       DSI::Header::F8  // Delete pages of a bucket  
#define LOOKUP      DSI::Header::F9  // Get the current number of pages of a bucket
#define STATS       DSI::Header::F12 // Return statistics

#endif

