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
  
#ifndef RTEMS_TASK
#define RTEMS_TASK

#include <rtems.h>

//RTEMS_INTERRUPT_LEVEL(0)
//RTEMS_INTERRUPT_LEVEL(n)

namespace RTEMS {

class Task {
public:
  enum Stack      : size_t          {MINIMUM = RTEMS_MINIMUM_STACK_SIZE, CONFIGURED = RTEMS_CONFIGURED_MINIMUM_STACK_SIZE};
  enum Modes      : rtems_mode      {PREEMPT = RTEMS_PREEMPT, NO_PREEMPT = RTEMS_NO_PREEMPT, TIMESLICE= RTEMS_TIMESLICE, NO_TIMESLICE= RTEMS_NO_TIMESLICE, ASR = RTEMS_ASR, NO_ASR = RTEMS_NO_ASR, DEFAULT_MODES = RTEMS_DEFAULT_MODES}; 
  enum Attributes : rtems_attribute {FLOATING_POINT = RTEMS_FLOATING_POINT, NO_FLOATING_POINT = RTEMS_NO_FLOATING_POINT, LOCAL = RTEMS_LOCAL, GLOBAL = RTEMS_GLOBAL, DEFAULT_ATTRIBUTES = RTEMS_DEFAULT_ATTRIBUTES};
public:
  static rtems_name name(char C0, char C1, char C2, char C3) {return rtems_build_name(C0, C1, C2, C3);}
public: // Creates a task...
  Task(rtems_name, rtems_task_priority, size_t stack_size, Modes);
  Task(rtems_name, rtems_task_priority, size_t stack_size, Modes=DEFAULT_MODES, Attributes=DEFAULT_ATTRIBUTES);
  Task(rtems_name, rtems_task_priority, size_t stack_size, Attributes);
public: // Lookup pre-existing task...
  Task(rtems_name=RTEMS_SELF, uint32_t node=RTEMS_SEARCH_ALL_NODES); 
public:
  Task(const Task& clone) : _task(clone._task) {}
public:
  virtual ~Task();
public:
  virtual void run() = 0; 
public:
  bool startup();
  bool suspend();
  bool resume();
private:
  rtems_id _task;  
 };
 
}

#endif
