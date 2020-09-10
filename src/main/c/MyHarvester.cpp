#include <stdio.h>

#include "MyHarvester.h"

void MyHarvester::process(const DAQ::Location& loc, const RMS::InstructionList& result, int32_t error) {
  _all.insert(loc);
  
  if(error || (result.level() != 1)) {_errors.insert(loc); return;}
  const RMS::Instruction* instruction = result.lookup(0);
  if(instruction->fault())             _errors.insert(loc);
  if(instruction->operand() & SEQ_BIT) _busy.insert(loc);
  values[loc.index()] = instruction->operand();
}

