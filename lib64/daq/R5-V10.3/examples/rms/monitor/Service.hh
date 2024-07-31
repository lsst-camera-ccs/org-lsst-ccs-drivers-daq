#ifndef RMS_MONITOR_SERVICE
#define RMS_MONITOR_SERVICE

#include "Command.hh"

namespace RMS { 

class Client;

namespace Monitor {

class Service : public Command { 

public:
  Service(Client&);
public:
 ~Service() {}
public:
  void help(   int argc, const char** argv, int qualifier);        
public:
  bool _display(const DAQ::Location&, bool clear, bool header_done);
};

}}

#endif
