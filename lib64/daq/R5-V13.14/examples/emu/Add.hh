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
 
#ifndef EMU_EDITOR_ADD
#define EMU_EDITOR_ADD

#include "osa/Command.hh"
#include "ims/Store.hh"
#include "emu/PlayList.hh"

namespace EMU {namespace Editor {

class Add : public OSA::Command { 
public:
  Add(IMS::Store&, const char* folder, PlayList*);
public:
 ~Add() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  IMS::Store& _store;
  const char* _folder; 
  PlayList*   _playlist;
};

}}

#endif
