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
 
#ifndef DSM_PROBE
#define DSM_PROBE

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsm/Editor.hh"

namespace DSM {

class Probe : public OSA::Command { 
public:
  Probe(Interface&, Editor&);
public:
 ~Probe() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Interface& _interface;
  Editor&    _editor;        
};

}

#endif
