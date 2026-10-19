# GDS Emulator — User Manual

## Overview

The GDS Emulator reads archived guider stamp files and replays them on the
DAQ multicast streaming interface at a configurable rate. Existing
`GDS::Subscriber` and `GDS::Decoder` clients receive the emulated data
through the same code path as live hardware readout — they cannot
distinguish emulated stamps from real ones.

The primary use case is developing and testing new client classes that
inherit from `GDS::Subscriber` or `GDS::Decoder` without requiring live
hardware.

The emulator runs in one of two modes:

- **One-shot mode** (default): play the given files end to end, then
  exit. See [One-Shot Mode](#gds_emulator-one-shot-mode).
- **Server mode** (`-S`): act as the GDS control service for a
  partition. Clients command it exactly as they command the real GDS
  (`wake`, `start`, `resume`, `pause`, `stop`, ...), and it publishes
  data accordingly. An emulator-only PLAY command chooses the files.
  See [Server Mode](#gds_emulator-server-mode).

### Components

| Tool | Purpose |
|------|---------|
| `gds_emulator` | Reads stamp files and publishes on the GDS stream (one-shot or server mode) |
| `gds_stampinfo` | Prints summary statistics for stamp files |
| `gds_stamp_roundtrip` | Validates encode/decode round-trip fidelity |

All three tools are **x86-only** (not built for alinux or RTEMS).
Server mode is commanded with the existing `gds` tool (x86 and alinux),
which also provides the emulator-only `play` command
(`GDS::Client::emu_play()`).

### Supported File Formats

| Format | Extension | Source | Pixel Data |
|--------|-----------|--------|------------|
| Decoded | `.stamp` | `gds_archiver` | int32 pixels |
| Raw | `.rstamp` | `gds_archiver -r` | 18-bit packed |
| FITS | `.fits` | CCS / DM | RICE-compressed int32 |

File format is detected automatically from the extension.

---

## Quick Start

### 1. Inspect a file

```
gds_stampinfo MC_O_20260501_000824_R00_SG0_guider.fits
```

Output:

```
File: MC_O_20260501_000824_R00_SG0_guider.fits
  Format:      fits
  Sensor:      00/1/0: segment 07, startrow  800, startcol  56
  Vendor:      ITL
  ROI:         400 rows x 400 cols (integration 200 ms)
  Pixels:      160000 decoded, 160000 raw
  Stamp size:  640000 bytes (file), 5760000 bytes (raw)
  Sequence:    44220
  Stamp count: 50 (from header)
  Stamps:      50
  Duration:    29.239 s
  Avg cadence: 596.704 ms (1.7 Hz)
```

The **Avg cadence** value is the rate you should use with the emulator
to match real-time playback.

### 2. Ensure a DSID partition exists

The emulator requires a DAQ partition. If you have an existing DAQ
deployment, partitions are managed with `dsid_editor`. For standalone
development, start `dsid_standalone` in a separate terminal:

```
dsid_standalone -p my-partition
```

See [Standalone Setup](#standalone-setup) below for interface
configuration and details.

### 3. Start a subscriber

In one terminal, start whatever subscriber you want to test. For example,
the archiver:

```
gds_archiver <partition> -d /tmp/output
```

### 4. Run the emulator (one-shot mode)

In another terminal:

```
gds_emulator <partition> 597 MC_O_20260501_000824_R00_SG0_guider.fits
```

The emulator will:
1. Open the file and read metadata
2. Send a START message on the GDS multicast stream
3. Wait for subscribers to respond (ACK handshake)
4. Send a RESUME, which begins data flow
5. Publish stamps at the specified rate (597 ms between stamps)
6. Send a STOP message when the file is exhausted

To have clients command the emulator instead, see
[Server Mode](#gds_emulator-server-mode).

---

## gds_emulator: One-Shot Mode

```
usage: gds_emulator [options] <partition> <rate_ms> <file(s)...>
```

Plays the given files end to end on the GDS stream, then exits. No
control commands are involved: the emulator itself sends START, RESUME,
the stamps (and any PAUSE/RESUME/STOP records in archive files), and
STOP.

### Arguments

| Argument | Description |
|----------|-------------|
| `partition` | DAQ partition name |
| `rate_ms` | Milliseconds between stamps. Use `auto` to reproduce the input file's own stamp intervals, or 0 for maximum speed. |
| `file(s)` | One or more `.stamp`, `.rstamp`, or `.fits` files |

### Options

| Option | Default | Description |
|--------|---------|-------------|
| `-t <ms>` | 500 | ACK timeout for subscriber discovery |
| `-s <n>` | 0 | Skip first N stamps before publishing |
| `-n <n>` | 0 (all) | Publish at most N stamps per sensor |
| `-l <n>` | 1 | Loop N times (0 = infinite) |
| `-q <n>` | 1 | Starting sequence number (increments per START) |
| `-c <txt>` | (none) | Override RESUME comment (e.g. image name), including the RESUME that begins each series |
| `-v` | off | Verbose: print per-stamp status |
| `-h` | | Print usage and exit |

### Rate Selection

- Use `auto` to reproduce the input file's own stamp intervals exactly
  (see below) — recommended for realistic playback
- Use a fixed value (e.g. the cadence reported by `gds_stampinfo`) for a
  uniform interval
- Use `0` for maximum throughput (stamps sent as fast as possible) —
  for stress testing only; timestamps are not meaningful in real time
- The rate is the interval *between* stamp timestamps, not a delay added
  after each send

### Timestamps and Timing

Stamp timestamps represent **acquisition time**, as in the real system:
all sensors' stamp N was acquired at the same moment, so stamp N carries
an identical timestamp on every sensor, and the intervals between
timestamps reflect the acquisition cadence.

Each series is emitted as `START → RESUME → stamps → STOP`. The RESUME
begins data flow; its time, plus the file's integration time (`ROIMILLI`
for FITS), is the series base time **T0**:

| Mode | Stamp N timestamp |
|------|-------------------|
| `auto` | T0 + (input timestamp of stamp N − input timestamp of the first played stamp) |
| fixed `rate_ms` | T0 + slot × `rate_ms` (see below) |
| `0` | same as `auto`, but nothing waits (see below) |

- **The same file played twice** produces different absolute timestamps
  (a new T0 each time) but identical intervals.
- **Each record is sent at its timestamp**, used as an absolute deadline
  (`CLOCK_TAI`, the clock `OSA::TimeStamp` uses), so errors never
  accumulate. Only the actual *arrival* time at a subscriber varies,
  as it does in the real system from pipeline and network latency.
- **All sensor threads start together.** They complete their START/ACK
  handshakes, then meet; one RESUME time is taken for all of them.
- **Stamp numbers** come from the input file's stamp index (FITS
  `DAQSTAMP`), relative to the first played stamp. They start at 0, as on
  real hardware, and gaps from dropped stamps in the input are preserved.
  If an input index goes backwards, the emulator warns and numbers
  sequentially for the rest of the series.
- **All files must be from the same acquisition.** Each file's first
  played stamp (after `-s`) must have exactly the same input timestamp,
  and all files must have the same integration time; otherwise the
  emulator refuses to play.
- **Records that represent commands** — the STOP the emulator generates at
  end of file or at the `-n` limit, and START — carry the wall-clock time
  at which they are sent.

### Input-Delta Timing (`auto`)

```
gds_emulator <partition> auto guider.fits
```

`auto` reproduces the input file's stamp intervals exactly, including
any natural variation and any gaps (for example, a dropped stamp leaves
a correspondingly longer interval, and the stamp number skips).

**In-file PAUSE/RESUME/STOP records** (archive formats only; FITS has
none) are forwarded at their own place on the same timeline: T0 +
(their input timestamp − the first played stamp's input timestamp). A
real pause in the source therefore reproduces as a faithful gap, with
the PAUSE and RESUME records at the right times. An in-file RESUME does
not change T0.

**Skipped stamps (`-s`)** are treated as absent: timing and numbering
start from the first played stamp. Any PAUSE/RESUME records before it
are collapsed into the single RESUME that begins the series (its
comment is `-c` if given, otherwise the last skipped RESUME's comment).

**Fixed rate:** every forwarded record occupies one `rate_ms` slot, so
STAMP → PAUSE → RESUME → STAMP spans three intervals. A dropped stamp
in the input also leaves an empty slot.

**Rate 0:** records are sent immediately, but their timestamps still
follow the input timeline. The timestamps are therefore ahead of real
time; the generated STOP (wall-clock) can carry an earlier time than
the last stamps.
Rate 0 is intended for stress testing, not timing fidelity.

### Multi-Sensor Operation

Pass multiple files to emulate multiple sensors simultaneously:

```
gds_emulator <partition> 597 \
  MC_O_20260501_000824_R00_SG0_guider.fits \
  MC_O_20260501_000824_R00_SG1_guider.fits \
  MC_O_20260501_000824_R44_SG0_guider.fits
```

Each file is published from its own thread with its own network socket,
matching the real system where each sensor has an independent readout task.
All threads share one RESUME time and T0, so stamp timestamps are in
lockstep across sensors; only send/arrival times differ.

**Constraints:**
- Maximum 32 sensors
- File order does not matter; sensor identity comes from file metadata
- All files must be from the same acquisition (see
  [Timestamps and Timing](#timestamps-and-timing))

### Looping

Loop playback for sustained testing:

```
# Loop 10 times
gds_emulator -l 10 <partition> 597 guider.fits

# Loop forever (Ctrl-C to stop)
gds_emulator -l 0 <partition> 597 guider.fits
```

Each loop sends a full START → RESUME → stamps → STOP series, with its
own T0. There is a 100 ms pause between loops to allow subscribers to
process the STOP/START transition. The sequence number increments with
each START (including loop restarts), starting from 1 by default or the
value given with `-q`. Stamp numbers restart at 0 in each series.

### Sub-Range Playback

Skip stamps and limit count for targeted testing:

```
# Skip first 10 stamps, publish next 5
gds_emulator -s 10 -n 5 <partition> 200 guider.fits
```

### FITS Metadata

FITS files contain all metadata needed for emulation:
- Primary HDU: ROI, location, and DAQ metadata (no pixel data)
- Image extensions: one per stamp, RICE-compressed

The emulator reads metadata from FITS keywords (`ROIROWS`, `ROICOLS`,
`RAFTBAY`, `CCDSLOT`, `GDSSEQ`, `ROICCDTY`, etc.) and synthesizes the
`SeriesMetadata` and `StateMetadata` that subscribers expect. Per-stamp
`STMPTMJD` values set the intervals between stamp timestamps, and
`DAQSTAMP` sets the stamp numbers (see
[Timestamps and Timing](#timestamps-and-timing)).

FITS files do not contain PAUSE/RESUME events. The emulator synthesizes
START, one RESUME (comment from `-c`), and STOP.

Archive formats (`.stamp`, `.rstamp`) from `gds_archiver` are also
supported — see [Supported File Formats](#supported-file-formats) above.

### Subscriber Discovery

When the emulator sends START, it waits up to `-t` milliseconds for
subscriber ACK responses. If no subscribers respond, the sensor thread
exits with a message. Increase the timeout if subscribers are slow to
start:

```
gds_emulator -t 2000 <partition> 597 stamps.stamp
```

### Error Conditions

| Error | Cause |
|-------|-------|
| `partition does not exist` | Wrong partition name |
| `cannot open <file>` | File not found or unreadable |
| `No subscribers found` | No subscriber responded within ACK timeout |
| `Too many sensors (max 32)` | More than 32 files on command line |
| `Files are not from the same acquisition` | First played stamp timestamps or integration times differ between files |
| `non-monotonic stamp index` (warning) | Input stamp index went backwards; numbering falls back to sequential |

---

## gds_emulator: Server Mode

```
usage: gds_emulator -S [-d <dir>] [-q <n>] [-t <ms>] [-v] <partition>
```

In server mode the emulator is the GDS control service for
`<partition>`. It binds the same DSI service address as the real GDS
service, so an unmodified `GDS::Client`, including the `gds` command-line
tool, CCS, or your own client, discovers and commands it as it would
the real system. Data is published on the GDS stream exactly as in
one-shot mode, with the same timestamp rules (see
[Timestamps and Timing](#timestamps-and-timing) under one-shot mode),
but only when commanded.

| Option | Default | Description |
|--------|---------|-------------|
| `-d <dir>` | `.` | Data directory. File names given to PLAY are relative to it. |
| `-q <n>` | 1 | Sequence number of the first START (increments at each START) |
| `-t <ms>` | 500 | ACK timeout for subscriber discovery at each START |
| `-v` | off | Verbose: print commands and per-stamp status |

Server mode takes no files on the command line, and has no rate
argument: data is always played with the input file's own stamp
intervals (`auto`).

**Only one GDS service per partition.** At startup the emulator probes
the partition; if any GDS service (real, or another emulator) answers,
it refuses to start. Never run server mode on a partition used by real
hardware.

### Workflow

```
# Terminal 1: server, data directory with the guider files
gds_emulator -S -d /data/MC_O_20260529_000600 <partition>

# Terminal 2: a subscriber (archiver, or your own client)
gds_archiver <partition> -d /tmp/output

# Terminal 3: commands -- the gds tool, or your client via GDS::Client
gds <partition> wake clear.json
gds <partition> play MC_O_20260529_000600_R00_SG0_guider.fits \
                     MC_O_20260529_000600_R00_SG1_guider.fits
gds <partition> start roi.json -c obs1
gds <partition> resume -c image1      # stamps start flowing
gds <partition> pause                 # stamps stop
gds <partition> stop -c done
gds <partition> sleep
```

The usual sequence is `WAKE → PLAY → START → RESUME → (stamps) → PAUSE →
STOP`. PLAY must be sent before **each** START (see [PLAY](#play)).

### Commands

All GDS commands are accepted and follow the real GDS state machine
(`SLEEPING`, `IDLECLEARING`, `IDLEPAUSE`, `CLEARING`, `PAUSED`, `RUNNING`,
`ERROR`), including its replies. A command that is not valid in the
current state is answered "Invalid transition requested" and the state
is unchanged.

| Command | Effect in the emulator |
|---------|------------------------|
| `wake` | SLEEPING → IDLECLEARING. The clear parameters are stored (reported by `config`). Nothing is published. |
| `sleep` | → SLEEPING. Discards any pending PLAY load. |
| `start` | → PAUSED. Begins a new series (sequence + 1) with the PLAY load: sends START and waits for subscriber ACKs on each participating sensor's stream. See [START](#start). |
| `resume` | PAUSED → RUNNING. Sends RESUME (with its comment) and starts stamps (see [Timing](#timing-in-server-mode)). In IDLEPAUSE: → IDLECLEARING. |
| `pause` | RUNNING → PAUSED. Sends PAUSE (with its comment); stamps stop. In IDLECLEARING: → IDLEPAUSE. |
| `stop` | → IDLECLEARING. Sends STOP (with its comment) and ends the series. |
| `clear` | IDLECLEARING → CLEARING. Nothing is published. A finite clear count (`-n`) ends by itself. |
| `series`, `config` | Report the current state, series (stamp count, sensors) and configuration. |
| `play` | Emulator only: load the data for the next START. See [PLAY](#play). |

Clearing is modelled as one clear per clear-parameter `delay`
(milliseconds, minimum 1) and counted in the `series` report. It has no
effect on the data stream.

### PLAY

PLAY is an emulator-only command (it is not part of the real GDS
protocol). It loads the data for the next START: one file per sensor,
named relative to the server's `-d` directory. From the command line it
is the `gds` tool's `play` command:

```
gds <partition> play <file> [file...] [-q]
```

From code, use `GDS::Client::emu_play()` (`gds/Client.hh`):

```cpp
#include "gds/Client.hh"

GDS::Client client(partition);           // e.g. argv[1]
const char* files[] = {"R00_SG0_guider.fits", "R00_SG1_guider.fits"};
GDS::Status      status;
GDS::LocationSet loaded;
char             reason[GDS::Client::EMU_REASON];
int error = client.emu_play(files, 2, status, loaded, reason);
// error: 0 = loaded; > 0 = rejected (see reason); < 0 = no reply
```

- **Valid** in SLEEPING, IDLECLEARING, IDLEPAUSE and PAUSED. Loading
  while PAUSED is how a client prepares the next series before a START
  from PAUSED. PLAY does not change the state.
- **Rejected** (state unchanged, previous load kept) if a file cannot be
  opened, two files are for the same sensor, a file contains more than
  one series, or the files are not from the same acquisition (their
  first stamps must have identical timestamps, and they must have the
  same integration time). The reply's reason string says which. The
  status code for a rejected load is "Failed to configure ROIs"; read
  the reason for the actual cause.
- **Lifetime of a load:** used up by the next START, so PLAY must be sent
  again before every START. STOP does not discard a pending load (a
  load made while PAUSED is kept for the next START). SLEEP discards it.
- Sensor identity comes from each file's own metadata.
- Sent to a real GDS service, PLAY is rejected at once ("Invalid
  command specified"). A GDS service from a release before this
  rejection does not answer, and the client times out after 5 s
  ("Command Timeout").

### START

START must name only sensors that were loaded by PLAY, and its ROI must
be valid and match the files:

| Condition | Reply |
|-----------|-------|
| No PLAY load | Success, but the state goes to **ERROR** (as for a readout failure on real hardware). Only `sleep` is accepted from ERROR. |
| ROI invalid (the same checks `GDS::Client::validate()` applies) | "Invalid parameter specified", state unchanged |
| A requested sensor was not loaded | "Failed to configure ROIs", state unchanged |
| ROI rows/columns differ from a file's | "Failed to configure ROIs", state unchanged |

The ROI's location (segment, start row/column) is not used: the data
comes from the files. Loaded sensors that START does not name are not
played. A START from PAUSED begins a new series; as on real hardware,
no STOP is sent for the previous series.

A finite loop count (`gds start ... -n <count>`, at most 255) stops the
series automatically after that many stamps (→ IDLECLEARING, STOP
sent), as on real hardware. A later client `stop` then gets "Invalid
transition requested".

### Timing in Server Mode

- **Every RESUME sets T0** = RESUME time + the files' integration time.
  The first stamp after a RESUME is sent at T0; later stamps follow the
  file's own intervals. A PAUSE/RESUME therefore shifts the timeline by
  the length of the pause. Stamp numbers continue across it.
- **PAUSE/RESUME/STOP records inside archive files are ignored.** Only
  commanded transitions are published; gaps in the file still appear in
  the stamp timing.
- **End of file: the file rewinds** and playback continues. The first
  stamp after the wrap is one "last interval" after the previous stamp,
  and stamp numbers continue.
- Stamp N carries an identical timestamp and number on every sensor.
- If a file cannot be read during a series, the emulator enters ERROR.

---

## gds_stampinfo

```
usage: gds_stampinfo [-v] <stamp, rstamp, or fits file(s)>
```

Reads one or more files and prints summary statistics. Accepts all three
formats (`.stamp`, `.rstamp`, `.fits`).

### Options

| Option | Description |
|--------|-------------|
| `-v` | Verbose: dump full StateMetadata and SeriesMetadata |
| `-h` | Print usage and exit |

### Output Fields

| Field | Description |
|-------|-------------|
| Format | File type detected from extension |
| Sensor | Location (bay/board/sensor) with segment and ROI offset |
| Vendor | ITL or E2V |
| ROI | Rows × cols and integration time |
| Overscan | Under/over column and over-row counts |
| Split ROI | Whether split-ROI mode was active |
| Pixels | Decoded and raw pixel counts per stamp |
| Stamp size | Bytes per stamp in file and in raw wire format |
| Sequence | GDS sequence number |
| Stamp count | (FITS only) Stamp count from header |
| Stamps | Actual stamps read from file |
| Pauses/Resumes | Count of PAUSE/RESUME records (archive format) |
| Stop | Whether a STOP record was present |
| Duration | Time span from first to last stamp |
| Avg cadence | Mean inter-stamp interval in ms and Hz |

For FITS files, the cadence is computed from the original DAQ timestamps
stored in each extension's `STMPTMJD` keyword.

### Multiple Files

Pass multiple files to inspect them all:

```
gds_stampinfo *.stamp *.fits
```

---

## gds_stamp_roundtrip

```
usage: gds_stamp_roundtrip [-v] <file(s)>
       gds_stamp_roundtrip -x <stamp file> <rstamp file>
```

Validates that the encode/decode pipeline is lossless.

### Default Mode (Round-Trip)

Reads a `.stamp` or `.fits` file, encodes each stamp to raw 18-bit packed
format, decodes back to int32, and compares pixel-by-pixel against the
original:

```
gds_stamp_roundtrip guider.fits
gds_stamp_roundtrip 44220_00_1_0.stamp
```

### Cross-Format Mode (-x)

Reads a `.stamp` and matching `.rstamp` file (from the same archiver run),
decodes each raw stamp, and compares against the decoded version:

```
gds_stamp_roundtrip -x 44220_00_1_0.stamp 44220_00_1_0.rstamp
```

### Options

| Option | Description |
|--------|-------------|
| `-v` | Print per-stamp PASS/FAIL |
| `-x` | Cross-format mode (requires two files) |
| `-h` | Print usage and exit |

---

## Architecture

### Data Flow

```
  File ──► Reader ──► Encoder ──► Publisher ──► Subscriber/Decoder
  (.stamp)  StampFile  int32 →     START/ACK     (existing clients,
  (.rstamp) StampFile  raw 18-bit  handshake,     unchanged)
  (.fits)   FitsFile   packed      unicast
                                   stamp delivery
```

### Key Classes

**`GDS::Emulator::StampFile`** — Reads `.stamp` and `.rstamp` archive
files written by `gds_archiver`. Handles the enriched archive format
including inline PAUSE, RESUME, STOP, and mid-file START records.

**`GDS::Emulator::FitsFile`** — Reads guider FITS files. Parses primary
header keywords into `SeriesMetadata`, reads RICE-compressed image
extensions via cfitsio, and provides per-stamp input timestamps from
`STMPTMJD` and stamp indices from `DAQSTAMP`.

**`GDS::Emulator::Encoder`** — Re-encodes decoded int32 pixel data into
the raw 18-bit packed format expected by the streaming protocol. The
encoding is lossless for science pixels (values < 2^18).

**`GDS::Emulator::Publisher`** — Manages the GDS stream protocol:
multicast START announcement, ACK-based subscriber discovery, unicast
stamp delivery with Transfer chunking for large payloads, and state
transitions (PAUSE/RESUME/STOP).

**`GDS::Emulator::Source`** — One sensor's input file (any format) with
its encoder and buffers; also the cross-file alignment check.

**`GDS::Emulator::Player`** — One-shot playback of one sensor.

**`GDS::Emulator::Service` / `Controller`** — Server mode: the DSI
control service (request dispatch) and the state machine, the
emulator's counterpart of `GDS::System::Service` / `Guider`.

**`GDS::Emulator::Streamer`** — Server mode: one sensor's data stream
for one series, driven by the Controller.

**`GDS::Emulator::Loader`** — Server mode: opens and checks the files
named by PLAY.

**`GDS::Emulator::Rendezvous`, `Timeline`** — Shared RESUME time across
sensor threads; timestamp, deadline and stamp-numbering helpers.

**`GDS::Client::emu_play()`** (public, `gds/Client.hh`) — Sends the
emulator-only PLAY command; wire types in `gds/EmulatorPlay.hh`.

### Threading Model

With multiple files, each sensor runs in its own thread with its own
`Publisher` instance and network socket. This matches the real system
where each GREB sensor has an independent readout task. The threads meet
before each series' RESUME so they share one T0. The main thread waits
for all sensor threads to complete.

With a single file, the sensor runs inline (no thread created).

In server mode, a controller thread processes commands in order (as the
real Guider does), and each participating sensor has a Streamer thread
for the duration of a series. The sensors' Streamers meet at every
RESUME, so they share its time and T0.

### Protocol Compatibility

The emulator publishes directly on the GDS stream multicast address,
bypassing both the readout service and the relay. Subscribers connect
through the standard `GDS::Subscriber` / `GDS::Decoder` path. The
wire format is identical to live hardware output.

---

## Examples

### Real-Time Playback

```bash
# Start archiver to capture output
gds_archiver <partition> -d /tmp/output &

# Play back with the original stamp intervals
gds_emulator <partition> auto MC_O_20260501_000824_R00_SG0_guider.fits
```

### High-Speed Multi-Sensor Replay

```bash
# Play 8 sensors from a FITS observation as fast as possible
gds_emulator <partition> 0 /data/MC_O_20260501_000824/*_guider.fits
```

### Continuous Loop for Stress Testing

```bash
# Loop forever, 3 stamps per loop, 100ms rate
gds_emulator -l 0 -n 3 <partition> 100 guider.fits
```

### Validate Encode/Decode Fidelity

```bash
# Round-trip test with FITS file
gds_stamp_roundtrip guider.fits

# Round-trip test with archive file
gds_stamp_roundtrip 44220_00_1_0.stamp

# Cross-format: compare decoded vs raw archives
gds_stamp_roundtrip -x 44220_00_1_0.stamp 44220_00_1_0.rstamp
```

---

## Standalone Setup

The emulator requires a DSID partition. In a full DAQ deployment the DSID
service is already running and partitions are managed with `dsid_editor`.
For standalone development without existing infrastructure, use
`dsid_standalone`.

### Prerequisites

The DSID service requires a network interface with multicast and broadcast
support. Any existing multicast-capable interface will work — use `-i` to
specify it:

```bash
dsid_standalone -i eth0 -p my-partition
```

If no suitable interface is available, create a dummy one (requires root,
one-time setup):

```bash
sudo ip link add gds-devel type dummy
sudo ip addr add 192.168.100.1/24 dev gds-devel
sudo ip link set gds-devel up multicast on
```

Then pass `-i gds-devel` to `dsid_standalone`.

### dsid_standalone

```
usage: dsid_standalone [-i interface] [-p partition] [-s site_dir]
```

| Option | Default | Description |
|--------|---------|-------------|
| `-i` | `lsst-daq` | Network interface |
| `-p` | `gds-emu` | Partition name to auto-create |
| `-s` | `/tmp/dsid-standalone` | Directory for partition state file |

`dsid_standalone` checks for an existing DSID service on the interface
before starting. If one is found, it prints instructions to create the
partition in the existing service and exits.

If no DSID service is found, it starts one in the foreground, creates the
requested partition, and waits for Ctrl-C.

### Standalone Workflow

```bash
# Terminal 1: Start DSID service on dummy interface
dsid_standalone -i gds-devel -p my-partition

# Terminal 2: Start a subscriber
gds_archiver my-partition -d /tmp/output

# Terminal 3: Run emulator
gds_emulator my-partition 597 /path/to/guider.fits

# When done, Ctrl-C in terminal 1 to stop the DSID service
```

---

## Creating Synthetic FITS Files

The emulator can replay FITS files created synthetically (e.g. for testing
with known pixel patterns). This section documents the required FITS
keywords and provides a Python example.

### FITS Structure

A valid guider FITS file has:

- **Primary HDU** (no image data): all ROI, location, and DAQ metadata
- **Image extensions** (one per stamp): pixel data, optionally
  RICE-compressed, with per-stamp metadata

### Required Primary Header Keywords

| Keyword | Type | Example | Description |
|---------|------|---------|-------------|
| `ROIROWS` | int | `400` | ROI height in rows |
| `ROICOLS` | int | `400` | ROI width in columns |
| `ROIMILLI` | int | `200` | Integration time in milliseconds |
| `RAFTBAY` | str | `R00` | Raft bay (Rxx format) |
| `CCDSLOT` | str | `SG0` | CCD slot (SGn for guider sensor n) |
| `ROIROW` | int | `800` | ROI start row on CCD |
| `ROICOL` | int | `56` | ROI start column on CCD |
| `ROISEG` | str | `Segment07` | Segment name (SegmentNN) |
| `GDSSEQ` | int | `44220` | GDS sequence number |
| `N_STAMPS` | int | `50` | Number of stamp extensions |

### Optional Primary Header Keywords

| Keyword | Type | Default | Description |
|---------|------|---------|-------------|
| `ROIUNDRC` | int | `0` | Under-scan columns |
| `ROIOVERC` | int | `0` | Over-scan columns |
| `ROIOVERR` | int | `0` | Over-scan rows |
| `ROIFLUSH` | int | `0` | Flush rows |
| `ROISPLIT` | int | `0` | Split ROI mode (0 or 1) |
| `ROICCDTY` | int | `0` | CCD vendor (0=ITL, 1=E2V) |
| `FIRMWARE` | str | `00000000` | Firmware version (hex) |
| `CONTNUM` | str | `0000000000000000` | Controller serial (hex) |
| `PLATFORM` | str | `""` | Platform identifier |
| `OBSID` | str | `""` | Observation ID (also used as START comment) |

### Per-Extension Keywords

| Keyword | Type | Example | Description |
|---------|------|---------|-------------|
| `DAQSTAMP` | int | `1` | Stamp index (informational only; emulator overrides) |
| `STMPTMJD` | double | `60788.012345` | Timestamp as Modified Julian Date |

If `STMPTMJD` is missing, the emulator uses the current time.

### Image Data

Each extension must contain a 2D image of dimensions `ROICOLS` × `ROIROWS`
(NAXIS1 = ROICOLS, NAXIS2 = ROIROWS) with `BITPIX = 32` (int32 pixels).
RICE compression (`ZCMPTYPE = 'RICE_1'`) is supported and recommended.

Pixel values must be in the range `[0, 2^18 - 1]` (262143) to survive
the 18-bit raw encoding used on the GDS stream.

### Python Example

```python
#!/usr/bin/env python3
"""Create a synthetic guider FITS file for gds_emulator."""

import numpy as np
from astropy.io import fits
from astropy.time import Time

# --- Configuration ---
NROWS = 400
NCOLS = 400
NSTAMPS = 10
SEQUENCE = 99999
INTEGRATION_MS = 200
RAFTBAY = "R00"
CCDSLOT = "SG0"
SEGMENT = "Segment07"
ROI_ROW = 800
ROI_COL = 56

# --- Primary HDU (metadata only) ---
primary = fits.PrimaryHDU()
hdr = primary.header
hdr["ROIROWS"]  = NROWS
hdr["ROICOLS"]  = NCOLS
hdr["ROIMILLI"] = INTEGRATION_MS
hdr["RAFTBAY"]  = RAFTBAY
hdr["CCDSLOT"]  = CCDSLOT
hdr["ROISEG"]   = SEGMENT
hdr["ROIROW"]   = ROI_ROW
hdr["ROICOL"]   = ROI_COL
hdr["GDSSEQ"]   = SEQUENCE
hdr["N_STAMPS"] = NSTAMPS
hdr["ROICCDTY"] = 0           # ITL
hdr["OBSID"]    = "synthetic-test-001"

# --- Stamp extensions ---
hdus = [primary]
t0 = Time.now()

for i in range(NSTAMPS):
    # Generate test pattern: ramp + stamp index offset
    pixels = np.mod(
        np.arange(NROWS * NCOLS, dtype=np.int32).reshape(NROWS, NCOLS) + i * 1000,
        262144,  # keep within 18-bit range
    )

    # RICE-compressed image extension
    comp = fits.CompImageHDU(data=pixels, compression_type="RICE_1")
    comp.header["DAQSTAMP"] = i + 1
    comp.header["STMPTMJD"] = (t0 + i * INTEGRATION_MS / 1000.0).mjd

    hdus.append(comp)

hdul = fits.HDUList(hdus)
hdul.writeto("synthetic_guider.fits", overwrite=True)
print(f"Wrote synthetic_guider.fits: {NSTAMPS} stamps, {NROWS}x{NCOLS}")
```

Usage:

```bash
python3 create_synthetic.py
gds_stampinfo synthetic_guider.fits
gds_emulator <partition> 200 synthetic_guider.fits
```

---

## Compiling Against the DAQ SDK

### Build Dependencies

The emulator tools and client libraries require:

- **pthreads** — multi-sensor threading and subscriber internals
- **cfitsio-devel** — FITS file reading (`gds_emulator`, `gds_stampinfo`,
  `gds_stamp_roundtrip`). Install via your package manager, e.g.
  `yum install cfitsio-devel` or `apt install libcfitsio-dev`.

### SDK Installation

The DAQ SDK is distributed as a directory tree with headers, libraries,
and pre-built binaries. Set `DAQ_SDK` to the root of this tree (the
directory containing `include/`, `x86/`, etc.):

```bash
export DAQ_SDK=/path/to/daq-sdk-release
```

### Directory Layout

```
$DAQ_SDK/
├── include/
│   ├── gds/              # GDS headers (Subscriber.hh, Decoder.hh, ...)
│   ├── daq/              # Common DAQ types (Sensor.hh, Location.hh)
│   ├── osa/              # OS abstraction (TimeStamp, etc.)
│   └── ...               # Other component headers
├── x86/
│   ├── bin/              # Pre-built tools (gds_emulator, gds_stampinfo, ...)
│   └── lib/              # Shared libraries (libdaq_gds.so, libdaq_osa.so, ...)
└── examples/             # Example source code
```

### Compiling a Subscriber

A minimal subscriber that inherits from `GDS::Decoder` needs headers
from several components and links against their libraries:

```bash
g++ -o my_subscriber my_subscriber.cc \
    -I$DAQ_SDK/include \
    -L$DAQ_SDK/x86/lib \
    -Wl,-rpath,'$ORIGIN/../lib' \
    -ldaq_gds -ldaq_daq -ldaq_dsid -ldaq_dsi -ldaq_dsm \
    -ldaq_net -ldaq_osa -ldaq_dvi \
    -lpthread
```

The installed binaries use `$ORIGIN/../lib` as their RPATH, so they
find their libraries relative to the binary location. Use the same
convention for your own binaries if you install them alongside the
SDK tools.

Alternatively, set `LD_LIBRARY_PATH` at runtime:

```bash
export LD_LIBRARY_PATH=$DAQ_SDK/x86/lib
./my_subscriber
```
