#ifndef RMS_ACCESS_WRITE
#define RMS_ACCESS_WRITE

#include "Command.hh"
#include "Harvest.hh"

namespace DAQ { class Location; }

namespace RMS { class Client; class InstructionList;

namespace Access {

class Write : public Command { 
public:
  Write(Client&);
public:
 ~Write() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Access::Harvest _harvest;
};

}}

#endif

