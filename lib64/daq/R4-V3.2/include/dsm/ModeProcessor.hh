
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
 
#ifndef DSM_MODEPROCESSOR
#define DSM_MODEPROCESSOR

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Mode.hh"

namespace DSM {

class ModeProcessor  {
public:
  ModeProcessor() {}  
public:
  virtual ~ModeProcessor() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Mode&, int32_t modified) = 0;  
 };

}

#endif

