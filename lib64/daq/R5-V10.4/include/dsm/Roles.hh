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

namespace DSM {

class Roles { 
public:
  enum Id {MANAGEMENT=0, CATALOG=1, READOUT=2, STORAGE=3, SEQUENCER=4, EMULATION=5, PINGPONG=6, GUIDING=7, INVALID=8};
public:
  enum  {MAX=Id::INVALID};
public:
  static bool      valid(Id id) {return id != Id::INVALID;} 
  static Roles::Id lookup(const char* name);
public:
  Roles();
public:  
  Roles(const Roles&) = delete;
public:
 ~Roles() {}
public:
  Roles::Id          id(const char* name) const; 
  unsigned           all()                const {return _all;}
  const char*        name(Roles::Id id)   const {return _name[id];}
  const char* const* names()              const {return _name;}
private:
  const char* _name[MAX+1];
  unsigned    _all;
};

}

#endif
