#ifndef NTP_CLIENT
#define NTP_CLIENT

#include <rtems.h>

#include "net/ipv4/Address.hh"

#include "ntp/Timestamp.hh"

namespace NTP {

class Client 
{
public:
  Client();
public:
  void start();
private:
  void _update();
private:
  int _init_sock();
private:
  IPV4::Address _server;
  int           _sock;
  uint8_t       _poll;
  rtems_id      _taskId;
};
}
#endif
