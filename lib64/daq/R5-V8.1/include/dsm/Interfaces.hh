
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
 
#ifndef DSM_INTERFACES
#define DSM_INTERFACES

#include "daq/Interface.hh"
#include "dsm/Roles.hh"

namespace DSM {

class Interfaces {
public:
  static const DAQ::Interface* setup(Roles::Id role);
  static const DAQ::Interface* lookup();
public:
  Interfaces(Roles::Id role);
public: 
  Interfaces()                  = delete;
  Interfaces(const Interfaces&) = delete;
public:  
 ~Interfaces() {}
private: 
  void _internal(DAQ::Interface::Index); 
  void _external(DAQ::Interface::Index);  
private:
  DAQ::Interface _interface[3]; 
 };

}

#endif

