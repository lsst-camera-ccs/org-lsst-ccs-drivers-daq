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
 
#ifndef IMS_EDITOR_PROBE
#define IMS_EDITOR_PROBE

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Probe : public OSA::Command { 
public:
  Probe(Store&);
public:
 ~Probe() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Store&  _store;
};

}}

#endif
