#ifndef RMS_ACCESS_SHELL
#define RMS_ACCESS_SHELL

#include "../Shell.hh"

namespace RMS {

class Client;

namespace Access {

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
