#ifndef RMS_ACCESS_READ
#define RMS_ACCESS_READ

#include "rms/Harvest.hh"
#include "Command.hh"

namespace DAQ { class Location; }

namespace RMS { class Client; class InstructionList;

namespace Access {

class Read : public Command { 

class Harvest : public RMS::Harvest
{
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
};

public:
  Read(Client&);
public:
 ~Read() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Harvest _harvest;
};

}}

#endif
