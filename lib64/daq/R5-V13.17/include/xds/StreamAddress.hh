
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
 
#ifndef XDS_STREAMADDRESS
#define XDS_STREAMADDRESS

#include "dsi/StreamAddress.hh"
#include "dsm/Streams.hh"

namespace XDS {

class  StreamAddress : public DSI::StreamAddress {
public:
  StreamAddress(uint8_t pid)                : DSI::StreamAddress(pid, DSM::Streams::Id::XDS) {}    
  StreamAddress(const StreamAddress& clone) : DSI::StreamAddress(clone) {}
public:
  StreamAddress() = delete;
public:
 ~StreamAddress() {} 
public: 
  StreamAddress& operator=(const StreamAddress& clone) {DSI::StreamAddress::operator=(clone); return *this;}                
 };

}

#endif

