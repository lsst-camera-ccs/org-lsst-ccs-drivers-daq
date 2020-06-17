
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
 
#ifndef XDS_EDITOR
#define XDS_EDITOR

#include "osa/Editor.hh"
#include "dcs/Catalog.hh"
#include "xds/Store.hh"
#include "xds/TestPattern.hh"

namespace XDS {

class Editor : public OSA::Editor {
public:
  Editor(Store&, DCS::Catalog&);
public:
 ~Editor() {}
public:
  void announce();
public:  
  void _throw(const char* command, DSI::LocationSet& missing);
private:
  //Store&        _store;
 // DCS::Catalog& _catalog;        
  TestPattern   _pattern;
  }; 
                 
}

#endif
