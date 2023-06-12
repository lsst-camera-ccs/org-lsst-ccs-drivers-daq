#ifndef RMS_MONITOR_DRIVER
#define RMS_MONITOR_DRIVER

#include "Command.hh"

namespace RMS { 

class Client;

namespace Monitor {

class Driver : public Command { 

public:
  Driver(Client&);
public:
 ~Driver() {}
public:
  void help(   int argc, const char** argv, int qualifier);        
public:
  bool _display(const DAQ::Location&, bool clear, bool header_done);
};

}}

#endif
