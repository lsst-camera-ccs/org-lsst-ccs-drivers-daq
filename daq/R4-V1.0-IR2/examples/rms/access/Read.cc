#include <stdio.h>
#include <string.h>

#include "osa/Tokens.hh"
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

  if(0 == argc)  { printf(NO_REGISTER); return; }
  if(!_parse_regs(REGISTERS, base, number)) { printf(NO_REGISTER); return; }
  
  InstructionList instructions(number);
  
  _build_list(base, Instruction::GET, DONT_CARE, instructions);
  
  DAQ::LocationSet targets = _parse_targets(argc-1, (const char**)&TARGETS);

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

void Access::Read::help(int argc, const char** argv, int qualifier)   
{
  
  printf(HELP);
  
  return;
}

static const char ERROR[]         = "%4s:        ERROR: %s (%i)\n";
static const char DISPLAY[]       = "%4s:%06x  %08x\n";
static const char DISPLAY_FAULT[] = "%4s:%06x  %08x - FAULT %s (%i)\n";

void Access::Read::Harvest::process(const DAQ::Location& location, const InstructionList& result, int32_t error)
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
