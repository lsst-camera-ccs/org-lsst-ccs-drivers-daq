#ifndef RMS_ACCESS_READ
#define RMS_ACCESS_READ

#include "Command.hh"

namespace DAQ { class Location; }

namespace RMS { class Client; class InstructionList;

namespace Access {

class Read : public Command { 
public:
  Read(Client&);
public:
 ~Read() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
};

}}

#endif
