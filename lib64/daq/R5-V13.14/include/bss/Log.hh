
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

#ifndef BSS_LOG
#define BSS_LOG

#include "bss/Region.hh"

namespace BSS {

class Transaction; // forward reference

class Log {
public:
  Log() = delete;  
  Log(const char* drive); 
public:                    
 ~Log(); 
public: 
  void* head(); 
  void* tail();
public:  
  void* flink(void* entry);
  void* blink(void* entry);
public:  
  bool eol(void* entry); 
public:  
  uint16_t type(void* entry);
public:
  void* allocate(uint16_t type);
  void  deallocate(void*);
public:
  void  commit(void*);
  void* remove(void*);
public:  
  void dump();
  void dumpFree();
private:
  void  _insert(Transaction*, int list); // Insert after previous
  void  _remove(Transaction*);           // Remove arbitrary element  
private:  
  Transaction* _lookup(int index);
  void         _reconstruct(Transaction*);
  void         _format();
private:
  Region       _region;
  Sector*      _sectors;
  Transaction* _pending;
  Transaction* _freelist;
 };

}

#endif

