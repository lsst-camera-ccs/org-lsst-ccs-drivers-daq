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

  return targets & _client.sources();
}

static const char HELP[] = \
  "  If no DAQ Locations are specified, the default is all locations in the partition\n"
  "\n"
  "  DAQ Locations can be specified in the forms: \n"
  "    <Raft[/REB]> (Examples: 10/0 10/1 22 0/1 00/1\n"
  "\n"
  "  The following predefined Sets are also supported:\n"
  "    -a All Sources\n"
  "    -s Science Sources\n"
  "    -w Wavefront Sources\n"
  "    -g Guiding Sources\n"
  "\n"
  "  Any of these forms can be combined, for example:\n"
  "    Select Raft22/REB0, Raft10, and Wavefonts: 22/0 10 -w\n"
  "\n"
  "  Sources can be excluded from previously defined sets by placing them after -x. \n"
  "  For Example:\n"
  "    Select all Sources except Guiding Sources:   -a -x -g\n"
  "    Select all Science Sources except Raft 10:   -s -x 10\n"
  "    Select all Corner Sources except Raft0/REB1: -w -g -x 0/1\n"
  "\n";

void Utility::Command::help(int argc, const char** argv, int qualifier)
{
  printf(HELP);
}
