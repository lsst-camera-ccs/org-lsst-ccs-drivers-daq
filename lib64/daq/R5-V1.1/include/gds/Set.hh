 
#ifndef GDS_SET
#define GDS_SET

#include <stdint.h>

namespace GDS {

class __attribute__((__packed__)) Set {
public:
  enum : uint8_t {SIZE=255, ALL=0};
  enum State {ANY};
public:
  Set();
  Set(State);
public:  
  Set(uint8_t index);
  Set(uint8_t base, unsigned span);
public:  
  Set(const Set&); 
  Set(const Set&, const Set&);    
public:
 ~Set() {} 
public:                               
  Set& operator =(const Set&);                
  Set& operator|=(const Set&);                     
  Set& operator&=(const Set&);
  Set& operator^=(const Set&);                                                                           
public:
  Set operator&(const Set& operand) {Set mask = operand; mask &= *this; return mask;}
  Set operator|(const Set& operand) {Set add  = operand; add  |= *this; return add;}
  Set operator^(const Set& operand) {Set diff = operand; diff ^= *this; return diff;}
  Set& operator~();
public:
  operator bool() const {return _set[7] | _set[6] | _set[5] | _set[4] | _set[3] | _set[2] | _set[1] | _set[0];} 
public:
  unsigned numof() const;                  
public:
  bool has(uint8_t index) const;
public:  
  bool had(uint8_t index);
public:  
  uint8_t remove();
public:
  void insert(uint8_t index);
  void clear(const Set&);
public:
  void empty(); 
public:
  void print(int offset=0) const;   
private:
  uint32_t _set[8];
 };

}

#endif

