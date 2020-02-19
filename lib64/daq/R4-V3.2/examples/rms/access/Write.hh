#ifndef RMS_ACCESS_WRITE
#define RMS_ACCESS_WRITE

#include "rms/Harvest.hh"

#include "Command.hh"

namespace DAQ { class Location; }

namespace RMS { class Client; class InstructionList;

namespace Access {

class Write : public Command { 

class Harvest : public RMS::Harvest
{
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
};

public:
  Write(Client&);
public:
 ~Write() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Harvest _harvest;
};

}}

#endif

