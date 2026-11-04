
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
 
#ifndef DAQ_ALL
#define DAQ_ALL

#include "daq/LocationSet.hh"

namespace DAQ {

class __attribute__((__packed__)) All : public LocationSet {
public:  
  All(); 
public:
 ~All() {}
 };

}

#endif

