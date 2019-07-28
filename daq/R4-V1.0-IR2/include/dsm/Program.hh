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
 
#ifndef DSM_PROGRAM
#define DSM_PROGRAM

#include <unistd.h>

namespace DSM {

class Program {
public:
  static bool interface(const char* name);
  static bool site(const char* name);
public:
  static const char* encode(unsigned, char buffer[]);
  static unsigned    decode(const char*);  
public:
  Program();
  Program(int argc, char* argv[]);
  Program(const Program&);
public:  
 ~Program() {} 
public:
  void push(const char* argument);
public:
  const char* name()               const {return _argv[0];}
  const char* lookup(int argument) const {return _argv[argument+1];}
public:
  void start(const char* program); 
  void restart(); 
public:
  pid_t startup(const char* program);
private:
  int   _argc; 
  char* _argv[15]; 
  char* _argp[1];
};

}

#endif

