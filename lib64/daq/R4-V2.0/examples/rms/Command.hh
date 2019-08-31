#ifndef RMS_UTILITY_COMMAND
#define RMS_UTILITY_COMMAND

#include "osa/Command.hh"

namespace DAQ { class LocationSet;}

namespace RMS { class Client;

namespace Utility {

class Command : public OSA::Command { 

public:
  Command(const char* command, Client&);
public:
 ~Command() {}
protected:
  const DAQ::LocationSet _parse_targets(int argc, const char** argv);
protected:
  Client& _client;
};

}}

#endif
