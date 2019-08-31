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
 
#ifndef DSM_TASKS
#define DSM_TASKS

#include "dsm/Services.hh"

namespace DSM {

class Tasks {
public:
 enum {PROGRAM=0, LOCATION=1, DIRECTORY=2, INTERFACE=3, SID=4, PID=5, PARTITION=6, ARGC=7}; 
public:
  Tasks()             = delete; 
  Tasks(const Tasks&) = delete;
public:
  Tasks(unsigned sids);  
public:  
  virtual ~Tasks() {}
public:
  void start();
  void stop();
private:  
  virtual void _start(unsigned sid, const char* name) = 0;
  virtual void _stop( unsigned sid)                   = 0;
private:
  Services _services;
  unsigned _sids;
  };

}

#endif

