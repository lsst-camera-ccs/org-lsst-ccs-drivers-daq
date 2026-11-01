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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef RTEMS_OS
#define RTEMS_OS

#include <rtems.h>

namespace RTEMS { 

class OS {
public:  
  OS() {}
public: 
  OS(const OS&) = delete;
public:
 ~OS() {}
public:
  void reboot(const char* service);
};

}

#endif
