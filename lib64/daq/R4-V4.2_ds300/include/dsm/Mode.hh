
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
 
#ifndef DSM_MODE
#define DSM_MODE

#include "osa/File.hh"

namespace DSM {

class Mode {
public:
  enum Id {ONLINE=0, OFFLINE=1, EMULATE=2, INVALID=3};
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
public: 
  Mode() {_mode[0] = 0;}
  Mode(const Mode&);                     
  Mode(const char*);                   
public:
  Mode& operator=(const Mode&);
  Mode& operator=(const char*);
public:
 ~Mode() {} 
public:
  operator bool() const {return _mode[0];}
public:
  const char* value() const {return _mode;}
public:
  void read( OSA::File&);
  void write(OSA::File&);
private:
  char _mode[32];  
 };

}

#endif

