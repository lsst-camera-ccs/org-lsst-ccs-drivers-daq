
#ifndef GDS_CLIENT
#define GDS_CLIENT

#include "dsm/Client.hh"
#include "net/ipv4/Address.hh"
#include "dsi/Buffer.hh"

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/ClearParameters.hh"
#include "gds/Status.hh"
#include "gds/Series.hh"
#include "gds/LocationSet.hh"

namespace GDS {

class Client : public DSM::Client
{
public:
  enum {INFINITE_LOOP=0};
public:
  Client(const char* partition);
  Client(const char* partition, const char* interface);
public:
  Client(const Client&) = delete;
public:
  ~Client() {};
public:
  int wake  (const ClearParameters&, Status&);
  int sleep (Status&);
public:
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, Status&);
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, const char* comment, Status&);
  int start (const RoiCommon&, const RoiLocation*, unsigned nlocs, const char* comment, uint32_t loops, Status&);
public:
  int clear(const ClearParameters&, Status&);
  int clear(const ClearParameters&, uint32_t loops, Status&);
public:
  int stop  (Status&);
  int pause (Status&);
  int resume(Status&);
  int stop  (const char* comment, Status&);
  int pause (const char* comment, Status&);
  int resume(const char* comment, Status&);
public:
  int series(Status&, Series& series, Series& idle);
  int config(Status&, ClearParameters&, RoiCommon&, RoiLocation*, unsigned& nlocs);
public:
  /*
  ** GDS emulator only (gds_emulator -S; see gds/EmulatorPlay.hh): load
  ** one file per sensor, named relative to the emulator's data
  ** directory, for the next START. Valid in SLEEPING, IDLECLEARING,
  ** IDLEPAUSE or PAUSED; consumed by START, kept by STOP, discarded by
  ** SLEEP. Returns 0 if loaded; INVALID_PARAMETER, INVALID_TRANSITION or
  ** CONFIG_FAILED if the emulator rejected the load ('reason', at least
  ** EMU_REASON chars, says why); INVALID_COMMAND from a real GDS
  ** service, which rejects it (a service predating that rejection does
  ** not answer: COMMAND_TIMEOUT); < 0 on other communication failures.
  ** An Ack error from the service (e.g. NOSPACE) is returned as is.
  */
  enum {EMU_MAX_FILES = 32, EMU_REASON = 160};
  int emu_play(const char* const* files, unsigned nfiles, Status&, LocationSet& loaded, char* reason=0);
public:
  bool validate(const RoiCommon&, const RoiLocation*, unsigned nlocs);
private:
  IPV4::Address _get_system();
  IPV4::Datagram* _wait_deadline(const DSI::Header& request, uint64_t budget_ms);
private:
  IPV4::Address _system;
  DSI::Buffer   _buffer;
};

}

#endif
