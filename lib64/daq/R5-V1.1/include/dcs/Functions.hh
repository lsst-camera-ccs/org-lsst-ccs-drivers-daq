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

#include "dsi/Header.hh"

/*
** Entry functions...
*/

#define ASSIGN           DSI::Header::F16 // Create new entry (by name) in a folder specified by key
#define LOOKUP_ENTRY     DSI::Header::F17 // Lookup entry by key, in a folder specified by key, returning ID
#define MOVE_ENTRY       DSI::Header::F18 // Move entry from its specified (existing) folder to a new (existing) folder
#define REMOVE_ENTRY     DSI::Header::F19 // Remove existing entry (by folder and name keys)
#define REMOVE_BY_ID     DSI::Header::F20 // Remove existing entry (by ID)

/*
** Folder functions...
*/

#define INSERT_FOLDER    DSI::Header::F4  // Create a specified folder (by name)
#define LOOKUP_FOLDER    DSI::Header::F5  // Lookup by key, returning folder name  
#define REMOVE_FOLDER    DSI::Header::F6  // Remove by key an existing folder

#define MARSHALL_FOLDERS DSI::Header::F7  // Get a list of keys for all existing folders
#define MARSHALL_IDS     DSI::Header::F21 // Get a list (by Key) of the IDs for all entries within a specified (existing) folder 
#define MARSHALL_KEYS    DSI::Header::F22 // Get a list (by Key) of the Keys for all entries within a specified (existing) folder 

 
#endif

