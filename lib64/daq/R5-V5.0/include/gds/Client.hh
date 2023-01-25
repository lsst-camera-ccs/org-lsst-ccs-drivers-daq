
#ifndef GDS_CLIENT
#define GDS_CLIENT

#include "dsm/Client.hh"
#include "net/ipv4/Address.hh"

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/Status.hh"
#include "gds/Series.hh"

namespace GDS {

class Client : public DSM::Client
{
public:
  Client(const char* partition);
  Client(const char* partition, const char* interface);
public:
  Client(const Client&) = delete;
public:
  ~Client() {};
public:
  int wake  (Status&);
  int sleep (Status&);
public:
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, Status&);
  int stop  (Status&);
  int pause (Status&);
  int resume(Status&);
public:
  int series(Status&, Series&);
  int config(Status&, Series&, RoiCommon&, RoiLocation*, unsigned& nlocs);
private:
  IPV4::Address _get_system();
private:
  IPV4::Address _system;
};

}

#endif
