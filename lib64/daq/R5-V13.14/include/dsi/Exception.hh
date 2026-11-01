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
 
#ifndef DSI_EXCEPTION
#define DSI_EXCEPTION

#include "dsi/Location.hh"

#include <exception>

namespace DSI {

class Exception : public std::exception {
public:
  static const char* decode(int32_t error);
public:
  enum Error : int32_t {NONE = 0, TIMOUT=-1, INVALID_RESPONSE=-2, DUPLICATE=-3, INVALID_REQUEST=-4, NOSUCH_INTERFACE=-5, NOSUCH_PARTITION=-6, NOSUCH_SERVICE=-7, ASSIGNED=-8};
public:  
  Exception(); 
  Exception(const char* network);
public: 
  Exception(const char* service, const char* partition);
  Exception(const char* service, const Location&);
  Exception(const char* service, uint8_t pid);
public:   
  Exception(const Exception&);     
public:
  Exception& operator=(const Exception& clone); 
public:
  virtual ~Exception() throw() {}  
public: 
 virtual const char* what() const throw() {return _string;}
private: 
  char _string[256];  
};

}

#endif
