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
 
#ifndef HASH_ELEMENT
#define HASH_ELEMENT

#include <inttypes.h>
#include <stddef.h>

#include "linked/Element.hh"

namespace Hash {

class Element : public Linked::Element {
public:
  Element()             : Linked::Element()            {}
  Element(uint64_t key) : Linked::Element(), _key(key) {} 
public:
  Element* flink() {return (Element*)Linked::Element::flink();}   
public:
  virtual ~Element() {}
public:
  uint64_t key() const {return _key;}
public:
  void rename(uint64_t key) {extract(); _key = key;}
  void reset(uint64_t key)  {_key = key;}   
private:
  uint64_t _key;
};

}

#endif
