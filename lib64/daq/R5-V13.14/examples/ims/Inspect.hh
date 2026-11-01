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
 
#ifndef IMS_EDITOR_INSPECT
#define IMS_EDITOR_INSPECT

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Inspect : public OSA::Command { 
public:
  Inspect(Store&, const char* folder);
public:
 ~Inspect() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Store&      _store;
  const char* _default; 
};

}}

#endif
