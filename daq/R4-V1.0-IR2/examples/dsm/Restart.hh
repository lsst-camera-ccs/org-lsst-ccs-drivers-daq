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
 
#ifndef DSM_RESTART
#define DSM_RESTART

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsm/Editor.hh"

namespace DSM {

class Restart : public OSA::Command { 
public:
  Restart(Interface&, Editor&);
public:
 ~Restart() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:  
  Interface& _interface;
  Editor     _editor;      
};

}

#endif
