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
 
#ifndef DSID_TASK
#define DSID_TASK

namespace DSID {

class Task {
public:
  static bool site(const char* name);
  static bool interface(const char* name);
public:
  Task(char* const argv[]);
  Task(const char* service, char* program, char* site, char* interface, char* location);
public:
  Task()            = delete; 
  Task(const Task&) = delete;
public:  
 ~Task() {}
public:
  char* program()   const {return _argv[0];}
  char* release()   const {return _argv[1];}
  char* site()      const {return _argv[2];}
  char* interface() const {return _argv[3];}
  char* location()  const {return _argv[4];}
public:  
 void start();
 void restart();
public:
  void dump(int argc) const;
private:
  void _execute();
private:
  enum {ARGC=5};
private:
  char* _argv[ARGC+1];
  };

}

#endif

