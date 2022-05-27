
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DSI_LOCATIONSET
#define DSI_LOCATIONSET

#include "dsi/Set.hh"
#include "dsi/Location.hh"

namespace DSI {

class __attribute__((__packed__)) LocationSet : public Set {
public:
  LocationSet()                                           : Set()                 {}
  LocationSet(Set::State any)                             : Set(any)              {}
  LocationSet(const LocationSet& a, const LocationSet& b) : Set(a, b)             {}
  LocationSet(const LocationSet& clone)                   : Set(clone)            {}
  LocationSet(const Location& location)                   : Set(location.index()) {}               
  LocationSet(const Set& set)                             : Set(set)              {}
public:  
  LocationSet(const char* locations);
  LocationSet(int argc, const char* argv[]);  
public:
 ~LocationSet() {} 
public:
  bool has(const Location& location) const {return Set::has(location.index());}
  bool had(const Location& location)       {return Set::had(location.index());}
public:
  void insert(const Location& location) {if(!location) return; Set::insert(location.index());}
public:
  bool remove(Location& location);
public: 
 const char* encode(char* buffer) const;  
 void        dump(int indent=0)   const; 
private: 
 void _insert(uint8_t slot, uint8_t bay, int bays, uint8_t rce, int rces);
 void _insert(uint8_t slot, uint8_t bay,           uint8_t rce, int rces);   
 };

}

#endif

