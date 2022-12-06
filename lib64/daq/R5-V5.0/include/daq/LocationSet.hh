
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
 
#ifndef DAQ_LOCATIONSET
#define DAQ_LOCATIONSET

#include "dsi/Set.hh"
#include "daq/Location.hh"

namespace DAQ {

class __attribute__((__packed__)) LocationSet : public DSI::Set {
public:
  LocationSet() : DSI::Set() {}
public:  
  LocationSet(const Location& location)                   : Set(location.index())       {}
  LocationSet(const LocationSet& a, const LocationSet& b) : Set(a, b)                   {}
  LocationSet(const DSI::Set& set)                        : Set(set)                    {}
public:  
  LocationSet(const char* locations);
  LocationSet(int argc, const char* argv[]);    
public:
 ~LocationSet() {} 
public:
  bool has(const Location& location) const {return DSI::Set::has(location.index());}
  bool had(const Location& location)       {return DSI::Set::had(location.index());}
public:
  void insert(const Location& location) {DSI::Set::insert(location.index());}
public:
  bool remove(Location&);
public: 
 const char* encode(char* buffer) const;
 const char* encode()             const;
 void        dump(int indent=0)   const;
private: 
 void _insert(uint8_t bay, uint8_t board, int boards);
 };

}

#endif

