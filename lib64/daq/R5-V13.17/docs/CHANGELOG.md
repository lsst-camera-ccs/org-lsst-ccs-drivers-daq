# Changelog

All notable changes to the DAQ SDK are documented in this file.
Organized by release tag. Each release highlights API changes relevant
to client software (CCS), followed by enhancements, bug fixes, and
new utilities.

## R5-V13.17

### API Changes

#### GDS::Client
- **New method:** `emu_play(files, nfiles, status, loaded, reason)` —
  emulator-only; loads one file per sensor into a server-mode
  `gds_emulator` for the next START. A real GDS service rejects it with
  `INVALID_COMMAND` (a service predating this release does not answer:
  `COMMAND_TIMEOUT`).
- **New header:** `gds/EmulatorPlay.hh` defines the PLAY request and
  response (function code `F24`, reserved as `Function::EMULATOR_PLAY`).

#### GDS System Service
- **Changed:** The GDS service now rejects `F24` (emulator PLAY)
  immediately with `INVALID_COMMAND` instead of not answering.

### Enhancements

- **GDS Emulator — real-time playback:** New `auto` rate reproduces the
  input file's own stamp intervals. Wire timestamps now follow a shared
  acquisition timeline (T0 at RESUME plus each stamp's input offset), so
  stamp N carries an identical timestamp and number on every sensor.
  In-file PAUSE/RESUME/STOP records are forwarded at their place on the
  timeline. Rate `0` is documented as stress-test only (not
  timing-faithful).
- **GDS Emulator — server mode (`-S`):** `gds_emulator` can act as the
  GDS control service for a partition, so unmodified `GDS::Client` users
  (CCS, the `gds` tool) command it through the full GDS state machine
  (WAKE/START/RESUME/PAUSE/STOP/CLEAR/SLEEP, series/config reports).
  Data is published only on commanded transitions; every RESUME sets T0;
  files rewind at end of file. Data is loaded per series with the
  emulator-only PLAY command (`-d` sets the data directory). Refuses to
  start if any GDS service already answers on the partition.
- **gds:** New `play <file> [file...]` command (wraps
  `GDS::Client::emu_play()`).
- **GDS Emulator:** Playback engine refactored into reusable classes
  (`Source`, `Player`, `Streamer`, `Controller`, `Timeline`,
  `Rendezvous`, `Loader`, `Service`).

### Bug Fixes

- **GDS Emulator:** PAUSE/RESUME/STOP stamp counts now report
  acquisitions (last stamp number + 1), matching real readout, instead
  of stamps sent.
- **GDS Emulator:** `StampFile` rewind restores the header START state
  and series metadata, fixing looped replay of files with a mid-file
  START.

See `GDS_EMULATOR.md` (in the `gds/` package) for usage details.

---

## R5-V13.16

### API Changes

#### DSID::Client
- **New method:** `setInterface()` — runtime override of the network
  interface name. Must be called early in `main()`, before any client
  instances are created. Eliminates hardcoded `"lsst-daq"` interface
  names across RMS, IMS, and EMU tools.

### Enhancements

- **REB:** `reb_info` gained board ID display, selective field flags
  (`--id`, `--build`, `--hash`, `--version`, `--clock`, `--all`), and
  verbose version-register decoding. Multi-sequencer field now shows
  `n/a` for WREBs (single-sensor boards where the concept doesn't apply).
- **Build:** Documentation (`GDS_EMULATOR.md`, `DAQ_TOOLS.md`,
  `CHANGELOG.md`) now exported to `$EXPORT_ROOT/docs/` on build.

### New Tools — GDS Emulator

A new toolset for replaying archived guider stamps and FITS files through
the live GDS multicast interface, so `GDS::Subscriber`/`Decoder` clients
can be developed without hardware:

- `gds_emulator` — replays `.stamp`, `.rstamp`, and `.fits` files as a
  stream publisher. Supports multi-sensor (threaded) playback, looping
  (`-l`), sub-ranges (`-s`/`-n`), emulator-controlled sequence numbers
  (`-q`), and RESUME comment override (`-c`).
- `gds_stampinfo` — reports stamp count, ROI geometry, cadence, and
  metadata for a stamp/FITS file.
- `gds_stamp_roundtrip` — validates encode/decode fidelity of stamp and
  FITS files, pixel-by-pixel.
- `dsid_standalone` — foreground DSID service for development; auto-creates
  a partition and refuses to start if a DSID service is already running.

See `GDS_EMULATOR.md` (in the `gds/` package) for usage details.

---

## R5-V13.15

### API Changes

#### DAQ::Location
- **New method:** `encodeReb()` returns REB-style location names
  (`R22/Reb0`, `R00/RebW`, `R00/RebG`). The existing `encode()`
  method is unchanged and continues to return numeric `bay/board` format.
- **New:** `Location(const char*)` parser now accepts both numeric (`22/0`)
  and REB (`R22/Reb0`) input formats.
- **Removed:** `LocationSetCli.hh` — location parsing consolidated into
  `LocationSet`.

#### DAQ::LocationSet
- **Changed:** `encode()` returns smart summaries for well-known sets
  (e.g., `ScienceSet` instead of listing all 63 locations; `All except R22/Reb1`
  for near-complete sets).

#### RMS::Client
- **Renamed:** `reset_link()` → `set_link(targets, enable)`.
  The new API supports both enabling and disabling PGP links.
- **Renamed:** Wire protocol function `RESETLINK` → `SETLINK`
  (opcode value unchanged: `F6`).

### Enhancements

- **IMS:** `Image::synopsis()` column order changed — Readout Time is now
  the first column, enabling `| sort` on `ims_list` output.
- **REB bitfile location metadata:** Bitfiles now carry a location trailer
  enabling load-time gating — firmware can only be loaded onto the REB it
  was built for (override with `-i`).
- **XDS::Stats:** New `title()` and `encode()` methods for compact tabular
  output of storage server statistics.
- **RMS/EMU/XDS:** Reboot and reset utilities now prompt for confirmation
  before executing, showing the number of affected targets.

### Bug Fixes

- **GDS:** Fixed `_get_system()` discovery timeout; added `POST_FAIL`
  error string.
- **CMS/EMU:** Fixed incorrect program names in USAGE strings
  (`cms_delete` said `cms_trigger`; `emu_reboot_rce` said `rms_reboot_rce`).
- **IMS:** Fixed path order in all help text, USAGE strings, and error
  messages — was `image/folder`, corrected to `folder/image` to match
  actual `Path` class parsing.

### New Utilities

- `reb_info` — read FPGA build info from REB status registers
- `reb_pgpstats` — PGP link statistics per REB
- `reb_imgstats` — image statistics per REB virtual channel
- `rms_set_link` — control PGP link state (down/up/reset with duration)
- `scs_stats` — SCS sequencer statistics
- `ims_list` — list folders and images in the IMS data store
- `ims_inspect` — inspect stored image metadata
- `ims_badpixel` — scan images for bad pixels with optional stripe dump
- `xds_config` — display XDS server configuration and consistency
- `xds_sample` — tabular XDS server statistics with optional reset
- `xds_list` — XDS storage capacity summary
- `xds_probe` — probe XDS service for active servers
- `xds_reboot_rce` — reboot Storage RCEs by DSI location
- `emu_probe` — probe EMU service for active locations

### Removed Utilities

- `rms_reset` — removed (duplicated by `rms_reset_reb`)

See [DAQ_TOOLS.md](DAQ_TOOLS.md) for usage details.

---

## R5-V13.14

### API Changes

#### DSI::Client / DSI::Socket / DSI::Stream
- **Changed:** Timeout parameters changed from 10ms ticks (`uint32_t`) to
  milliseconds (`uint64_t`). Old tick-based constructors are deprecated
  but still functional.

#### DAQ::Sensor
- **New method:** `Sensor::xor_mask()` centralizes the sensor XOR mask
  computation (previously duplicated in GDS).

### Enhancements

- **SATA:** New `ssd`, `errors`, and `transport` shell commands for drive
  diagnostics. Fixed `standby`/`idle` argument handling. Added `nowake`
  flag to suppress drive wakeup before I/O.
- **GDS:** New `version` command.
- **IMS:** Bad-pixel counting added to `ims_editor` and
  `ims_image_monitor`. Dump mode prints bad stripes with surrounding
  context.

### Bug Fixes

- **NET:** Fixed `uint32_t` overflow in socket timeout conversion that
  could produce incorrect timeout values for large tick counts.
- **SATA:** Fixed `Drive::probe()` missing `SAS_Wait(_session)` call.
- **IMS:** Fixed `Decoder` segfault on corrupt image data by validating
  source size before decoding.
- **GDS:** Fixed spurious command timeouts by adding per-command retry
  loop with deadline budget. `series`/`config` now use `LONG_TIMEOUT`.

### New Utilities

- `gds_probe` — consolidated GDS/GDR service probing

---

## R5-V13.13

### API Changes

#### DAQ::SyncCommands
- **Moved:** `SyncCommands` class moved from `dsm/` to `daq/` package.
  New RCE firmware disables synchronous commands by default; the SDK
  now explicitly enables them during partition startup.

#### RMS::Client
- **New method:** `reboot_rce()` — reboot RCEs by DAQ location through
  the RMS service.

#### EMU::Client
- **New method:** `reboot_rce()` — reboot RCEs by DAQ location through
  the EMU service.

### Enhancements

- **RMS/EMU:** Added ability to reboot RCEs by DAQ location
  (`emu_reboot_rce`, `rms_reboot_rce`).

---

## R5-V13.12

### Bug Fixes

- **GDS:** Fixed split ROI calculation of next segment/amp that could
  produce incorrect region-of-interest boundaries.
- **RMS:** Fixed `HarvestLists` crash when REBs are offline — the
  harvest list now handles missing locations gracefully.

---

## R5-V13.11

### API Changes

#### RMS::Client
- **New method:** `reset_link()` — bounce the PGP link on the RCE
  (subsequently renamed to `set_link()` in V13.15).

### Enhancements

- **RMS:** Added `HarvestLists` that orders `rms_read`/`rms_write`
  output by REB type (science, wavefront, guiding).

### Bug Fixes

- **GDS:** Fixed relay bug where a double `start` without an intervening
  `stop` failed to clear the subscriber list, causing stale subscribers
  to accumulate.
