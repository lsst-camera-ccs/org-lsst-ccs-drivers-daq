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
 
#ifndef DSM_ARGV
#define DSM_ARGV

namespace DSM {

class Argv {
public:
  static char*    encode(unsigned pid, char buffer[]);
  static unsigned decode(const char pid[]);
public:
  Argv(char* const argv[]);
  Argv(const char* argv[]);
  Argv(const char* site, const char* pid, const char* location, const char* interface);
public:  
 ~Argv() {} 
public:
  const char*    site()      const {return _argv[1];}
  unsigned       pid()       const {return decode(_argv[2]);}
  const char*    location()  const {return _argv[3];}
  const char*    interface() const {return _argv[4];}
protected:
  enum {ARGC=5};
protected:  
  const char* _argv[ARGC+1];
};

}

#endif

