
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef OSA_BUCKET
#define OSA_BUCKET

#include <stddef.h>

namespace OSA {
  
class Bucket {
public:
  static int pagesize();  
public:
  Bucket();
  Bucket(const char* backing_file);
  Bucket(size_t maximum);
  Bucket(const char* backing_file, size_t maximum);
  Bucket(Bucket&); 
public:
 ~Bucket();
public:
  operator bool() const {return _base;}  
public:  
  Bucket& operator=(Bucket&);
public:
  size_t size() const {return _size;}
public:  
  const char* content(size_t offset=0) const {return (const char*)(_base + offset);}
public:
  void* buffer(size_t offset, size_t length);
  void  flush( size_t offset, size_t length);
  void  flush();
private: 
  char*  _base;
  size_t _size;
  size_t _mapped;
  int    _fd;
};

}

#endif

