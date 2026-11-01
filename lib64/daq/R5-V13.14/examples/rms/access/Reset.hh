#ifndef RMS_ACCESS_RESET
#define RMS_ACCESS_RESET

#include "../Command.hh"

namespace RMS { class Client;

namespace Access {

class Reset : public Utility::Command { 

public:
  Reset(Client&);
public:
 ~Reset() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
};

}}

#endif
