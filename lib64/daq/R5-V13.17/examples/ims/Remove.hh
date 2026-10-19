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
 
#ifndef IMS_EDITOR_REMOVE
#define IMS_EDITOR_REMOVE

#include "osa/Command.hh"
#include "ims/Catalog.hh"

namespace IMS {namespace Editor {

class Remove : public OSA::Command { 
public:
  Remove(Catalog&);
public:
 ~Remove() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Catalog&  _catalog; 
};

}}

#endif
