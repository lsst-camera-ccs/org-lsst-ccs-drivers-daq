#ifndef RMS_UTILITY_QUIT
#define RMS_UTILITY_QUIT

#include "osa/Quit.hh"

namespace RMS {namespace Utility {

class Quit : public OSA::Quit { 
public:
  Quit() : OSA::Quit() {}
public:
 ~Quit() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}}

#endif
