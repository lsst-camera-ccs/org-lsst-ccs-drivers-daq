
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
 
#ifndef DAQ_LOCATION
#define DAQ_LOCATION

#include <stdint.h>

namespace DAQ {

class __attribute__((__packed__)) Location  {
public: 
  enum Board : uint8_t {ZERO=0, ONE=1, TWO=2, WILDCARD=3};
public:  
  enum : uint8_t {MAX_BAYS=45, SYSTEM=MAX_BAYS, MAX_BOARDS=3}; 
  enum : uint8_t {UNDEFINED=0x80}; 
  enum : uint8_t {MAX_ELEMENTS=2};  
public:  
  Location()                      : _location(UNDEFINED)       {} 
  Location(uint8_t location)      : _location(location)        {} 
  Location(const Location& clone) : _location(clone._location) {}
  Location(uint8_t bay, enum Board);
  Location(int argc, const char* argv[]); 
  Location(const char* location);
public:
 ~Location() {}
public:
  uint8_t    bay()    const;
  enum Board board()  const;
  unsigned   boards() const {return board() == WILDCARD ? MAX_BOARDS : 1;}
public:  
  uint8_t index() const {return _location;}
public:  
  Location& operator=(const Location& clone) {_location = clone._location; return *this;} 
  Location& operator=(uint8_t location)      {_location = location;        return *this;}
  Location& operator=(const char* location); 
public:
  operator bool() const {return _location != UNDEFINED;} 
public:
  void reset() {_location = UNDEFINED;} 
public:
  const char* encode(char* buffer) const;         
public:  
  void dump(int indent=0) const;
private:
  uint8_t _location;
 };

}

#endif

