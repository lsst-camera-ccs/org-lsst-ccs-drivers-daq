
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

#ifndef BSS_SHUFFLE
#define BSS_SHUFFLE

#include "dsi/Set.hh"

namespace BSS {

class RandomSet {
public:
  RandomSet() = delete;  
  RandomSet(const DSI::Set&); 
public:                    
 ~RandomSet() {} 
public:
  uint8_t lookup(uint8_t index) const {return _set[index];}
private:  
  uint8_t* _mix(uint8_t index, uint8_t* next, unsigned remaining, unsigned remainder);
private:
  uint8_t _set[DSI::Set::SIZE];
 };

}

#endif

