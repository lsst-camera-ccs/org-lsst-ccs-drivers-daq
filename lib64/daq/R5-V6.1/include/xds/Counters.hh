
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
  Counter(const char* type, const char* units) : requests(0),              retries(0),             timouts(0),             _type(type),        _units(units)        {}   
  Counter(const Counter& clone)                : requests(clone.requests), retries(clone.retries), timouts(clone.timouts), _type(clone._type), _units(clone._units) {}                                               
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
  unsigned requests;
  unsigned retries;
  unsigned timouts;
private:  
  const char* _type;
  const char* _units;
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
  Counters& operator=( const Counters& clone) {write  = clone.write; read  = clone.read; remove  = clone.remove; return *this;}
  Counters& operator+=(const Counters& clone) {write += clone.write; read += clone.read; remove += clone.remove; return *this;}
public:
  void dump(int indent=0) const;
public: 
  Counter write;
  Counter read;
  Counter lookup;
  Counter remove;
  };

}

#endif

