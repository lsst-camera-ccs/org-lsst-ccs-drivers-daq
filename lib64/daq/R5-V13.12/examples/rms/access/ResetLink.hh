#ifndef RMS_ACCESS_RESET_LINK
#define RMS_ACCESS_RESET_LINK

#include "../Command.hh"

namespace RMS { class Client;

namespace Access {

class ResetLink : public Utility::Command {

public:
  ResetLink(Client&);
public:
 ~ResetLink() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}}

#endif
