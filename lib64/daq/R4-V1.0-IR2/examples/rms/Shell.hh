#ifndef RMS_UTILITY_SHELL
#define RMS_UTILITY_SHELL

#include "osa/Editor.hh"
#include "rms/Client.hh"

namespace RMS {namespace Utility {

class Shell : public OSA::Editor {
public:
  Shell(Client& client);
public:
  ~Shell() {}
protected:
  Client& _client;
}; 
    
}}

#endif
