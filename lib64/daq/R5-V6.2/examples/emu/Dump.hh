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
 
#ifndef EMU_EDITOR_DUMP
#define EMU_EDITOR_DUMP

#include "osa/Command.hh"
#include "ims/Store.hh"
#include "emu/PlayList.hh"

namespace EMU {namespace Editor {

class Dump : public OSA::Command { 
public:
  Dump(IMS::Store&, PlayList*);
public:
 ~Dump() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  IMS::Store& _store;
  PlayList*   _playlist; 
};

}}

#endif
