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
 
#ifndef DSM_EXCEPTION
#define DSM_EXCEPTION

#include <stdint.h>
#include <exception>

#include "dsi/Location.hh"

namespace DSM {

class Exception : public std::exception {
public:
  static const char* decode(int32_t error);
public:
  enum : int32_t {NONE=0, INVALID_RESPONSE=1, NOSUCH_PARTITION=2, NOSUCH_ROLE=3, INVALID_REQUEST=4, NOSUCH_LOCATION=5, NOSUCH_SERVICE=6, TIMOUT=7};
  enum {MAX_STRING=127};
public:  
  Exception();
  Exception(int32_t error); 
  Exception(int32_t error, const char* name); 
  Exception(const Exception&);
public:
  Exception& operator=(const Exception& clone); 
public:
  virtual ~Exception() throw() {}  
public: 
  virtual const char* what() const throw() {return _string;}
private: 
  char _string[MAX_STRING+1];
};

}

#endif
