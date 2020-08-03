
/*
**  Package:
**	
**
**  Abstract:
** 
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DSM_EDITOR
#define DSM_EDITOR

#include "dvi/Version.hh"
#include "osa/Editor.hh"
#include "dsm/Interface.hh"

namespace DSM {

class Editor : public OSA::Editor {
public:
 enum Index {ZERO=1, ONE=2, TWO=3, MAX=3};
 enum Value {CURRENT=4, PENDING=5};
public:
  Editor(Interface&);
public:
 ~Editor() {}
public:
  void announce(); 
public:
  const DVI::Version& version() const {return _version;}    
public:
  void _throw(const char* command, DSI::LocationSet&);
private:
  DVI::Version _version;
  Interface&   _interface;
  }; 
                 
}

#endif
