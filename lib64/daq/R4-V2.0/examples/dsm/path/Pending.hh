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

#ifndef DSM_PATHEDITOR_PENDING
#define DSM_PATHEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/PathProcessor.hh"

namespace DSM {namespace PathEditor {

class Pending : public DSM::PathProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Path paths[], int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

