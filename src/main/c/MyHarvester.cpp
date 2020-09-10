#include <stdio.h>

#include "MyHarvester.h"

MyHarvester::MyHarvester() {

}

void MyHarvester::process(const DAQ::Location& loc, const RMS::InstructionList& result, int32_t error) {
  _all.insert(loc);
  
  if(error || (result.level() != 1)) {_errors.insert(loc); return;}
  const RMS::Instruction* instruction = result.lookup(0);
  if(instruction->fault())             _errors.insert(loc);
  _values[loc.index()] = instruction->operand();
}

