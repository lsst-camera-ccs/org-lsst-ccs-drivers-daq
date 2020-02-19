#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osa/Tokens.hh"
#include "rms/Errors.hh"
#include "rms/Client.hh"

#include "Write.hh"

using namespace RMS;

static const char COMMAND[] = "write";

/*
** ++
**
**
** --
*/

Access::Write::Write(Client& client) :
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
#define VALUE     argv[1]
#define TARGETS   argv[2]

static const char NO_REGISTER[] = "No Valid Register Address supplied\n";
static const char NO_VALUE[]    = "No Valid Register Value supplied\n";
static const char NO_TARGETS[]  = "No valid targets given\n";

void Access::Write::process(int argc, const char** argv, int constant)   
{
  unsigned base;
  unsigned number;

  if(0 == argc)  { printf(NO_REGISTER); return; }
  if(!_parse_regs(REGISTERS, base, number)) {printf(NO_REGISTER); return;}
  
  char* last;
  unsigned value = strtoll(VALUE, &last, 0);
  if(last == VALUE) {printf(NO_VALUE); return;}

  InstructionList instructions(number);
  
  _build_list(base, Instruction::PUT, value, instructions);

  DAQ::LocationSet targets = _parse_targets(argc-2, (const char**)&TARGETS);

  if(!targets) { printf(NO_TARGETS); return; }

  _client.access(targets, instructions, _harvest);

  return; 
}

/*
** ++
**
**
** --
*/

static const char HELP[]    = "Yeah, we should have some help.\n";

void Access::Write::help(int argc, const char** argv, int qualifier)   
{
  
  printf(HELP);
 
  return;
}

static const char ERROR[]         = "%4s:        ERROR: %s (%i)\n";
static const char DISPLAY[]       = "%4s:%06x  %08x\n";
static const char DISPLAY_FAULT[] = "%4s:%06x  %08x - FAULT %s (%i)\n";

void Access::Write::Harvest::process(const DAQ::Location& location, const InstructionList& result, int32_t error)
{
  char source[8];
  location.encode(source);

  if(error) {printf(ERROR, source, Errors::decode(error), error); return;}

  for (unsigned i=0; i < result.level(); ++i)
  {
    const Instruction* instruction = result.lookup(i);
    
    unsigned   reg           = instruction->reg();
    uint32_t   operand       = instruction->operand(); 
    Instruction::Fault fault = instruction->fault();
    const char* explain      = instruction->decodeFault();
   
    if(!fault) printf(DISPLAY, source, reg, operand);
    else       printf(DISPLAY_FAULT, source, reg, operand, explain, fault);
  }

  return;
}
