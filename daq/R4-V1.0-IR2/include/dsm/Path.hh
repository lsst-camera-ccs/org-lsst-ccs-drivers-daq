
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
 
#ifndef DSM_PATH
#define DSM_PATH

#include "osa/File.hh"

namespace DSM {

class Path {
public: 
  Path()                 {_path[0] = 'A';           _path[1] = 0;}
  Path(const Path& path) {_path[0] = path._path[0]; _path[1] = 0;}                    
  Path(const char* path) {_path[0] = path[0];       _path[1] = 0;}                   
 ~Path() {} 
public:
  Path& operator=(const char* clone) {_path[0] = clone[0];       _path[1] = 0; return *this;}
  Path& operator=(const Path& clone) {_path[0] = clone._path[0]; _path[1] = 0; return *this;} 
public:
  operator bool() const {return _path[0];}
public:
  bool A() const {return _path[0] == 'A';}
  bool B() const {return _path[0] == 'B';}
public:
  const char* value() const {return _path;}
public:
  void read( OSA::File&, int index);
  void write(OSA::File&, int index);
private:
  char _path[4];  
 };

}

#endif

