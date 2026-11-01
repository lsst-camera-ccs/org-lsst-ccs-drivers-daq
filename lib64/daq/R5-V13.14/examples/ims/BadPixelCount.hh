#ifndef IMS_EDITOR_BADPIXELCOUNT
#define IMS_EDITOR_BADPIXELCOUNT

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class BadPixelCount : public OSA::Command { 
public:
  BadPixelCount(Store&, const char* folder);
public:
 ~BadPixelCount() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Store&      _store; 
  const char* _default;
};

}}

#endif
