#ifndef RMS_ACCESS_SET_LINK
#define RMS_ACCESS_SET_LINK

#include "../Command.hh"

namespace RMS { class Client;

namespace Access {

class SetLink : public Utility::Command {

public:
  SetLink(Client&);
public:
 ~SetLink() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}}

#endif
