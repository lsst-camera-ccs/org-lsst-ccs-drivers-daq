#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"
#include "rms/Errors.hh"
#include "rms/Stats.hh"
#include "Service.hh"

using namespace RMS;

static const char COMMAND[] = "rms";

/*
** ++
**
**
** --
*/

Monitor::Service::Service(Client& client) :
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

bool Monitor::Service::_display(const DAQ::Location& loc, bool clear, bool header_done)
{
  char    locstr[8];
  Stats   stats;
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
  " Statitics reflecting the requests and responses handled by the RMS Service.\n\n";

void Monitor::Service::help(int argc, const char** argv, int qualifier)   
{
  
  printf(SUMMARY);
  DAQ::LocationSet::usage();

  return;
}
