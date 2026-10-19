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
 
#ifndef EMU_EXCEPTION
#define EMU_EXCEPTION

#include <stdint.h>
#include <exception>

namespace EMU {
  
class Exception : public std::exception {
public:
  static const char* decode(int32_t error);
public:
  enum : int32_t {NONE=0, INVALID_REQUEST = 1, NOT_ID=2, PLAYLIST_FULL=3, TIMEOUT=4};
public:  
  Exception(); 
public:  
  Exception(int error);
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
