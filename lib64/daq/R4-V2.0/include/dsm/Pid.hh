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
 
#ifndef DSM_PID
#define DSM_PID

namespace DSM {

class Pid {
public:
 static unsigned lookup(const char* interface); // Set...
 static unsigned lookup();                      // Get...
public:
  Pid()           = delete;
  Pid(const Pid&) = delete;
public:  
 ~Pid() {}
};

}

#endif

