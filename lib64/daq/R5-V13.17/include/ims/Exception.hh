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
 
#ifndef IMS_EXCEPTION
#define IMS_EXCEPTION

#include <stdint.h>
#include <exception>

namespace IMS {
  
class Exception : public std::exception {
public:
  static const char* decode(int32_t error);
public:
  enum Error : int32_t {NONE=0, INVALID_REQUEST=64, NOSUCH_IMAGE=65, NOSUCH_SOURCE=66, NOMORE_FREESPACE=67, TIMOUT=68, NOSUCH_DATA=69};
public:  
  Exception(); 
public:  
  Exception(int32_t error, const char* folder, const char* name);
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
