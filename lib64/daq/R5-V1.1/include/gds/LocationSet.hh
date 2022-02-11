 
#ifndef GDS_LOCATIONSET
#define GDS_LOCATIONSET

#include "gds/Set.hh"
#include "gds/Location.hh"

namespace GDS {

class __attribute__((__packed__)) LocationSet : public Set {
public:
  LocationSet() : Set() {}
public:  
  LocationSet(const Location& location)                   : Set(location.index())       {}
  LocationSet(const LocationSet& a, const LocationSet& b) : Set(a, b)                   {}
  LocationSet(const Set& set)                             : Set(set)                    {}
  LocationSet(State dont_care)                            : Set(dont_care)              {}
//public:  
//  LocationSet(const char* locations);
//  LocationSet(int argc, const char* argv[]);    
public:
 ~LocationSet() {} 
public:
  bool has(const Location& location) const {return Set::has(location.index());}
  bool had(const Location& location)       {return Set::had(location.index());}
public:
  void insert(const Location& location) {Set::insert(location.index());}
public:
  bool remove(Location&);
public: 
 const char* encode(char* buffer) const;
// void        dump(int indent=0)   const;
private: 
 void _insert(uint8_t bay, uint8_t board, int boards);
 };

}

#endif

