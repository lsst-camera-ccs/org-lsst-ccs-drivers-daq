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
 
#ifndef DSM_SOURCEEDITOR_SET
#define DSM_SOURCEEDITOR_SET

#include "osa/Command.hh"
#include "dsm/Interface.hh"

namespace DSM {namespace SourceEditor {

class Set : public OSA::Command { 
public:
  Set(Interface&);
public:
 ~Set() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Interface& _interface;
};

}}

#endif
