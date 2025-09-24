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
 
#ifndef DSI_PLATFORM
#define DSI_PLATFORM

namespace DSI {

class __attribute__((__packed__)) Platform {
public: 
  Platform();
  Platform(const char* value);
  Platform(const Platform&);
public:
 ~Platform() {} 
public:
  Platform& operator=(const Platform& clone);
  Platform& operator=(const char*);
public:
  const char* name() const {return _platform;}
private:
  char _platform[32];
};

}

#endif
