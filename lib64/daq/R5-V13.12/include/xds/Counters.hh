
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
 
#ifndef XDS_COUNTERS
#define XDS_COUNTERS

#include <stdint.h>

namespace XDS {

/*
** ++
**
** Bucket counter independent of access...
**
** --
*/
  
class Counter {
public: 
  Counter(const char* type, const char* units);
  Counter(const Counter& clone);
public:
  Counter() = delete;
public:
 ~Counter() {}
public:  
  Counter& operator=( const Counter& clone) {requests =  clone.requests; retries  = clone.retries; timouts  = clone.timouts; return *this;}
  Counter& operator+=(const Counter& clone) {requests += clone.requests; retries += clone.retries; timouts += clone.timouts; return *this;}
public:
  void dump(int indent=0) const;
public:
  void clear() {requests=0; retries=0; timouts=0;}
public: 
  uint64_t requests;
  uint64_t retries;
  uint64_t timouts;
private:  
  char _type[32];
  char _units[32];
  };

/*
** ++
**
** Sum of all different Bucket counters...
**
** --
*/

class Counters {
public: 
  Counters();  
  Counters(const Counters& clone) : write(clone.write), read(clone.read), lookup(clone.lookup), remove(clone.remove) {}                                               
public:
 ~Counters() {}
public:  
 Counters& operator=( const Counters& clone);
 Counters& operator+=(const Counters& clone);
public:
  void dump(int indent=0) const;
public:
  void clear() {write.clear(); read.clear(); lookup.clear(); remove.clear();}
public: 
  Counter write;
  Counter read;
  Counter lookup;
  Counter remove;
  };

}

#endif

