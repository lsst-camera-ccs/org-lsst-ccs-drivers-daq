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
 
#ifndef XDS_EXCEPTION
#define XDS_EXCEPTION

#include <stdint.h>
#include <exception>

namespace XDS {
  
class Exception : public std::exception {
public:
  enum Error : int32_t {NONE=0, INVALID_REQUEST=32, NOSUCH_PAGE=33, DUPLICATE_PAGE=34, NOMORE_FREESPACE=35, TIMOUT=36, NOSUCH_BUCKET=37, NOMORE_BUFFERS=38, INVALID_SYSCON=39};
public:
  static const char* decode(Exception::Error);
public:  
  Exception(); 
public:  
  Exception(Exception::Error);
public:   
  Exception(const Exception&);     
public:
  Exception& operator=(const Exception& clone); 
public:
  virtual ~Exception() throw() {}  
public: 
 virtual const char* what() const throw() {return _error;}
private: 
  const char* _error;  
};

}

#endif
