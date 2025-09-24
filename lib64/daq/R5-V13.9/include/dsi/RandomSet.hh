
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

#ifndef DSI_RANDOMSET
#define DSI_RANDOMSET

#include <stdint.h>

#include "dsi/Set.hh"

namespace DSI {

class RandomSet {
public:
  RandomSet() = delete;  
  RandomSet(const Set&); 
public:                    
 ~RandomSet() {}
public:
  uint8_t lookup(uint64_t index) const {return _set[index%_lutsize];}
private:
  unsigned _lutsize;
  uint8_t  _set[Set::SIZE];
 };

}

#endif

