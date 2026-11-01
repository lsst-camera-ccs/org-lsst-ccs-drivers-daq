#ifndef GDS_EMULATOR_PLAY
#define GDS_EMULATOR_PLAY

#include <stdint.h>
#include <string.h>

#include "dsi/Header.hh"
#include "gds/Status.hh"
#include "gds/LocationSet.hh"

namespace GDS { namespace Emulator {

/*
** ++
**
** PLAY: emulator-only request (not part of the real GDS protocol) that
** loads the data the server-mode emulator will play for the next
** series. Sent to the same service address as the GDS commands, using
** function code F24, reserved as Function::EMULATOR_PLAY. The real GDS
** service rejects it with INVALID_COMMAND (System::Service::M24).
** Sent with GDS::Client::emu_play().
**
** Request payload: a sequence of NUL-terminated file names, relative to
** the server's data directory (gds_emulator -d), one per sensor. Sensor
** identity comes from each file's own metadata.
**
** Response: GDS Status (state in/out unchanged by PLAY), the set of
** sensors loaded, and a reason string on failure.
**
** --
*/

enum : uint8_t  {PLAY_VERSION = 1};
enum : unsigned {PLAY_MAX_FILES = 32,     // one file per sensor
                 PLAY_REASON    = 160};   // reason string, including NUL
static const DSI::Header::Function PLAY = DSI::Header::F24;   // == GDS::Function::EMULATOR_PLAY

class __attribute__((__packed__)) PlayRequest : public DSI::Header {
public:
  PlayRequest(unsigned nfiles) :
    DSI::Header(PLAY, sizeof(*this), PLAY_VERSION),
    _nfiles((uint16_t)nfiles) {memset(_pad, 0, sizeof(_pad));}
public:
  PlayRequest(const PlayRequest&) = delete;
public:
  uint8_t  version() const {return user();}
  unsigned nfiles()  const {return _nfiles;}
private:
  uint16_t _nfiles;
  uint8_t  _pad[6];
};

static_assert(!(sizeof(PlayRequest) & 7), "PlayRequest is not an even number of quadwords");

/*
** Reply status (Status::status()) is a GDS::Exception id:
**   SUCCESS            loaded
**   INVALID_PARAMETER  malformed request (version, file count, names)
**   INVALID_TRANSITION not SLEEPING, IDLECLEARING, IDLEPAUSE or PAUSED
**   CONFIG_FAILED      a file could not be loaded; see reason()
*/

class __attribute__((__packed__)) PlayResponse : public DSI::Header {
public:
  PlayResponse(const DSI::Header& request, const Status& status, const LocationSet& loaded, const char* reason) :
    DSI::Header(request, sizeof(*this), 0),
    _status(status),
    _loaded(loaded)
  {
    memset(_reason, 0, sizeof(_reason));
    if(reason) strncpy(_reason, reason, sizeof(_reason) - 1);
  }
public:
  PlayResponse(const PlayResponse&) = delete;
public:
  const Status&      status() const {return _status;}
  const LocationSet& loaded() const {return _loaded;}
  const char*        reason() const {return _reason;}
private:
  Status      _status;
  LocationSet _loaded;
  char        _reason[PLAY_REASON];
};

static_assert(!(sizeof(PlayResponse) & 7), "PlayResponse is not an even number of quadwords");
static_assert(sizeof(PlayResponse) <= 255, "PlayResponse must fit DSI::Header length (uint8_t)");

}}

#endif
