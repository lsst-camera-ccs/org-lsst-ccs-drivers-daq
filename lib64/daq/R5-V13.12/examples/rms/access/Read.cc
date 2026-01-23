#include <stdio.h>
#include <string.h>

#include "osa/Tokens.hh"
#include "rms/HarvestLists.hh"
#include "rms/Client.hh"
#include "rms/Errors.hh"

#include "Read.hh"

using namespace RMS;

static const char COMMAND[] = "read";

/*
** ++
**
**
** --
*/

Access::Read::Read(Client& client) :
  Command(COMMAND, client)
{

}

/*
** ++
**
**
** --
*/

#define REGISTERS argv[0]
#define TARGETS   argv[1]

static const char NO_REGISTER[] = "No Valid Register Address supplied\n";
static const char NO_TARGETS[]  = "No valid targets given\n";
static const uint32_t DONT_CARE = 0;

void Access::Read::process(int argc, const char** argv, int constant)
{
  unsigned base;
  unsigned number;

  HarvestLists harvest;

  if(0 == argc)  { printf(NO_REGISTER); return; }

  if(0 == strncmp(argv[0], "-h", 2))
  {
    help(argc, argv, constant);
    return;
  }

  if(!_parse_regs(REGISTERS, base, number)) { printf(NO_REGISTER); return; }

  InstructionList instructions(number);

  _build_list(base, Instruction::GET, DONT_CARE, instructions);

  DAQ::LocationSet targets(argc-1, (const char**)&TARGETS);
  targets &= _client.sources();

  if(!targets) { printf(NO_TARGETS); return; }

  _client.access(targets, instructions, harvest);

  harvest.summary();
  harvest.clear();

  return;
}

/*
** ++
**
**
** --
*/

static const char SUMMARY[] =
  " Read one or more registers from one or more Sources.\n\n"
  "  Usage: <command> <registers> [DAQ Loacations]\n\n";

void Access::Read::help(int argc, const char** argv, int qualifier)
{
  printf(SUMMARY);
  DAQ::LocationSet::usage();

  return;
}
