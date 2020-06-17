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
 
#ifndef DSM_ROLES
#define DSM_ROLES

#include "dsm/Role.hh"

namespace DSM {

class Roles { 
public:
  Roles();
public:  
  Roles(const Roles&) = delete;
public:
 ~Roles() {}
public:
  unsigned lookup(const char* role) const;
public:  
  unsigned all() const {return _all;}
public:  
  const char* name(Role::Id id) const {return Role::valid(id) ? _name[id] : (const char*)0;}
private:
  unsigned    _all;
  unsigned    _role[Role::Id::INVALID];
  const char* _name[Role::Id::INVALID+1];
};

}

#endif
