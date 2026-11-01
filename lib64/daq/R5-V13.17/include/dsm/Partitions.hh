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
 
#ifndef DSM_PARTITIONS
#define DSM_PARTITIONS

namespace DSM {

class Partitions { 
public:
  enum Id {SYSTEM=0, INVALID=32, NONE=32};
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
public:
  Partitions();
public:  
  Partitions(const Partitions&) = delete;
public:
 ~Partitions() {}
};
}

#endif
