#ifndef RMS_ACCESS_COMMAND
#define RMS_ACCESS_COMMAND

#include "../Command.hh"

namespace DAQ { class LocationSet;}

namespace RMS { class Client;

namespace Access {

class Command : public Utility::Command { 

public:
  Command(const char* command, Client&);
public:
 ~Command() {}
public: 
  virtual void help(int argc, const char** argv, int qualifier);
protected:
  bool _parse_regs(const char* locations, unsigned& base, unsigned& number);
  bool _build_list(unsigned base, Instruction::Opcode, uint32_t operand, InstructionList&);
};

}}

#endif
