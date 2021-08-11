
#include <stdio.h>

#include "Harvest.hh"

#include "rms/Errors.hh"
#include "rms/Instruction.hh"

using namespace RMS;

/*
** ++
**
**
** --
*/

static const char ERROR[]         = "%4s:        ERROR: %s (%i)\n";
static const char DISPLAY[]       = "%4s:%06x  %08x\n";
static const char DISPLAY_FAULT[] = "%4s:%06x  %08x - FAULT %s (%i)\n";

void Access::Harvest::process(const DAQ::Location& location, const InstructionList& result, int32_t error)
{
  char source[8];
  location.encode(source);

  if(error) {
    ++_response_errors;
    printf(ERROR, source, Errors::decode(error), error); 
    return;
  }

  _sources |= DAQ::LocationSet(location);
  
  for (unsigned i=0; i < result.level(); ++i)
  {
    const Instruction* instruction = result.lookup(i);
    
    unsigned   reg           = instruction->reg();
    uint32_t   operand       = instruction->operand(); 
    Instruction::Fault fault = instruction->fault();
    const char* explain      = instruction->decodeFault();

    if(!fault) {
      ++_instructions; 
      printf(DISPLAY, source, reg, operand);
    }
    else {
      ++_instruction_errors; 
      printf(DISPLAY_FAULT, source, reg, operand, explain, fault);
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

static const char TOTAL[] = 
  " Total Sources      = %d, Source Errors      = %d\n"
  " Total Instructions = %d, Instruction Errors = %d\n";

void Access::Harvest::summary()
{
  printf(TOTAL, _sources.numof(), _response_errors, _instructions, _instruction_errors);
}
