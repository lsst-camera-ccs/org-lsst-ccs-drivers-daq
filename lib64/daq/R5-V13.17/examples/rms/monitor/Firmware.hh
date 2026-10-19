#ifndef RMS_MONITOR_FIRMWARE
#define RMS_MONITOR_FIRMWARE

#include "Command.hh"

namespace RMS { 
  
class Client;

namespace Monitor {

class Firmware : public Command { 

public:
  Firmware(Client&);
public:
 ~Firmware() {}
public:
  void help(   int argc, const char** argv, int qualifier);        
public:
  bool _display(const DAQ::Location&, bool clear, bool header_done);
};

}}

#endif
