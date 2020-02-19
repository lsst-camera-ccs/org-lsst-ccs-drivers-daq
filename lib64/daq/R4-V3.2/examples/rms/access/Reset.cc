#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"

#include "Reset.hh"

using namespace RMS;

static const char COMMAND[] = "reset";

/*
** ++
**
**
** --
*/

Access::Reset::Reset(Client& client) :
  Command(COMMAND, client) 
{ 
  
}

/*
** ++
**
**
** --
*/

#define TARGETS   argv[0]

static const char SUCCESS[]    = "%4s: Reset Succeeded\n";
static const char FAILURE[]    = "%4s: Reset FAILED\n";
static const char NO_TARGETS[] = "No valid targets given\n";

void Access::Reset::process(int argc, const char** argv, int constant)   
{

  DAQ::LocationSet targets = _parse_targets(argc-0, (const char**)&TARGETS);

  if(!targets) { printf(NO_TARGETS); return; }
  
  DAQ::LocationSet success = _client.reset(targets);

  DAQ::Location loc;
  char buffer[8];
  while(targets.remove(loc))
  {
    const char* message = success.has(loc) ? SUCCESS : FAILURE;
    printf(message, loc.encode(buffer));
  }
  
  return; 
}

/*
** ++
**
**
** --
*/

static const char HELP[]    = "Yeah, we should have some help.\n";

void Access::Reset::help(int argc, const char** argv, int qualifier)   
{
  
  printf(HELP);
 
  return;
}
