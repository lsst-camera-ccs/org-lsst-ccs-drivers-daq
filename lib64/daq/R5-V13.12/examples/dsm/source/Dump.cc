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
 
using namespace DSM::Editor::Source;
                                        
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
#include "daq/Location.hh"

static void _dump(const DSI::Location&, const DAQ::Location interfaces[], uint8_t modified=0);

static const char HEADER[] = "\n"
                             "Location  Interface-1    Interface-2   Interface-3"
                             "\n"
                             "-------- -------------  ------------- -------------" 
                             "\n";     
                             
static const char CURRENT[] = "Current";
static const char PENDING[] = "Pending";

static const char TRAILER[] = "  total = %d\n";
    
void Dump::process(int argc, const char** argv, int qualifier)   
 {
   
 bool current = qualifier == 0 ? true : false;

 printf(HEADER);//, current ? CURRENT : PENDING);
 
 DSI::LocationSet remaining = _stack.servers();
 DSI::Location    server; 

 int total = 0;
 
 while(remaining.remove(server))
  {
  
  uint8_t pending = 0;
  
  DAQ::Location interfaces[3];
   
  _interface.source(server, interfaces, pending, current);
  
  _dump(server, interfaces, pending);
 
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
 
static const char NA[]    = "N/A";
static const char CLEAN[] = "       ";
static const char DIRTY[] = "(dirty)";

static const char LINE[] = "%-8s" 
                           " %-7s %4s"
                           " %-7s %4s"
                           " %-7s %4s"
                           "\n";
                           
#define ONE(pending)    (pending & (1 << 0))
#define TWO(pending)    (pending & (1 << 1))
#define THREE(pending)  (pending & (1 << 2))
                                          
void _dump(const DSI::Location& server, const DAQ::Location interfaces[], uint8_t pending)
 {
 
 char location[16];
 
 char buffer1[32];
 char buffer2[32];
 char buffer3[32];
 
 const char* value1 = interfaces[0] ? interfaces[0].encode(buffer1) : NA;
 const char* value2 = interfaces[1] ? interfaces[1].encode(buffer2) : NA;
 const char* value3 = interfaces[2] ? interfaces[2].encode(buffer3) : NA;
 
 printf(LINE, server.encode(location), 
        ONE(pending)  ? DIRTY : CLEAN, 
        value1, 
        TWO(pending)  ? DIRTY : CLEAN, 
        value2,
        THREE(pending) ? DIRTY : CLEAN, 
        value3);
    
 return;
 } 
 
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  Dump the source names for the servers currently held by\n"
                           "  the stack. See the 'empty' command to empty the stack.\n"
                           "  See the 'push' command to populate the stack.\n"
                           "\n";
                                                     
void Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }

