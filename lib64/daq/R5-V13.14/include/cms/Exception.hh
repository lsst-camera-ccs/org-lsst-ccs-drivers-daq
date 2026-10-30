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
 
#ifndef CMS_EXCEPTION
#define CMS_EXCEPTION

#include <stdint.h>
#include <exception>

namespace CMS {
   
class Exception : public std::exception {
public:
  static const char* decode(int32_t error);
public:
  enum Error : int32_t {NONE=0, INVALID_OPCODE=1, SEQUENCER_TIMEOUT=2, WAKEUP_TIMEOUT=3, INVALID_IMAGE=4};
public:  
  Exception(); 
public:  
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
