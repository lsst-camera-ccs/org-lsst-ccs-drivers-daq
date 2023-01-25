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

static const char COMMAND[] = "dump";

using namespace CMS;

/*
** ++
**
**
** --
*/

Editor::Dump::Dump(Camera& camera) :
 Command(COMMAND), 
 _camera(camera) 
 { 
 
 }

/*
** ++
**
**
** --
*/

#include "net/ipv4/Address.hh"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"

static const char HEADER[] = "\n"
                             " Source     Address     Location"
                             "\n"
                             " ------ --------------- --------" 
                             "\n";     
                             
static const char SOURCE[]  = " %-6s %-15s %-8s\n";
static const char TRAILER[] = "  total = %d\n";
                           
void Editor::Dump::process(int argc, const char** argv, int constant)   
 {

 printf(HEADER);

 Camera& camera = _camera;
 
 DAQ::LocationSet sources(camera.sources());
 
 unsigned total = 0;

 char buffer1[32];
 char buffer2[32];
 char buffer3[32];
 
 DAQ::Location source;
 
 while(sources.remove(source))
  {
  
  DVI::Version  version; 
  IPV4::Address address;
  DSI::Location location;
     
  bool found = camera.probe(source, location, address, version);
  
  if(!found) continue;
 
  printf(SOURCE, source.encode(buffer1), address.presentation(buffer2), location.encode(buffer3));
   
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
                           "  Print to standard out a synopsis of each SCI participating in image readout. This synopsis\n"
                           "  includes the SCI location as well as its corresponding ATCA and IPV4 (network) address.\n" 
                            "\n";
                                                  
void Editor::Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
