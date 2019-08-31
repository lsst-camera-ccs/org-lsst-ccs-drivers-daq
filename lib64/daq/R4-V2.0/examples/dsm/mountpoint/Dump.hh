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
 
#ifndef DSM_MOUNTPOINTEDITOR_DUMP
#define DSM_MOUNTPOINTEDITOR_DUMP

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "Shell.hh"

namespace DSM {namespace MountpointEditor {

class Dump : public OSA::Command { 
public:
  Dump(Interface&, Shell&);
public:
 ~Dump() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Interface& _interface;
  Shell      _shell;        
};

}}

#endif
