
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
 
#ifndef DSM_CURRENTPATH
#define DSM_CURRENTPATH

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Path.hh"

namespace DSM {

class CurrentPath {
public:
  CurrentPath() {}  
public:
  virtual ~CurrentPath() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Path paths[], int32_t modified) = 0;  
 };

}

#endif

