#include <stdio.h>
#include <string.h>

#include "daq/LocationSet.hh"

#include "rms/Client.hh"

#include "Command.hh"

using namespace RMS;

Monitor::Command::Command(const char* command, Client& client) :
  Utility::Command(command, client)
{

}


#define TARGETS   argv[0]
static const char NO_TARGETS[] = "No valid targets given\n";

void Monitor::Command::process(int argc, const char** argv, int qualifier)
{
  bool clear = false;
  int idx = 0;

  if(argc>0)
  {
    if(0 == strncmp(argv[0], "-c", 2))
    {
      clear = true; 
      --argc;
      ++idx;
    }
  }

  DAQ::LocationSet sources = _parse_targets(argc, (const char**)&argv[idx]);

  if(!sources) { printf(NO_TARGETS); return; }

  DAQ::Location loc;
  bool header_done = false;
  while(sources.remove(loc))
  {
    header_done |= _display(loc, clear, header_done);
  }
  
  return; 
  



}
