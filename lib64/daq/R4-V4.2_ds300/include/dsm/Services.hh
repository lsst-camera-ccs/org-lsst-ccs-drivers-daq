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
 
#ifndef DSM_SERVICES
#define DSM_SERVICES

//#include "dsid/Services.hh"

namespace DSM {

class Services { 
public:
 enum Id {DSID=0, DSM=1, DCS=2, XDS=3, RMS=4, RDS=5, SCS=6, EMU=7, GDS=8, INVALID=9};
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
public:
  Services();
public:  
  Services(const Services&) = delete;
public:
 ~Services() {}
public:
  unsigned lookup(const char* service) const;
public:
  const char* const* list() const;
public:  
  const char*  name(Id id) const {return _service[id];};
private:
  const char* _service[Id::INVALID+1];
};
}

#endif
