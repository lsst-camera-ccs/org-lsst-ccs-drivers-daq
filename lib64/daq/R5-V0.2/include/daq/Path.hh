
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
 
#ifndef DAQ_PATH
#define DAQ_PATH

#include <stdint.h>

namespace DAQ {

class __attribute__((__packed__)) Path  {
public: 
  enum LANE : uint8_t {A=0, B=1, C=2, EMULATOR=3, DISABLE=0xff};
public:  
  Path()                  : _path(DISABLE)       {} 
  Path(const Path& clone) : _path(clone._path)   {}
  Path(LANE        lane)  : _path((uint8_t)lane) {}
  Path(char        path);
  Path(const char* path);
public:
 ~Path() {}
public:  
  Path& operator=(const Path& clone) {_path = clone._path;    return *this;} 
  Path& operator=(LANE        clone) {_path = (uint8_t)clone; return *this;}
  Path& operator=(char        clone);
  Path& operator=(const char* clone);
public:
  operator bool() const {return _path != DISABLE;} 
public:
  LANE lane() const {return (LANE)_path;}
public:
  bool a() const {return _path == LANE::A;}
  bool b() const {return _path == LANE::B;}
  bool c() const {return _path == LANE::C;}
  bool e() const {return _path == LANE::EMULATOR;}
public:
  void reset() {_path = DISABLE;} 
public:
  const char* encode(char* buffer) const;         
private:
  uint8_t _path;
 };

}

#endif

