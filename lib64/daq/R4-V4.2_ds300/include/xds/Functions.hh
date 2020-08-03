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

#include "dsi/Frame.hh"

#define PROBE     DSI::Frame::F0  // Return size of storage (in pages)

#define CAPACITY  DSI::Frame::F1  // Return size of storage (in pages)
#define CURRENT   DSI::Frame::F2  // Return remaining storage size (in pages)

#define WRITE     DSI::Frame::F4  // Write a page to a bucket
#define READ      DSI::Frame::F5  // Read a page from a bucket
#define ERASE     DSI::Frame::F6  // Delete pages of a bucket 
 
#define EOB_GET   DSI::Frame::F8  // Get the current extent of a bucket
#define EOB_SET   DSI::Frame::F9  // Set the current extent of a bucket
#define EOB_RESET DSI::Frame::F10 // Delete the current extent of a bucket

#define SAMPLE    DSI::Frame::F12 // Sample running statistics 
#define RESET     DSI::Frame::F13 // Reset running statistics

#endif

