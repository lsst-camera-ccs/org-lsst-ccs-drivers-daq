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
 
#ifndef DSM_EDITOR_ROLE_LIST
#define DSM_EDITOR_ROLE_LIST

#include "osa/Command.hh"

namespace DSM {namespace Editor {namespace Role {

class List : public OSA::Command { 
public:
  List();
public:
 ~List() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);    
};

}}}

#endif
