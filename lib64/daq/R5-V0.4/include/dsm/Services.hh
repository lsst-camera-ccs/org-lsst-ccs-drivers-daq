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

#include "dsm/Roles.hh"

namespace DSM {

class Roles;

class Services { 
public:
  enum Id {DSID=1, DSM=2, DCS=3, XDS=4, RMS=5, RDS=6, SCS=7, EMU=8, GDS=9, PPS=10, LAM=11, INVALID=12};
public:
  enum {MAX=Id::INVALID}; 
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
  static unsigned lookup(Roles::Id);
public:
  Services();
public:  
  Services(const Services&) = delete;
public:
 ~Services() {}
public:
  const char*        name(Services::Id id) const {return _name[id];}
  const char* const* names()               const {return _name;}
  unsigned           list(Roles::Id id)    const {return _services[id];}
  Services::Id id(const char*)             const;
private:
  const char* _name[Services::MAX + 1];
  unsigned    _services[Roles::MAX + 1];
};

}

#endif
