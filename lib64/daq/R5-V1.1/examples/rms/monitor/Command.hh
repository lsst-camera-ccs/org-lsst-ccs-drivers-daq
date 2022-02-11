#ifndef RMS_MONITOR_COMMAND
#define RMS_MONITOR_COMMAND

#include "../Command.hh"

namespace DAQ { class LocationSet;}

namespace RMS { class Client;

namespace Monitor {

class Command : public Utility::Command { 

public:
  Command(const char* command, Client&);
public:
 ~Command() {}
public:
  void process(int argc, const char** argv, int qualifier);
  virtual void help(   int argc, const char** argv, int qualifier);
private:
  virtual bool _display(const DAQ::Location&, bool clear, bool header_done) = 0;
};

}}

#endif
