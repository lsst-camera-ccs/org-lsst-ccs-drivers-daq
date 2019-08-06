
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef XDS_STREAMADDRESS
#define XDS_STREAMADDRESS

#include "dsid/ServiceAddress.hh"

namespace XDS {

class StreamAddress : public DSID::ServiceAddress {
public:
 enum : unsigned  {STREAM = 16};
public:
  StreamAddress() = delete;  
public:
  StreamAddress(unsigned partition)         :  DSID::ServiceAddress(STREAM, partition) {} 
  StreamAddress(const StreamAddress& clone) :  DSID::ServiceAddress(clone)             {} 
public:  
 ~StreamAddress() {}
  }; 

}

#endif

