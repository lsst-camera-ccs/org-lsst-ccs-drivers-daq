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
 
#ifndef DSM_EDITOR_SHELL
#define DSM_EDITOR_SHELL

#include "osa/Shell.hh"
#include "dsi/Stack.hh"
#include "dsm/Interface.hh"

namespace DSM {namespace Editor {

class Shell : public OSA::Shell {
public:
  Shell(Interface&);
public:
  Shell()             = delete;
  Shell(const Shell&) = delete;
public:
 ~Shell() {}
public:
  void announce(); 
public:
  DSI::Stack& stack() {return _stack;}
private:
  Interface& _interface;
  DSI::Stack _stack;
  }; 
                 
}}

#endif
