
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
 
#ifndef DSM_ROLE
#define DSM_ROLE

#include "osa/File.hh"

namespace DSM {

class Role {
public:
   enum Id {MANAGEMENT=0, CATALOG=1, READOUT=2, STORAGE=3, SEQUENCER=4, EMULATION=5, INVALID=6};
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
public: 
  Role() {_role[0] = 0;}
  Role(const Role&);                     
  Role(const char*);                   
public:
  Role& operator=(const Role&);
  Role& operator=(const char*);
public:
 ~Role() {} 
public:
  operator bool() const {return _role[0];}
public:
  const char* value() const {return _role;}
public:
  void read( OSA::File&);
  void write(OSA::File&);
private:
  char _role[32];  
 };

}

#endif

