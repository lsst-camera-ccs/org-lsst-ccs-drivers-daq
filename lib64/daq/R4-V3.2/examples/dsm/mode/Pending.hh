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

#ifndef DSM_MODEEDITOR_PENDING
#define DSM_MODEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/ModeProcessor.hh"

namespace DSM {namespace ModeEditor {

class Pending : public DSM::ModeProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Mode&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

