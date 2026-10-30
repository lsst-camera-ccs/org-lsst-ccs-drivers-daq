#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"

#include "ResetLink.hh"

using namespace RMS;

static const char COMMAND[] = "reset_link";

/*
** ++
**
**
** --
*/

Access::ResetLink::ResetLink(Client& client) :
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

static const char SUCCESS[]    = "%4s: Reset Link Succeeded\n";
static const char FAILURE[]    = "%4s: Reset Link FAILED\n";
static const char NO_TARGETS[] = "No valid targets given\n";

void Access::ResetLink::process(int argc, const char** argv, int constant)
{

  if(0 == argc)  { help(argc, argv, constant); return; }

  if(0 == strncmp(argv[0], "-h", 2))
  {
    help(argc, argv, constant);
    return;
  }

  DAQ::LocationSet targets(argc-0, (const char**)&TARGETS);
  targets &= _client.sources();

  if(!targets) { printf(NO_TARGETS); return; }

  DAQ::LocationSet success = _client.reset_link(targets);

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

static const char SUMMARY[] =
  " Reset PGP Link in the RCE of one or more sources\n\n"
  " Specifying DAQ Locations is required for reset.\n"
  " No matter what the next line says!\n";

void Access::ResetLink::help(int argc, const char** argv, int qualifier)
{
  printf(SUMMARY);
  DAQ::LocationSet::usage();

  return;
}
