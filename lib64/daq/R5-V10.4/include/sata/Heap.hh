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

#ifndef SATA_HEAP
#define SATA_HEAP
 
#include "memory/resources.h"

namespace SATA {

class Heap {
public:
 enum {MAX = 64}; // Max buffers allocatable from heap...
public:
  Heap(unsigned size, char* region, int entries=MAX);
public: 
  Heap()            = delete; 
  Heap(const Heap&) = delete;   
public:
 ~Heap() {mem_rsClose(&_heap);}
public:  
  bool empty()     const {return !mem_rsGet(_heap);}
  int  remaining() const; 
public:
  char* region() {return _region;}  
private:
  friend class Object; 
private:
  static void __deallocate(void* object) {Heap** heap = (Heap**)object - 1; (*heap)->_deallocate(heap);}    
private: 
  void* _allocate()              {Heap** object = (Heap**)mem_rsAlloc(_heap) + 1; return (void*)object;}
  void  _deallocate(Heap** heap) {mem_rsFree(_heap, (void*)heap);}    
private:
  void* _heap;
  char* _region;
 };
 
}	

#endif
