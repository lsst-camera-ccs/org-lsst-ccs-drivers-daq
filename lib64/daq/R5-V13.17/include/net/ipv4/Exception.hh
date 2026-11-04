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
 
#ifndef IPV4_EXCEPTION
#define IPV4_EXCEPTION

#include <stdint.h>
#include <exception>

namespace IPV4 {

class Exception : public std::exception {
public:
  //  enum {TIMEOUT=0, INTERUPTED=-1, OFLOW=-2, UNKNOWN=-3};
public:  
  Exception();
  Exception(uint32_t address);
public:   
  Exception(const Exception&);     
public:
  Exception& operator=(const Exception& clone); 
public:
  virtual ~Exception() throw() {}  
public: 
 virtual const char* what() const throw() {return _string;}
private: 
  char _string[128];  
};

}

#endif
