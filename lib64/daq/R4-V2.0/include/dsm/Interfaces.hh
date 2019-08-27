
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

namespace DSM {

class Interfaces {
public:
  static DAQ::Interface* startup();
  static DAQ::Interface* lookup();
public: 
  Interfaces()                  = delete;
  Interfaces(const Interfaces&) = delete;                     
public:
 ~Interfaces() {} 
 };

}

#endif

