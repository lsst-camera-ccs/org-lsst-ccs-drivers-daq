#ifndef RMS_MONITOR_SHELL
#define RMS_MONITOR_SHELL

#include "../Shell.hh"

namespace RMS {

class Client;

namespace Monitor {

class Shell : public Utility::Shell {
public:
  Shell(Client& client);
public:
  ~Shell() {}
public:
  void announce();
}; 
  
}}

#endif
