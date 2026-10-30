#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rms/Client.hh"

#include "RebootRce.hh"

using namespace RMS;

static const char COMMAND[] = "reboot_rce";

/*
** ++
**
**
** --
*/

Access::RebootRce::RebootRce(Client& client) :
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

static const char SUCCESS[]    = "Reboot RCE posted Successfully\n";
static const char FAILURE[]    = "Reboot RCE commanding FAILED\n";
static const char NO_TARGETS[] = "No valid targets given\n";

void Access::RebootRce::process(int argc, const char** argv, int constant)
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

  bool success = _client.reboot_rce(targets);

  if(success) printf(SUCCESS);
  else        printf(FAILURE);

  return;
}

/*
** ++
**
**
** --
*/

static const char SUMMARY[] =
  " Reboot the Readout RCE of one or more sources\n\n"
  " Specifying DAQ Locations is required for reset.\n"
  " No matter what the next line says!\n";

void Access::RebootRce::help(int argc, const char** argv, int qualifier)
{
  printf(SUMMARY);
  DAQ::LocationSet::usage();

  return;
}
