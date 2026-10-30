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
 
#ifndef DCS_ENTRY
#define DCS_ENTRY

#include <stddef.h>

#include "linked/Element.hh"
#include "dcs/Id.hh"
#include "dcs/Key.hh"

namespace DCS {

class Freelist; // forward reference...

class Entry : public Linked::Element {
public:
 enum {MAX=63}; //  Maximum name, inclusive of termination...
public:
  static void title(int indent=0); 
public:
  void* operator new(size_t, Freelist&);
  void  operator delete(void*) {}
public:
  Entry(const Entry&) = delete;
public:  
  Entry() : _id(), _key() {}
  Entry(const char* name, const Id&);
  Entry(const char* name);
public:
 ~Entry() {_id = uint64_t(0);}
public:
  operator bool() const {return _id;} 
public:
  const Key&  key()  const {return _key;}   
  const char* name() const {return _name;}
  const Id&   id()   const {return _id;} 
public:   
  void dump(int indent=0) const;
private:
  Id    _id;
  Key   _key;
  char  _name[MAX+1];
};

}

#endif
