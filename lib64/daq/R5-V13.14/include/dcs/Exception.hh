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
 
#ifndef DCS_EXCEPTION
#define DCS_EXCEPTION

#include <stdint.h>
#include <exception>

namespace DCS {

class Exception : public std::exception {
public:
  static const char* decode(int error);
public:
  enum : int32_t {NONE=0, NOSUCH_NAME=1, NOSUCH_FOLDER=2, DUPLICATE_NAME=3, DUPLICATE_FOLDER=4, INVALID_NAME=5, INVALID_FOLDER=6, NO_POST=7, TIMEOUT=8};
public:  
  Exception()                       : std::exception(),      _error(decode(INVALID_NAME+1)) {}
  Exception(int error)              : std::exception(),      _error(decode(error))         {}
  Exception(const Exception& clone) : std::exception(clone), _error(clone._error)          {}
public:
  Exception& operator=(const Exception& clone) { _error = clone._error; return *this;}   
public:
  virtual ~Exception() throw() {}  
public: 
 virtual const char* what() const throw() {return _error;}
private: 
  const char* _error;  
};

}

#endif
