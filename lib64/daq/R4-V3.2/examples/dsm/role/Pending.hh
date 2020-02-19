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

#ifndef DSM_ROLEEDITOR_PENDING
#define DSM_ROLEEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/RoleProcessor.hh"

namespace DSM {namespace RoleEditor {

class Pending : public DSM::RoleProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Role&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

