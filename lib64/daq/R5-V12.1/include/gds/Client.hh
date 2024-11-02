
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
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, const char* comment, Status&);
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, const char* comment, uint32_t loops, Status&);
public:
  int clear(const LocationSet&, uint32_t delay, uint32_t loops, Status&);
  int clear(const LocationSet&, uint32_t delay, uint32_t loops, const char* comment, Status&);
public:
  int stop  (Status&);
  int pause (Status&);
  int resume(Status&);
  int stop  (const char* comment, Status&);
  int pause (const char* comment, Status&);
  int resume(const char* comment, Status&);
public:
  int series(Status&, Series&);
  int config(Status&, Series&, RoiCommon&, RoiLocation*, unsigned& nlocs);
public:
  void validate(const RoiCommon&, const RoiLocation*, unsigned nlocs);
private:
  IPV4::Address _get_system();
private:
  IPV4::Address _system;
};

}

#endif
