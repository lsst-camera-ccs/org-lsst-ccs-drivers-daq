#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"
#include "rms/Errors.hh"
#include "daq/InterfaceDriverStats.hh"
#include "Driver.hh"

using namespace RMS;

static const char COMMAND[] = "driver";

/*
** ++
**
**
** --
*/

Monitor::Driver::Driver(Client& client) :
  Command(COMMAND, client) 
{ 
  
}

/*
** ++
**
**
** --
*/

static const char FAILURE[]    = "%4s ERROR: %s (%i)\n";

bool Monitor::Driver::_display(const DAQ::Location& loc, bool clear, bool header_done)
{
  char locstr[8];
  DAQ::InterfaceDriverStats stats;
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
  " Statistics recorded by the low level PGP Driver\n\n";

void Monitor::Driver::help(int argc, const char** argv, int qualifier)   
{
  printf(SUMMARY);
  Monitor::Command::help(argc, argv, qualifier);

  return;
}
