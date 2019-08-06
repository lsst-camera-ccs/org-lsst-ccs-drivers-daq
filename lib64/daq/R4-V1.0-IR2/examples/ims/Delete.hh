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
 
#ifndef IMS_EDITOR_DELETE
#define IMS_EDITOR_DELETE

#include "osa/Command.hh"
#include "ims/Store.hh"

namespace IMS {namespace Editor {

class Delete : public OSA::Command { 
public:
  Delete(Store&, const char* folder);
public:
 ~Delete() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  void _delete(const char* folder);  
private:
  Store&      _store; 
  const char* _default;
};

}}

#endif
