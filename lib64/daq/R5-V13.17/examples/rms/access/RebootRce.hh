#ifndef RMS_ACCESS_REBOOT_RCE
#define RMS_ACCESS_REBOOT_RCE

#include "../Command.hh"

namespace RMS { class Client;

namespace Access {

class RebootRce : public Utility::Command {

public:
  RebootRce(Client&);
public:
 ~RebootRce() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}}

#endif
