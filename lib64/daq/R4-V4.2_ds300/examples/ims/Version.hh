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
 
#ifndef IMS_EDITOR_VERSION
#define IMS_EDITOR_VERSION

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Version : public OSA::Command { 
public:
  Version(Store&);
public:
 ~Version() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Store&  _store;
};

}}

#endif
