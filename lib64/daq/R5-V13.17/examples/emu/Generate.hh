/*
** ++
**  Package:
**	
**
**  Abstract:
**      
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef EMU_EDITOR_GENERATE
#define EMU_EDITOR_GENERATE

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace EMU {namespace Editor {

class Generate : public OSA::Command { 
public:
  Generate(IMS::Store&, const char* folder);
public:
 ~Generate() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  IMS::Store&  _store;
  const char*  _default;
};

}}

#endif
