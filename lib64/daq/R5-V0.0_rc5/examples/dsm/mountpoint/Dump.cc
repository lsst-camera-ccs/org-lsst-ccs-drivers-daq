/*
** ++
**  Package:
**	
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/

#include <stdio.h>

#include "Dump.hh"
 
using namespace DSM::Editor::Mountpoint;
                                        
/*
** ++
**
**
** --
*/

static const char NAME[] = "dump";

Dump::Dump(Interface& interface, DSI::Stack& stack) : 
 OSA::Command(NAME),
 _interface(interface),
 _stack(stack)
 {
 }

/*
** ++
**
**
** --
*/

#include "dsi/LocationSet.hh"
#include "dsi/Location.hh"

#define MAX 512

static void _dump(const DSI::Location&, const char* name, uint8_t modified=0);

static const char NOT_AVAILABLE[] = "Not available (no response)\n";

static const char CURRENT[] = "Current";
static const char PENDING[] = "Pending";

static const char HEADER[] = "\n"
                             " Location Pending? %s assignment..."
                             "\n"
                             " -------- -------- ---------------------" 
                             "\n";     
                             
static const char TRAILER[] = "  total = %d\n";
  
void Dump::process(int argc, const char** argv, int qualifier)   
 {
   
 bool current = qualifier == 0 ? true : false;

 printf(HEADER, current ? CURRENT : PENDING);
 
 char partition[MAX];
 
 DSI::LocationSet remaining = _stack.servers();
 DSI::Location    server; 

 int total = 0;
 
 while(remaining.remove(server))
  {
  
  uint8_t modified;
  
  if(_interface.mountpoint(server, partition, modified, current))
    _dump(server, partition, modified);
  else
    _dump(server, NOT_AVAILABLE);

  total++;
  
  }

 printf(TRAILER, total);
  
 return; 
 }

/*
** ++
**
**
** --
*/

static const char NA[]   = "N/A";
static const char YES[]  = "Yes";
static const char NO[]   = "No";
static const char LINE[] = " %-8s %-8s %s\n";
                                  
void _dump(const DSI::Location& server, const char* value, uint8_t modified)
 {
 
 char location[16];
 
 printf(LINE, server.encode(location), modified  ? YES : NO, *value ? value : NA);
     
 return;
 } 
 
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  Dump the NFS mountpoints for the servers currently held on\n"
                           "  the command stack. See the 'empty' command to empty that stack.\n"
                           "  And see the 'push' command to populate that stack.\n"
                           "\n";
                                                     
void Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
