#ifndef NTP_HEADER
#define NTP_HEADER

#include <stdint.h>
#include "ntp/Short.hh"
#include "ntp/Timestamp.hh"

namespace NTP {

class Header 
{
public:
  Header() {};
public:
  void dump() const;
public:
  uint8_t   li_vn_mode;
  uint8_t   stratum;
  int8_t    poll_interval;
  int8_t    precision;
  Short     root_delay;
  Short     root_dispersion;
  char      reference_identifier[4];
  Timestamp reference_timestamp;
  Timestamp originate_timestamp;
  Timestamp receive_timestamp;
  Timestamp transmit_timestamp;
};
}

#endif
