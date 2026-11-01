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
 
#ifndef IMS_EDITOR_REMAINING
#define IMS_EDITOR_REMAINING

#include "osa/Command.hh"
#include "xds/Page.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Remaining : public OSA::Command { 
public:
  Remaining(Store&);
public:
 ~Remaining() {}
public:
  unsigned long long bytes(unsigned pages) const {return (unsigned long long)pages * XDS::Page::SIZE;}   
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Store& _store;
};

}}

#endif
