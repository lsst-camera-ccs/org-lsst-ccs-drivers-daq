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

using namespace DSM;

/*
** ++
**
**
** --
*/

Probe::Probe(Interface& interface, Editor& editor) :
 Command(COMMAND), 
 _interface(interface),
 _editor(editor) 
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
 
static const char CLEAN[]   = " %-6s %-15s %-8s %s [08%X] (clean), %-24s\n";
static const char DIRTY[]   = " %-6s %-15s %-8s %s [08%X] (dirty), %-24s\n";                               
static const char TRAILER[] = "  total = %d\n";
                           
void Probe::process(int argc, const char** argv, int constant)   
 {

 printf(HEADER);

 Interface& interface = _interface;
 
 DSI::Set sources(interface.servers());
 
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
    
  bool found = interface.probe(source, location, address, version);
  
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
                           "  Prints to standard out a list of the RCEs in the corresponding partition\n" 
                           "  The list includes their corresponding addresses and build information.\n"
                           "  The command requires no arguments.\n"
                           "\n";
                              
void Probe::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
