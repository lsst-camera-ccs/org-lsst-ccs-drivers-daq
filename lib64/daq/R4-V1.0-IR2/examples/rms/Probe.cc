#include <stdio.h>

#include "rms/Client.hh"

#include "Probe.hh"

static const char COMMAND[] = "probe";

using namespace RMS;

/*
** ++
**
**
** --
*/

Utility::Probe::Probe(Client& client) :
 Command(COMMAND), 
 _client(client)
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
                           
void Utility::Probe::process(int argc, const char** argv, int constant)   
 {

 printf(HEADER);

 DSI::Set sources(_client.sources());
 
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
  
  DAQ::Location source(next);
 
  next = sources.remove();
    
  bool found = _client.probe(source, location, address, version);
  
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

static const char HELP[]    = "Prints to standard out a synopsis of the image  as specified by its name and folder.\n"
                              "Image Name and folder are specified as command lines arguments, with the image as\n"
                              "the *first* argument and the folder as the second argument.\n";
                              
void Utility::Probe::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
