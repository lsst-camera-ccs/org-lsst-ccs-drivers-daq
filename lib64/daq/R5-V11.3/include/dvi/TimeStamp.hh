
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
 
#ifndef DVI_TIMESTAMP
#define DVI_TIMESTAMP

#include <stdint.h>
#include <time.h>

namespace DVI {

class __attribute__((__packed__)) TimeStamp { 
public:
 TimeStamp(); 
 TimeStamp(bool) : _timestamp(uint64_t(0)) {}
 TimeStamp(time_t);        
 TimeStamp(const TimeStamp& clone) : _timestamp(clone._timestamp) {} 
public:
 ~TimeStamp() {}
public:  
  TimeStamp& operator=(const TimeStamp& clone) {_timestamp  = clone._timestamp; return *this;} 
public:
  bool operator<( const TimeStamp&) const;
  bool operator<=(const TimeStamp&) const;
  bool operator>( const TimeStamp&) const;
  bool operator==(const TimeStamp&) const;
public:  
  const char* decode() const;
  const char* decode(char* buffer) const;
public:
  operator uint64_t() const;    
private:   
 uint64_t _timestamp;
 };

}

#endif

