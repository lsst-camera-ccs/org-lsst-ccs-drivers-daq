
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
 
#ifndef DSI_INTERFACE
#define DSI_INTERFACE

#include "net/ipv4/Interface.hh"
#include "dsi/Exception.hh"

namespace DSI {
   
class Interface : public IPV4::Interface {
public:   
  Interface(const char* network) throw (Exception);                                                                        
public:
  Interface()                 = delete;
  Interface(const Interface&) = delete;
public:
 ~Interface() {}  
  };

}

#endif


