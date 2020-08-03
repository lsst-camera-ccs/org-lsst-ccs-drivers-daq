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

#ifndef DCS_FUNCTIONS
#define DCS_FUNCTIONS

#include "dsi/Frame.hh"

#define PROBE            DSI::Frame::F0  // Get service address
#define RELEASE          DSI::Frame::F1  // Get version information 

#define CREATE_FOLDER    DSI::Frame::F4  // Insert a specified folder
#define LOOKUP_FOLDER    DSI::Frame::F5  // Lookup by key, returning folder name  
#define DELETE_FOLDER    DSI::Frame::F6  // Delete (by key) an existing folder
#define MARSHALL_FOLDERS DSI::Frame::F7  // Get a list of keys for all existing folders

#define CREATE           DSI::Frame::F16 // Create new entry
#define LOOKUP           DSI::Frame::F17 // Lookup by key, returning ID
#define MOVE             DSI::Frame::F18 // Move name from its specified (existing) folder to a new (existing folder)
#define DELETE_BY_KEY    DSI::Frame::F19 // Delete existing entry (by folder and name keys)
#define DELETE_BY_ID     DSI::Frame::F20 // Delete existing entry (by ID)
#define MARSHALL_IDS     DSI::Frame::F21 // Get a list (by Key) of the IDs for all entries within a specified (existing) folder 
#define MARSHALL_KEYS    DSI::Frame::F22 // Get a list (by Key) of the Keys for all entries within a specified (existing) folder 
 
#endif

