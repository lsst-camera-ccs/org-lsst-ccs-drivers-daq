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
 
#ifndef DSM_EDITOR_ROLE_RESET
#define DSM_EDITOR_ROLE_RESET

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsi/Stack.hh"

namespace DSM {namespace Editor {namespace Role {

class Reset : public OSA::Command { 
public:
  Reset(Interface&, DSI::Stack&);
public:
 ~Reset() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Interface&  _interface;
  DSI::Stack& _stack;
};

}}}

#endif
