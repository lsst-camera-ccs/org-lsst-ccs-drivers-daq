#ifndef RMS_UTILITY_COMMAND
#define RMS_UTILITY_COMMAND

#include "osa/Command.hh"
#include "rms/Client.hh"

namespace DAQ { class LocationSet;}

namespace RMS { namespace Utility {

class Command : public OSA::Command { 

public:
  Command(const char* command, RMS::Client& client) : OSA::Command(command), _client(client) {};
public:
 ~Command() {}
protected:
  RMS::Client& _client;
};

}}

#endif
