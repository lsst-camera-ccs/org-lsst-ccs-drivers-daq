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

#include "Probe.hh"

static const char COMMAND[] = "probe";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Probe::Probe(Store& store) :
 Command(COMMAND), 
 _store(store)
 { 
 }

/*
** ++
**
**
** --
*/

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsi/Set.hh"

static const char HEADER[] = "\n"
                             " Source     Address     Location                    Build Information                    "
                             "\n"
                             " ------ --------------- -------- --------------------------------------------------------" 
                             "\n";     
 
static const char CLEAN[]   = " %-6s %-15s %-8s %s [%08X] (clean), %-24s\n";
static const char DIRTY[]   = " %-6s %-15s %-8s %s [%08X] (dirty), %-24s\n";                               
static const char TRAILER[] = "  total = %d\n";
                           
void Editor::Probe::process(int argc, const char** argv, int constant)   
 {

 printf(HEADER);

 Store& store = _store;
 
 DSI::Set sources(store.servers());
 
 unsigned total = 0;

 char buffer1[32];
 char buffer2[32];
 char buffer3[32];
    
 uint8_t next = sources.remove();
 
 while(next != DSI::Set::SIZE)
  {
  
  DVI::Version  version; 
  IPV4::Address address;
  DSI::Location location;
  
  DSI::Location source(next);
 
  next = sources.remove();
    
  bool found = store.probe(source, location, address, version);
  
  if(!found) continue;
       
  printf(version.dirty() ? DIRTY : CLEAN,  
         source.encode(buffer1), 
         address.presentation(buffer2), 
         location.encode(buffer3),
         version.tag(), version.hash(), version.buildTime.decode() 
         );
   
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

static const char HELP[] = "\n"
                           "  Prints to standard out a list of all the servers implementing the store.\n"
                           "  For each server its ATCA address, IPV4 address and version information is output.\n"
                           "\n";
                                                            
void Editor::Probe::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
