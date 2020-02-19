
/*
**  Package:
**	
**
**  Abstract:
**  
** Beware: This is and must be an exact duplicate of the class by the same name found in INIT.
    
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
 
#ifndef DSM_SOURCE
#define DSM_SOURCE

#include "osa/File.hh"

namespace DSM {

class Source {
public: 
  Source() {_source[0] = 0;}
  Source(const Source&);                    
  Source(const char* source);                  
 ~Source() {} 
public:
  Source& operator=(const char*);
  Source& operator=(const Source&);
public:
  operator bool() const {return _source[0];}
public:
  const char* value() const {return _source;}
public:
  void read( OSA::File&, int index);
  void write(OSA::File&, int index);
private:
  char _source[8];  
 };

}

#endif

