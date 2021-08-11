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

#ifndef DSM_EDITOR_ROLE_PREFERENCE
#define DSM_EDITOR_ROLE_PREFERENCE

#include "dsi/Location.hh"
#include "dsm/Role.hh"

namespace DSM {namespace Editor {namespace Role {

class Preference : public DSM::Role { 
public:
  Preference(const char* value);
  Preference();
public:
  Preference(const Preference&) = delete;
public:
 ~Preference() {}
public:
  void process(const DSI::Location&, const char* value, unsigned pending);
public:  
  void summary() const;
private:
  int _total; 
};

}}}

#endif

