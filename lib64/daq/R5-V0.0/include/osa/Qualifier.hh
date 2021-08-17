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
 
#ifndef OSA_QUALIFIER
#define OSA_QUALIFIER

#include "hash/Element.hh"
#include "hash/Key.hh"

namespace OSA {

class Qualifier : public Hash::Element { 
public:
  enum uint32_t {SEED=0xABADBABE};
public:
  static uint64_t key(const char* name) {return Hash::Key(name, SEED);}
public:
  Qualifier(); 
  Qualifier(const char* name, int index, const char* help);
public:
 ~Qualifier() {}
public:
  const char* name() const {return _name;} 
  const char* help() const {return _help;}
public:
  unsigned index() const {return _index;}
public:   
  Qualifier* dump(int indent=0);
  Qualifier* dump(const Qualifier* until, int indent=0);      
public:
  Qualifier* flink() {return (Qualifier*)Hash::Element::flink();}
private:
  const char* _name;
  const char* _help;
  int        _index; 
};

}

#endif
