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
 
#ifndef IMS_EDITOR_CAPACITY
#define IMS_EDITOR_CAPACITY

#include "osa/Command.hh"
#include "xds/Page.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Capacity : public OSA::Command { 
public:
  Capacity(Store&);
public:
 ~Capacity() {}
public:
  unsigned long long bytes(unsigned pages) const {return (unsigned long long)pages << XDS::Page::SIZE2;}   
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Store&  _store;
};

}}

#endif
