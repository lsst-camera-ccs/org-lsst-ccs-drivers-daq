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
 
#ifndef DSM_RELEASEPOINT
#define DSM_RELEASEPOINT

#include "osa/File.hh"

namespace DSM {

class Releasepoint {
public: 
  Releasepoint() {_releasepoint[0] = 0;}
  Releasepoint(const Releasepoint&);                     
  Releasepoint(const char*);                   
public:
  Releasepoint& operator=(const Releasepoint&);
  Releasepoint& operator=(const char*);
public:
 ~Releasepoint() {} 
public:
  operator bool() const {return _releasepoint[0];}
public:
  const char* value() const {return _releasepoint;}
public:
  void read( OSA::File&);
  void write(OSA::File&);
private:
  char _releasepoint[256];  
 };

}

#endif
