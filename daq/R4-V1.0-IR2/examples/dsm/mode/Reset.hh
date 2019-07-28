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
 
#ifndef DSM_MODEEDITOR_RESET
#define DSM_MODEEDITOR_RESET

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "Shell.hh"

namespace DSM {namespace ModeEditor {

class Reset : public OSA::Command { 
public:
  Reset(Interface&, Shell&);
public:
 ~Reset() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Interface& _interface;
  Shell      _shell;        

};

}}

#endif
