#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"
#include "rms/Errors.hh"
#include "daq/InterfaceFirmwareStats.hh"
#include "Firmware.hh"

using namespace RMS;

static const char COMMAND[] = "firmware";

/*
** ++
**
**
** --
*/

Monitor::Firmware::Firmware(Client& client) :
  Command(COMMAND, client)
{ 
  
}

/*
** ++
**
**
** --
*/

static const char FAILURE[] = "%4s: ERROR: %s (%i)\n";;

bool Monitor::Firmware::_display(const DAQ::Location& loc, bool clear, bool header_done)
{
  char locstr[8];
  DAQ::InterfaceFirmwareStats stats;
  int32_t error;
  
  loc.encode(locstr);
  if(_client.stats(loc, clear, stats, error)) stats.dump(locstr, !header_done);
  else printf(FAILURE, locstr, Errors::decode(error), error);

  return !header_done;  
}


/*
** ++
**
**
** --
*/

static const char SUMMARY[] =
  " Statistics recorded by the PGP Firmware\n\n";

void Monitor::Firmware::help(int argc, const char** argv, int qualifier)   
{
  printf(SUMMARY);
  Monitor::Command::help(argc, argv, qualifier);
}
