
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

#ifndef BSS_REGION
#define BSS_REGION

#include "osa/File.hh"

#include <stdint.h>

namespace BSS {

class Sector; // Forward reference (see implementation file)...

class Region {
public:
  Region() = delete;  
  Region(const char* drive, const char* name); 
public:                    
 ~Region(); 
public:
  uint32_t version() const {return _version;}
public:
  unsigned size() const {return _size;}
  unsigned base() const {return _base;} 
public:
  void read(uint32_t sector, Sector*);                // One sector (512 bytes) @ base
  void read(Sector*);                                 // All sectors starting @ base 0
public:
  void write(uint32_t sector, const Sector*);         // One sector (512 bytes) @ base
  void write(const Sector*);                          // All sectors starting @ base 0
public:
  void reset(uint32_t version=0);
private:
  OSA::File _region;
  uint32_t  _base;
  uint32_t  _size;
  uint32_t  _version;
 };

}

#endif

