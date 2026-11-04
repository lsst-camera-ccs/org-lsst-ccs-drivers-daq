/*
** ++
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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef IMS_ID
#define IMS_ID

#include "dcs/Id.hh"

namespace IMS {

class __attribute__((__packed__)) Id : public DCS::Id {
public:
  Id()                  : DCS::Id(DCS::Id::random()) {}
  Id(bool isnull)       : DCS::Id(isnull)            {}
  Id(uint64_t id)       : DCS::Id(id)                {}
  Id(const DCS::Id& id) : DCS::Id(id)                {}
public:
 ~Id() {}
};

}
 
#endif

