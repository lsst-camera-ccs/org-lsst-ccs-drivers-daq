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

#ifndef DSM_SOURCEEDITOR_PENDING
#define DSM_SOURCEEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/SourceProcessor.hh"

namespace DSM {namespace SourceEditor {

class Pending : public DSM::SourceProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Source sources[], int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

