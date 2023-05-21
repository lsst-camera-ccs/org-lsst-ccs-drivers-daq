
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

#ifndef OSA_PATH
#define OSA_PATH

namespace OSA {

class Path {
public: 
  enum {MAX=511+1}; // Inclusive of NULL termination... 
public: 
  Path()                                     : _tail(name)                             {poison();} 
  Path(const char* path)                     : _tail(_copy(path, name))                {} 
  Path(const char* base, const char* offset) : _tail(_copy(offset, _copy(base, name))) {} 
  Path(const Path& clone)                    : _tail(_copy(clone.name, name))          {}                
public:
 ~Path() {} 
public:  
  Path& operator=(const Path& clone) {_tail = _copy(clone.name, name); return *this;}
  Path& operator=(const char* clone) {_tail = _copy(clone, name);      return *this;}
public:  
  operator bool() const {return name[0];}
public:
  const char* append(const char* path) {return (const char*)_append(path);}
  const char* append()                 {*_tail = 0; return name;}
public:  
  void poison() {name[0] = 0;}      
public:
  char name[MAX];
private:
  char* _append(const char* path);
  char* _copy(  const char* path, char* dst);   
  char* _clone( const char* path);   
private:
  char* _tail; 
 };

}

#endif

