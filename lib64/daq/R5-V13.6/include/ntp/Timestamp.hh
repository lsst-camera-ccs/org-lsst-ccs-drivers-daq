#ifndef NTP_TIMESTAMP
#define NTP_TIMESTAMP

#include <stdint.h>
#include <time.h>

namespace NTP {

class Timestamp 
{
public:
  enum Epoch {NTP=0, UNIX=1, RTEMS=2};
public:
  Timestamp(uint64_t t=0);
public:
  Timestamp(uint32_t integer, uint32_t fraction);
  Timestamp(const timespec&, Epoch epoch=NTP);
public:
  Timestamp(const Timestamp& ts) : _timestamp(ts._timestamp) {};
public:
  const Timestamp& operator=(const Timestamp& ts) {_timestamp = ts._timestamp; return *this;}
public:
  bool operator<( const Timestamp&) const;
  bool operator<=(const Timestamp&) const;
  bool operator>( const Timestamp&) const;
  bool operator>=(const Timestamp&) const;
  bool operator==(const Timestamp&) const;
public:
  operator timespec() const;
  operator uint64_t() const;
public:
  timespec to_unix() const;
  timespec to_rtems() const;
public:
  const Timestamp  operator+ (int64_t);
  const Timestamp& operator+=(int64_t);
public:
  const Timestamp  operator+ (const Timestamp&);
  const Timestamp& operator+=(const Timestamp&);
  const Timestamp  operator- (const Timestamp&);
  const Timestamp& operator-=(const Timestamp&);
public:
  const Timestamp  operator/ (uint64_t);
  const Timestamp& operator/=(uint64_t);
public:
  const Timestamp  operator* (const Timestamp&) = delete;
  const Timestamp& operator*=(const Timestamp&) = delete;
public:
  uint32_t integer() const;
  uint32_t fraction() const;
  uint32_t nsecs() const;
public:
  const char* decode() const;
  const char* decode(char* buffer) const;
private:
  uint64_t _timestamp;
};
}
#endif
