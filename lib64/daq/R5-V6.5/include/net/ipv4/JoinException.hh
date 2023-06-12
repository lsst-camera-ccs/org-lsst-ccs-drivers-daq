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

#ifndef IPV4_JOIN_EXCEPTION
#define IPV4_JOIN_EXCEPTION

#include <stdint.h>
#include <exception>

#include "net/ipv4/Address.hh"

namespace IPV4 {

class JoinException : public std::exception {
public:  
  JoinException() : std::exception() {}
  JoinException(const Address& group, int error=0);
public:   
  JoinException(const JoinException&);     
public:
  JoinException& operator=(const JoinException& clone); 
public:
  virtual ~JoinException() throw() {}  
public: 
  virtual const char* what() const throw() {return _string;}
private: 
  char _string[128];  
};

}

#endif
