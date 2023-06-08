#ifndef RMS_UTILITY_SHELL
#define RMS_UTILITY_SHELL

#include "osa/Shell.hh"
#include "rms/Client.hh"

namespace RMS {namespace Utility {

class Shell : public OSA::Shell {
public:
  Shell(Client& client);
public:
  ~Shell() {}
protected:
  Client& _client;
}; 
    
}}

#endif
