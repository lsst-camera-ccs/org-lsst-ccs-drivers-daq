#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rms/Client.hh"

#include "SetLink.hh"

using namespace RMS;

static const char COMMAND[] = "set_link";

/*
** ++
**
**
** --
*/

Access::SetLink::SetLink(Client& client) :
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

static const char SUCCESS[]    = "%8s: Link %s Succeeded\n";
static const char FAILURE[]    = "%8s: Link %s FAILED\n";
static const char NO_TARGETS[] = "No valid targets given\n";
static const char BAD_USAGE[]  = "Must specify -d (down), -u (up), or -r [duration] (reset)\n";

static const unsigned DEFAULT_DURATION = 15;

void Access::SetLink::process(int argc, const char** argv, int constant)
{

  if(0 == argc)  { help(argc, argv, constant); return; }

  if(0 == strncmp(argv[0], "-h", 2))
  {
    help(argc, argv, constant);
    return;
  }

  // Parse mode flag: must be first argument
  enum { DOWN, UP, RESET } mode;

  if     (0 == strncmp(argv[0], "-d", 2)) { mode = DOWN;  }
  else if(0 == strncmp(argv[0], "-u", 2)) { mode = UP;    }
  else if(0 == strncmp(argv[0], "-r", 2)) { mode = RESET; }
  else { printf(BAD_USAGE); return; }

  argc--; argv++;

  // For reset mode, parse optional duration (seconds)
  unsigned duration = DEFAULT_DURATION;
  if(mode == RESET && argc > 0)
  {
    char* end;
    unsigned val = strtoul(argv[0], &end, 10);
    if(*end == '\0' && val > 0)
    {
      duration = val;
      argc--; argv++;
    }
  }

  DAQ::LocationSet targets(argc, (const char**)&TARGETS);
  targets &= _client.sources();

  if(!targets) { printf(NO_TARGETS); return; }

  DAQ::LocationSet success;
  char buffer[16];

  if(mode == DOWN || mode == RESET)
  {
    success = _client.set_link(targets, false);

    DAQ::LocationSet remaining(targets);
    DAQ::Location loc;
    while(remaining.remove(loc))
    {
      const char* message = success.has(loc) ? SUCCESS : FAILURE;
      printf(message, loc.encodeReb(buffer), "Down");
    }
  }

  if(mode == RESET)
  {
    printf("Waiting %u seconds...\n", duration);
    sleep(duration);
  }

  if(mode == UP || mode == RESET)
  {
    success = _client.set_link(targets, true);

    DAQ::LocationSet remaining(targets);
    DAQ::Location loc;
    while(remaining.remove(loc))
    {
      const char* message = success.has(loc) ? SUCCESS : FAILURE;
      printf(message, loc.encodeReb(buffer), "Up");
    }
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
  " Control PGP Link state of one or more sources\n\n"
  " Modes:\n"
  "   -d              Set link down\n"
  "   -u              Set link up\n"
  "   -r [duration]   Reset: down, wait, up (default %u seconds)\n"
  "\n"
  " Specifying DAQ Locations is required.\n"
  " No matter what the next line says!\n";

void Access::SetLink::help(int argc, const char** argv, int qualifier)
{
  printf(SUMMARY, DEFAULT_DURATION);
  DAQ::LocationSet::usage();

  return;
}
