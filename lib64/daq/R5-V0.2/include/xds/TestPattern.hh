
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
 
#ifndef XDS_TESTPATTERN
#define XDS_TESTPATTERN

#include <stdint.h>

namespace XDS {

class TestPattern {
public:
  TestPattern(uint32_t length, bool fill=false);
  TestPattern(uint32_t length, uint32_t* buffer, bool fill=false);
public:
 ~TestPattern() {if(_delete) delete[] _content;}
public:
  char*       buffer()                         {return (char*)_content;}
  const char* content(uint32_t offset=0) const {return (const char*)&_content[offset / sizeof(uint32_t)];}
public:
  void verify(unsigned length, unsigned offset=0) const;
private: 
  uint32_t* _content;
  bool      _delete;
 };
     
}

#endif

