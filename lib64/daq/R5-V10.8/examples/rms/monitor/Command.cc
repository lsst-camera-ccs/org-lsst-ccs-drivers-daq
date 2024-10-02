#include <stdio.h>
#include <string.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/GuidingSet.hh"
#include "daq/WavefrontSet.hh"

#include "rms/Client.hh"

#include "Command.hh"

using namespace RMS;

Monitor::Command::Command(const char* command, Client& client) :
  Utility::Command(command, client)
{

}


#define TARGETS   argv[0]
static const char NO_TARGETS[] = "No valid targets given\n";
static const char TOTAL[]      = "Total Sources = %d\n";

void Monitor::Command::process(int argc, const char** argv, int qualifier)
{
  bool clear = false;
  int idx = 0;

  if(argc>0)
  {
    if(0 == strncmp(argv[0], "-h", 2))
    {
      help(argc, argv, qualifier);
      return;
    }
    if(0 == strncmp(argv[0], "-c", 2))
    {
      clear = true; 
      --argc;
      ++idx;
    }
  }

  DAQ::LocationSet sources(argc, (const char**)&argv[idx]);
  sources &= _client.sources();

  if(!sources) { printf(NO_TARGETS); return; }

  DAQ::Location loc;
  unsigned total = 0;

  DAQ::ScienceSet   science;
  DAQ::WavefrontSet wavefront;
  DAQ::GuidingSet   guiding;

  while(science.remove(loc))
    if(sources.had(loc)) _display(loc, clear, total++);

  while(wavefront.remove(loc))
    if(sources.had(loc)) _display(loc, clear, total++);

  while(guiding.remove(loc))
    if(sources.had(loc)) _display(loc, clear, total++);
  
  printf(TOTAL, total);

  return; 
  
}
