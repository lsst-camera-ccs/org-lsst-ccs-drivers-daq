
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DSM_ROLE
#define DSM_ROLE

#include "dsm/String.hh"
#include "dsm/Functions.hh"

namespace DSM {

class Role : public String {
public: 
  Role(const char* value) : String(GET_ROLE, SET_ROLE, RESET_ROLE, value) {} 
  Role()                  : String(GET_ROLE, SET_ROLE, RESET_ROLE)        {} 
public:
  Role(const Role&) = delete;  
public:
  virtual ~Role() {}

 }; 
}

#endif

