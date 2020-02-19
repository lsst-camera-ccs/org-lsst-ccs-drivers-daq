
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
 
#ifndef IMS_SHELL
#define IMS_SHELL

#include "osa/Editor.hh"
#include "ims/Store.hh"

namespace IMS {

class Shell : public OSA::Editor {
public:
  Shell(Store&, const char* folder);
public:
 virtual ~Shell() {}
public:
  virtual void announce();
private:
  Store& _store;
  }; 
                 
}

#endif
