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
 
#ifndef IMS_EDITOR_LIST
#define IMS_EDITOR_LIST

#include "osa/Command.hh"
#include "ims/Store.hh"
#include "ims/Id.hh"
#include "ims/Image.hh"

namespace IMS {namespace Editor {

class List : public OSA::Command { 
public:
  List(Store&);
public:
 ~List() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  void _process(const Id&, Image&);
private:
  Store& _store;        
};

}}

#endif
