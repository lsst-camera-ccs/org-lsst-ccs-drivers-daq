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
 
#ifndef DSM_PARTITIONEDITOR_RESET
#define DSM_PARTITIONEDITOR_ReSET

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "Shell.hh"

namespace DSM {namespace PartitionEditor {

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
