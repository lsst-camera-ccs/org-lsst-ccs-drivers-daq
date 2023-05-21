#include <stdio.h>
#include <stdlib.h>

#include "osa/Tokens.hh"
#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"
#include "rms/Client.hh"

#include "Command.hh"

using namespace RMS;

Access::Command::Command(const char* command, Client& client) :
  Utility::Command(command, client)
{

}


/*
** ++
**
**
** --
*/

bool Access::Command::_parse_regs(const char* locations, unsigned& base, unsigned& number)
{
  OSA::Tokens tokens(locations, ':');
  
  const char* token = tokens.vector()[0];
  char* last;
  
  base = strtoll(token, &last, 0);
  
  if(last == token) return false;
  
  number = 1;

  if(2 == tokens.numof())
  {
    token = tokens.vector()[1];
    number = strtoll(token, &last, 0);
    if(last == token) return false;
  }
  
  return true;
}

bool Access::Command::_build_list(unsigned base, Instruction::Opcode opcode, uint32_t operand, InstructionList& list)
{
  unsigned remaining = list.size();
  while(remaining--) list.insert(opcode, base++, operand);
  
  return list;
}
