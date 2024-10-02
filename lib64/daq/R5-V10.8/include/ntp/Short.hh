#ifndef NTP_SHORT
#define NTP_SHORT

#include <stdint.h>
#include <time.h>

namespace NTP {

class Short 
{
public:
  enum Epoch {NTP=0, UNIX=1, RTEMS=2};
public:
  Short(uint32_t t=0);
public:
  Short(uint16_t integer, uint16_t fraction);
public:
  Short(const Short& ts) : _short(ts._short) {};
public:
  const Short& operator=(const Short& ts) {_short = ts._short; return *this;}
public:
  bool operator<( const Short&) const;
  bool operator<=(const Short&) const;
  bool operator>( const Short&) const;
  bool operator>=(const Short&) const;
  bool operator==(const Short&) const;
public:
  operator uint32_t() const;
//public:
//  const Short  operator+ (const Short&);
//  const Short& operator+=(const Short&);
//  const Short  operator- (const Short&);
//  const Short& operator-=(const Short&);
//public:
//  const Short  operator/ (uint32_t);
//  const Short& operator/=(uint32_t);
//public:
//  const Short  operator* (const Short&) = delete;
//  const Short& operator*=(const Short&) = delete;
public:
  uint16_t  integer() const;
  uint16_t fraction() const;
  uint32_t    usecs() const;
public:
  const char* decode() const;
  const char* decode(char* buffer) const;
private:
  uint32_t _short;
};
}
#endif
