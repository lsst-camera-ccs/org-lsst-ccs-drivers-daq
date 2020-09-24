#include <stdio.h>

#include "MyHarvester.h"

MyHarvester::MyHarvester(int size) : _size(size) {
   _values = new int[size * _all.SIZE];
}

void MyHarvester::process(const DAQ::Location& loc, const RMS::InstructionList& result, int32_t error) {
  _all.insert(loc);
  
  if(error || (result.level() != _size)) {_errors.insert(loc); return;}
  for (int j=0; j<_size; j++) {
    const RMS::Instruction* instruction = result.lookup(j);
    if(instruction->fault())             _errors.insert(loc);
    _values[j + loc.index()*_size] = instruction->operand();
  }
}

