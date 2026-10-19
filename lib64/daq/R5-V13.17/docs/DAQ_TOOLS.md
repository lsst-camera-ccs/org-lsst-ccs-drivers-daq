# DAQ Tools Reference

Command-line utilities for the LSST DAQ system, organized by subsystem.
All utilities that accept `[DAQ Locations]` use the standard location
syntax documented in `DAQ::LocationSet::usage()`. Locations can be
specified as `R22/Reb0` or `22/0` format.

---

## REB — Raft Electronics Board

### reb_info

    usage: reb_info <partition> [-v] [DAQ Locations]

Read FPGA build information from REB status registers. Displays version,
build timestamp, and location metadata for each REB in the partition.

- `-v` — Verbose: decode individual version register fields.

### reb_pgpstats

    usage: reb_pgpstats <partition> [-v] [-c] [DAQ Locations]

Display PGP link statistics from REB hardware counters.

- `-v` — Verbose: full per-REB field breakdown.
- `-c` — Clear counters after reading. Note: resets all status counters
  (both PGP and image).

### reb_imgstats

    usage: reb_imgstats <partition> [-v] [DAQ Locations]

Display image transfer statistics from REBs, per virtual channel.

- `-v` — Verbose: full per-VC field breakdown.

### reb_fw_load

    usage: reb_fw_load <partition> -s <slot> -f <file> [-i] -- <location> [location ...]

Load a firmware bitfile into a specified EEPROM slot on one or more REBs.
Displays bitfile info and prompts for confirmation before proceeding.

- `-s <slot>` — EEPROM slot number to load.
- `-f <file>` — Path to the bitfile.
- `-i` — Ignore location check (override bitfile location metadata).

### reb_fw_boot

    usage: reb_fw_boot <partition> -s <slot> -- <location> [location ...]

Boot REBs from a specified EEPROM slot. Prompts for confirmation.

- `-s <slot>` — EEPROM slot to boot from.

### reb_fw_erase

    usage: reb_fw_erase <partition> -s <slot> -- <location> [location ...]

Erase a specified EEPROM slot on one or more REBs. Prompts for confirmation.

- `-s <slot>` — EEPROM slot to erase.

### reb_fw_reset

    usage: reb_fw_reset <partition> -- <location> [location ...]

Reset the EEPROM state machine on one or more REBs. Prompts for confirmation.

### reb_fw_dump

    usage: reb_fw_dump -f <file> [-b] [-o <offset>] [-l <length>]

Dump raw content of a REB firmware bitfile in hex. Operates on local files;
does not require a partition.

- `-f <file>` — Bitfile to dump.
- `-b` — Dump as bytes (default: 32-bit words).
- `-o <offset>` — Starting offset in bytes.
- `-l <length>` — Number of bytes to dump.

---

## RMS — Raft Management Service

### rms_read

    usage: rms_read <partition> <register[:num]> [DAQ Locations]

Read one or more REB registers. The register argument may include a count
suffix (e.g., `0x100000:4` reads 4 consecutive registers starting at
`0x100000`).

### rms_write

    usage: rms_write <partition> <address> <data> [DAQ Locations]

Write a value to a REB register.

### rms_load

    usage: rms_load <partition> <instruction cache file> [DAQ Locations]

Load an RMS instruction cache file (a sequence of register read/write
instructions, typically produced by `gds_seq_convert`) to specified REBs.

### rms_stats

    usage: rms_stats <partition> [-c] [DAQ Locations]

Display RMS service statistics.

- `-c` — Clear counters after reading.

### rms_fwstats

    usage: rms_fwstats <partition> [-c] [DAQ Locations]

Display RMS firmware-level statistics.

- `-c` — Clear counters after reading.

### rms_drstats

    usage: rms_drstats <partition> [-c] [DAQ Locations]

Display RMS driver-level statistics.

- `-c` — Clear counters after reading.

### rms_set_link

    usage: rms_set_link <partition> -d|-u|-r [duration] [DAQ Locations]

Control the PGP link state on REBs.

- `-d` — Bring the link down.
- `-u` — Bring the link up.
- `-r [duration]` — Reset: bring the link down, wait `duration` seconds,
  bring it back up. Default duration is 15 seconds.

### rms_reset_reb

    usage: rms_reset_reb <partition> [DAQ Locations]

Hardware reset of the REB at specified locations. Prompts for
confirmation before executing.

### rms_reboot_rce

    usage: rms_reboot_rce <partition> [DAQ Locations]

Reboot the readout RCE that serves the specified DAQ locations (REBs).
Note that an RCE typically serves more than one REB — rebooting it
affects all REBs on that RCE. Prompts for confirmation before executing.

### rms_probe

    usage: rms_probe <partition> [DAQ Locations]

Probe the RMS service to discover active locations. Displays version and
address info for each responding RCE.

### lam_monitor

    usage: lam_monitor <partition> [vector] [DAQ Locations]

Monitor LAM (Look-At-Me) interrupts from REBs. Waits for and prints
LAMs as they arrive. Optional `vector` bitmask and location arguments
filter which LAMs are displayed.

### lam_wait

    usage: lam_wait <partition> <vector> <DAQ Location> [DAQ Locations...]

Wait for all bits of a LAM vector to be received from the specified
locations. Blocks until all arrive or timeout.

---

## SCS — Synchronous Command Service

### scs_stats

    usage: scs_stats <partition> [-c]

Display per-slot statistics for the synchronous command distribution
infrastructure.

- `-c` — Clear counters after reading.

---

## IMS — Image Management Service

### ims_list

    usage: ims_list <partition> [folder]

List contents of the IMS data store. Without a folder argument, lists
all folders. With a folder, lists all images in that folder. Output
begins with a header line followed by one image per line, with Readout
Time as the first column to enable `| sort`.

### ims_inspect

    usage: ims_inspect <partition> <folder/image>

Inspect a stored image, displaying detailed metadata and per-source
information. The image is specified as `folder-name/image-name`.

### ims_badpixel

    usage: ims_badpixel <partition> [-d] <folder/image> [low high [max]]

Scan a stored image for pixels outside a configurable good range.

- Image specified as `folder-name/image-name`.
- Default good range: `[10000, 240000]`.
- `-d` — Dump bad stripes with surrounding context. Optional `max`
  limits stripes shown (default 100, 0 for unlimited).

### ims_image_monitor

    usage: ims_image_monitor <partition> [-v] [-s] [-r] [-f <dir>] [-c] [-d] [-b [low,high]] [sources]

Monitor the image stream in real time, with optional data processing.

- `-v` — Print full synopsis for each image.
- `-s` — Process data stream as it is published.
- `-r` — Read data from data store.
- `-f <dir>` — Write image data to files in the specified directory.
- `-c` — Calculate CRC32 checksums.
- `-d` — Decode image data.
- `-b [low,high]` — Count bad pixels outside good range (default:
  `10000,240000`).

### ims_stream_monitor

    usage: ims_stream_monitor <partition>

Monitor raw IMS/XDS stream events (image, metadata, data bucket
arrivals) in real time. Low-level diagnostic tool.

### ims_timer

    usage: ims_timer <partition>

Time image acquisition by monitoring the IMS stream. Prints elapsed
time from the IMAGE event to the last DATA event.

### ims_stream_example

    usage: ims_stream_example <partition>

Example program demonstrating the IMS::Stream and IMS::Image APIs.
Waits for one image with timeout, blocks on the source barrier, and
prints the image synopsis.

### ims_lookup_page

    usage: ims_lookup_page <partition> <folder> <image name> <source> <offset>

Look up the XDS page location for a specific byte offset within an
image source. Displays IMS ID, XDS ID, page ID, server index, and
IP address.

---

## XDS — eXchange Data Store

### xds_list

    usage: xds_list <partition>

Display XDS storage capacity summary: consistency state, number of
servers, pages per server, total and remaining capacity in pages,
bytes, and gigabytes, and free fraction percentage.

### xds_config

    usage: xds_config <partition>

Display the XDS server configuration. Shows pages, maximum pages,
and server set for each storage server, plus the client's expected
configuration. Flags inconsistent servers with `*`.

### xds_sample

    usage: xds_sample <partition> [-c]

Display per-server XDS statistics in tabular form: capacity, remaining,
used pages, wakeups, reads, writes, lookups, removes, and socket pool
remaining. Includes a summary row.

- `-c` — Reset server statistics after sampling.

### xds_probe

    usage: xds_probe <partition>

Probe the XDS service to discover active servers. Displays version and
address info for each responding server.

### xds_reboot_rce

    usage: xds_reboot_rce <partition> [DSI Locations (slot/bay/rce)]

Reboot the Storage RCE at the specified DSI locations. The target set is
intersected with the partition's server set, so only reachable servers
are affected. Prompts for confirmation before executing.

---

## CMS — Camera Management Service

### cms_trigger

    usage: cms_trigger <partition> <image base name> <starting index> <number of images> [DAQ Locations]

Trigger camera image acquisition. Acquires a sequence of numbered images
(e.g., `test_0000`, `test_0001`, ...). Supports Ctrl-C for graceful
interruption.

### cms_inspect

    usage: cms_inspect <partition> [-v] <folder> <image base> <start idx> <repeats>

Inspect a range of stored images, printing synopsis and source
information.

- `-v` — Verbose output.

### cms_delete

    usage: cms_delete <partition> <image base name> <starting index> <number of images>

Delete a range of numbered images from the IMS data store. Supports
Ctrl-C for graceful interruption.

### cms_checksum

    usage: cms_checksum <partition> [-v] <folder> <image base> <start idx> <repeats> [DAQ Locations]

Compute CRC32 checksums over a range of stored images. Can verify
against expected values for test patterns.

- `-v` — Verbose output.

---

## DSM — DSI Service Manager

### dsm_dump

    usage: dsm_dump <partition> [options] [locations]

Dump DSM properties for a partition. Without options, shows a default
summary. With one or more property flags, shows only those properties.

Options:
- `--addr` — Service and stream addresses.
- `--mount` — Mountpoint property.
- `--path` — Path property.
- `--release` — Release property.
- `--role` — Role property.
- `--sensor` — Sensor property.
- `--sequencer` — Sequencer property.
- `--source` — Source property.
- `--vendor` — Vendor property.
- `--all` — All of the above.

### dsm_net_stats

    usage: dsm_net_stats <partition>

Display IPV4 network statistics for all RCEs in a partition.

### dsm_startup

    usage: dsm_startup <backing directory> [network interface] [dsi locations...]

Start DSM service instances. Infrastructure tool for system bootstrap;
not typically used by operators.

---

## GDS — Guider Data System

### gds

    usage: gds <partition> <command> [args]

GDS control client. Commands:

- `start <roi-file>` — Start guider with ROI configuration (JSON).
- `stop` — Stop guider.
- `pause` — Pause guider.
- `resume` — Resume guider.
- `wake <clear-file>` — Wake guider with clear parameters (JSON).
- `sleep` — Put guider to sleep.
- `series` — Query current series info.
- `config` — Query current configuration.

### gds_probe

    usage: gds_probe <partition>

Probe GDS and GDR services in a partition. Displays version, address,
and platform info for each responding server.

### gds_relay

    usage: gds_relay <partition> [-v] [-t <timeout>] [locations]

Relay guider data packets from specified locations. Runs continuously.

- `-v` — Verbose output.
- `-t <timeout>` — Timeout in 10ms ticks.

### gds_listener

    usage: gds_listener <partition> [-d] [-v] [locations]

Subscribe to and display guider data packets. Runs continuously.

- `-d` — Decode guider data.
- `-v` — Verbose output.

### gds_archiver

    usage: gds_archiver <partition> [-d <dir>] [-r] [locations]

Archive guider data packets to files on disk. Runs continuously.

- `-d <dir>` — Output directory (default: current directory).
- `-r` — Raw mode.

### gds_seq_convert

    usage: gds_seq_convert <seq.compiled> <seq.rms>

Convert a compiled sequencer file (text format with hex `addr:data`
pairs) into an RMS instruction cache file suitable for `rms_load`.

### gds_rstamp_unpack

    usage: gds_rstamp_unpack [-v] <rstamp file(s)>

Unpack raw guider stamp files, expanding packed pixel data. Produces
`.rstamp.unpacked` files.

- `-v` — Verbose: dump metadata.

### gds_rstamp_decode

    usage: gds_rstamp_decode [-v] [-s <segment>] <rstamp file(s)>

Decode raw guider stamp files into decoded pixel values. Produces
`.rstamp.decoded` files.

- `-v` — Verbose output.
- `-s <segment>` — Decode only the specified segment.

### gds_test_fast

    usage: gds_test_fast <partition> <loops> <roifile>

Stress-test the GDS by rapidly cycling through start/resume/pause/stop
sequences. Test utility.

### gds_test_wakesleep

    usage: gds_test_wakesleep <partition> <loops> <clearfile>

Stress-test the GDS wake/sleep cycle. Test utility.

---

## EMU — Emulation

### emu_reboot_rce

    usage: emu_reboot_rce <partition> [DAQ Locations]

Reboot the readout RCE that serves the specified DAQ locations (Emulated
REBs). Note that an RCE typically serves more than one REB — rebooting it
affects all REBs on that RCE. Prompts for confirmation before executing.

### emu_probe

    usage: emu_probe <partition> [DAQ Locations]

Probe the EMU service to discover active locations. Displays version and
address info for each responding RCE.

---

## RDS — Raft Data Service

### rds_probe

    usage: rds_probe <partition> [DAQ Locations]

Probe the RDS service to discover active locations. Displays version and
address info for each responding RCE.

### rds_stats

    usage: rds_stats <partition> [-c] [DAQ Locations]

Display RDS service statistics.

- `-c` — Clear counters after reading.

### rds_xstats

    usage: rds_xstats <partition> [-c] [DAQ Locations]

Display RDS XDS-layer statistics.

- `-c` — Clear counters after reading.

### rds_flush

    usage: rds_flush <partition> [DAQ Locations]

Flush the RDS data store queues at specified locations.

### rds_shell

    usage: rds_shell <partition> [network]

Interactive RDS utility shell. Optional network interface argument.

---

## DSID — DSI Daemon

### dsid_startup

    usage: dsid_startup <site directory> [network interface]

Start the DSID service from a site backing directory. Infrastructure
tool for system bootstrap; not typically used by operators.
