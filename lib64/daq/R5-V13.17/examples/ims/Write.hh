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
 
#ifndef IMS_EDITOR_WRITE
#define IMS_EDITOR_WRITE

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Write : public OSA::Command { 
public:
  Write(Store&, const char* folder);
public:
 ~Write() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Store&      _store;
  const char* _default; 
};

}}

#endif
