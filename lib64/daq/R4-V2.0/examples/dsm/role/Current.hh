/*
** ++
**  Package:
**	
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/

#ifndef DSM_ROLEEDITOR_CURRENT
#define DSM_ROLEEDITOR_CURRENT

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/RoleProcessor.hh"

namespace DSM {namespace RoleEditor {

class Current : public DSM::RoleProcessor { 
public:
  Current();
public:
 ~Current() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Role&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

