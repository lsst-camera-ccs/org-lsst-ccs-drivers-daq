
/*
**  Package:
**	
**
**  Abstract:
**  
** Beware: This is and must be an exact duplicate of the class by the same name found in INIT.
    
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
 
#ifndef DSM_PARTITION
#define DSM_PARTITION

#include "osa/File.hh"

namespace DSM {

class Partition {
public: 
  Partition() {_partition[0] = 0;}
  Partition(const Partition&);                     
  Partition(const char*);                   
public:
  Partition& operator=(const Partition&);
  Partition& operator=(const char*);
public:
 ~Partition() {} 
public:
  operator bool() const {return _partition[0];}
public:
  const char* value() const {return _partition;}
public:
  void read( OSA::File&);
  void write(OSA::File&);
private:
  char _partition[32];  
 };

}

#endif

