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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef DSI_LOCATION
#define DSI_LOCATION

#include <stdint.h>

namespace DSI {

class __attribute__((__packed__)) Location {
public:
  enum : uint8_t {UNDEFINED=0x80};
public:  
  enum {MAX_ELEMENTS=3};
public:    
  enum : uint8_t {MAX_RCES=2, MAX_BAYS=4, MAX_SLOTS=16};
public: 
  Location()                      : _location(UNDEFINED) {}
  Location(uint8_t location)      : _location(location)  {} 
  Location(const Location& clone) : _location(clone._location) {}
public:
  Location(uint8_t slot, uint8_t bay, uint8_t rce); 
  Location(const char*);                   
  Location(int argc, const char* argv[]);                   
public:  
  Location& operator=(const Location& clone) {_location = clone._location; return *this;}
  Location& operator=(uint8_t clone)         {_location = clone;           return *this;}
public:  
       operator bool() const                      {return _location != UNDEFINED;}
  bool operator ==(const Location& operand) const {return operand._location == _location;} 
public:
 ~Location() {} 
public:
  uint8_t rce()   const;      
  uint8_t bay()   const; 
  uint8_t slot()  const;      
  uint8_t index() const {return _location;}      
public:
  const char* encode(char* buffer) const;  
  void        dump(int indent=0)   const;
private:
  uint8_t _location;
};

}

#endif
