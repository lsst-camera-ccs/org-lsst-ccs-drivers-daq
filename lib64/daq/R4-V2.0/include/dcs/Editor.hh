
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
 
#ifndef DCS_EDITOR
#define DCS_EDITOR

#include "osa/Editor.hh"
#include "dsi/LocationSet.hh"
#include "dcs/Catalog.hh"

namespace DCS {

class Editor : public OSA::Editor {
public:
  Editor(Catalog& catalog);
public:
 ~Editor() {}
public:
  void announce();
public:  
  void _throw(const char* command, DSI::LocationSet& missing);
private:
  Catalog& _catalog;
  }; 
                 
}

#endif
