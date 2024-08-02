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
 
#ifndef XDS_SECTOR
#define XDS_SECTOR

#include <stdint.h>

namespace XDS {

class Sector {
public:
  enum {SIZE = 512}; // Sector size in units of bytes...
public: 
  static inline uint32_t offset(uint32_t bytes)   {return bytes / SIZE;}  
  static inline uint32_t length(uint32_t bytes)   {return (bytes + (SIZE - 1)) / SIZE;} 
  static inline uint32_t bytes( uint32_t sectors) {return sectors * SIZE;} 
public:
  Sector() {}
public:  
  Sector(const Sector&) = delete;
public:
 ~Sector() {}
};

}
 
#endif

