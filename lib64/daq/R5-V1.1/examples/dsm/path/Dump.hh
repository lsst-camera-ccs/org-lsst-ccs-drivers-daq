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
 
#ifndef DSM_EDITOR_PATH_DUMP
#define DSM_EDITOR_PATH_DUMP

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsi/Stack.hh"

namespace DSM {namespace Editor {namespace Path {

class Dump : public OSA::Command { 
public:
  Dump(Interface&, DSI::Stack&);
public:
 ~Dump() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Interface&  _interface;
  DSI::Stack& _stack;
};

}}}

#endif
