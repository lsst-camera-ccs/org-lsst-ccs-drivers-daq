#include <stdio.h>
#include <string.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"

#include "rms/Client.hh"

#include "Command.hh"

using namespace RMS;

Utility::Command::Command(const char* command, Client& client) :
  OSA::Command(command), 
  _client(client)
{

}

const DAQ::LocationSet Utility::Command::_parse_targets(int argc, const char** argv)
{
  
  DAQ::LocationSet targets;
  
  if(0 == argc) { targets |= _client.sources(); return targets; }

  bool exclude = false;
  
  for(int i=0; i<argc; ++i)
  {
    DAQ::LocationSet set;
    if(0==strncmp(argv[i], "-a", 2))
    {
      set |= _client.sources();
    }
    else if(0==strcmp(argv[i], "-x")) exclude = true;
    else if(0==strcmp(argv[i], "-s")) set  = DAQ::ScienceSet();
    else if(0==strcmp(argv[i], "-g")) set  = DAQ::GuidingSet();
    else if(0==strcmp(argv[i], "-w")) set  = DAQ::WavefrontSet();
    else                              set |= DAQ::LocationSet(1, &argv[i]);

    if(exclude) targets &= ~set;
    else        targets |=  set;
  }   

  return targets;
}
