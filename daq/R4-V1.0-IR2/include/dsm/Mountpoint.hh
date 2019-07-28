
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
 
#ifndef DSM_MOUNTPOINT
#define DSM_MOUNTPOINT

#include "osa/File.hh"

namespace DSM {

class Mountpoint {
public: 
  Mountpoint() {_mountpoint[0] = 0;}
  Mountpoint(const Mountpoint&);                     
  Mountpoint(const char*);                   
public:
  Mountpoint& operator=(const Mountpoint&);
  Mountpoint& operator=(const char*);
public:
 ~Mountpoint() {} 
public:
  operator bool() const {return _mountpoint[0];}
public:
  const char* value() const {return _mountpoint;}
public:
  void read( OSA::File&);
  void write(OSA::File&);
private:
  char _mountpoint[256];  
 };

}

#endif

