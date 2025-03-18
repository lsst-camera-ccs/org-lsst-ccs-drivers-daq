/*
** ++
**  Package: SMI
**	
**
**  Abstract: LSST Storage Manager Interface
**
**  Implementation of LSST Storage Manager Interface
**
**  Author:
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - November 04, 2018
**
**  Revision History:
**	None.
**
** --
*/
 
 
#ifndef SATA_EXCEPTION
#define SATA_EXCEPTION

#include <stdint.h>
#include <exception>

namespace SATA {

class Exception : public std::exception {
public:
  static const char* decode(int error);
public:
  enum : int32_t {NO_ERROR=0, INSF_PLUGINS=1, INSF_SECTORS=2, NO_SVT=3, NOSUCH_ERROR=4};
public:  
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
