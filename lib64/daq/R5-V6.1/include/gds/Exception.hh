 
#ifndef GDS_EXCEPTION
#define GDS_EXCEPTION

#include <exception>
#include <stdint.h>

namespace GDS {

class Exception : public std::exception {
public:
  static const char* decode(int32_t error, void* buffer);
public:
  enum Id : int32_t {COMMAND_TIMEOUT=-1, NOSPACE=-2, POST_FAIL=-3, SUCCESS=0, INVALID_PARAMETER=1, INVALID_TRANSITION=2, INVALID_COMMAND=3, CONFIG_FAILED=4, INVALID=5, UNKNOWN=6};
public:  
  Exception(); 
public:  
  Exception(Id id);
public:   
  Exception(const Exception&);     
public:
  Exception& operator=(const Exception& clone); 
public:
  virtual ~Exception() throw() {}  
public: 
 virtual const char* what() const throw() {return _string;}
protected: 
  char _string[256];  
};

  
class ParamException : public Exception {
public:
  ParamException() = delete;
  ParamException(const char* what);
};

}

#endif
