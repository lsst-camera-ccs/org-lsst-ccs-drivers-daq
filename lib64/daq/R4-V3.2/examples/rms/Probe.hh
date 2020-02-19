#ifndef RMS_UTILITY_PROBE
#define RMS_UTILITY_PROBE

#include "osa/Command.hh"

#include "rms/Client.hh"

namespace RMS { namespace Utility {

class Probe : public OSA::Command { 
public:
  Probe(Client&);
public:
 ~Probe() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Client& _client;
};

}}

#endif
