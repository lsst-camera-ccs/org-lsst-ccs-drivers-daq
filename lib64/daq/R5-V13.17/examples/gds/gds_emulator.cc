
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "../emulator/Source.hh"
#include "../emulator/Publisher.hh"
#include "../emulator/Rendezvous.hh"
#include "../emulator/Player.hh"
#include "../emulator/Service.hh"

#include "dsi/Header.hh"
#include "dsid/Client.hh"
#include "dsm/Client.hh"
#include "dsm/Services.hh"

enum {MAX_SENSORS=32};

const char USAGE[] =
  "usage: gds_emulator [options] <partition> <rate_ms> <file(s)...>\n"
  "       gds_emulator -S [-d <dir>] [-q <n>] [-t <ms>] [-v] <partition>\n"
  "\n"
  "  Server mode (-S): act as the GDS control service for <partition>.\n"
  "  Clients (gds, GDS::Client) command it with wake/sleep/start/stop/\n"
  "  pause/resume/clear/series/config. Takes no files.\n"
  "    -d <dir> data directory for files named by PLAY (default .)\n"
  "\n"
  "  Reads one or more .stamp, .rstamp, or .fits files and publishes\n"
  "  stamps on the GDS stream multicast interface at the specified rate.\n"
  "\n"
  "  Multiple files are published as multiple sensors, each in its\n"
  "  own thread (matching real system behavior).\n"
  "\n"
  "  partition  - DAQ partition name\n"
  "  rate_ms    - interval between stamp timestamps in ms,\n"
  "               auto = reproduce input stamp intervals,\n"
  "               0    = send as fast as possible (stress\n"
  "                      testing; not timing-faithful)\n"
  "  file(s)    - .stamp (decoded) or .rstamp (raw) from gds_archiver,\n"
  "               or .fits guider files from CCS/DM\n"
  "\n"
  "  Options:\n"
  "    -t <ms>  ACK timeout for subscriber discovery (default 500 ms)\n"
  "    -s <n>   skip first n stamps before publishing\n"
  "    -n <n>   publish at most n stamps (per sensor)\n"
  "    -l <n>   loop n times (0 = infinite, default 1)\n"
  "    -q <n>   starting sequence number (default 1)\n"
  "    -c <txt> override RESUME comment (e.g. image name)\n"
  "    -v       verbose (print per-stamp info)\n"
  "\n"
  "  Timestamps:\n"
  "    - Each series is START -> RESUME -> stamps -> STOP. T0 = RESUME\n"
  "      time + file integration time, shared by all sensors.\n"
  "    - Stamp timestamp = T0 + input offset from first played stamp\n"
  "      (auto, 0) or T0 + slot * rate_ms (fixed). Identical across\n"
  "      sensors; each record is sent at its timestamp (deadline).\n"
  "    - Stamp numbers follow the input index (from 0; gaps kept).\n"
  "    - In-file PAUSE/RESUME/STOP are forwarded on the same timeline;\n"
  "      those before the first played stamp (-s) collapse into the\n"
  "      series RESUME.\n"
  "    - All files must share the first played stamp timestamp.\n"
  "    - Each loop pass (-l) is a new series with its own T0.\n";

static void* _sensor_thread(void* arg)
{
  ((GDS::Emulator::Player*)arg)->run();
  return 0;
}

/*
** ++
**
** Server mode: run the emulator's GDS control service until shutdown
** (restarting on a DSI RESTART request, as gds.service does).
**
** --
*/

static int _serve(const char* partition, const char* root, unsigned sequence,
                  uint64_t ack_timeout, bool verbose)
{
  setvbuf(stdout, NULL, _IOLBF, 0);

  DSID::Client dsid;
  uint8_t pid = dsid.lookup(partition);
  if(!dsid.valid(pid))
  {
    printf("ERROR: partition %s does not exist\n", partition);
    return 1;
  }

  // Refuse to start if a GDS service (real or another emulator) already
  // serves this partition: clients bind to whichever answers first.
  // DSI::Client's constructor probes the service group (PROBE); any
  // GDS service answers with its server set.
  {
    DSM::Client probe(DSM::Services::Id::GDS, pid);
    if(probe.partition_set())
    {
      printf("ERROR: a GDS service is already running on partition %s; "
             "refusing to start a second one.\n", partition);
      return 1;
    }
  }

  printf("GDS Emulator (server mode)\n");
  printf("  Partition: %s (%u)\n", partition, (unsigned)pid);
  printf("  Data dir:  %s\n", root);
  printf("  Sequence:  %u (first START)\n", sequence);

  enum DSI::Header::Function reason;
  do {
    GDS::Emulator::Service service(pid, partition, DSID::Client::interface(), root,
                                   (uint16_t)sequence, ack_timeout, verbose);
    reason = service.start();
  } while(reason == DSI::Header::RESTART);

  return 0;
}

/*
** ++
**
**
** --
*/

int main(int argc, char** argv)
{
  bool server = false;
  const char* root = ".";
  bool verbose = false;
  uint64_t ack_timeout = 500;
  unsigned skip  = 0;
  unsigned count = 0;
  unsigned loops = 1;
  unsigned sequence = 1;
  const char* comment = 0;

  int c;
  while(-1 != (c = getopt(argc, argv, "hvSd:t:s:n:l:c:q:")))
  {
    if('S' == c) server = true;
    if('d' == c) root = optarg;
    if('v' == c) verbose = true;
    if('t' == c) ack_timeout = strtoull(optarg, 0, 10);
    if('s' == c) skip  = atoi(optarg);
    if('n' == c) count = atoi(optarg);
    if('l' == c) loops = atoi(optarg);
    if('c' == c) comment = optarg;
    if('q' == c) sequence = atoi(optarg);
    if('h' == c) {printf(USAGE); return 0;}
  }

  if(server)
  {
    if(argc - optind != 1) {printf(USAGE); return 1;}
    return _serve(argv[optind], root, sequence, ack_timeout, verbose);
  }

  if(argc - optind < 3) {printf(USAGE); return 1;}

  const char* partition = argv[optind++];
  bool        automatic = (0 == strcmp(argv[optind], "auto"));
  unsigned    rate_ms   = automatic ? 0 : atoi(argv[optind]);
  ++optind;

  // Remaining arguments are files
  unsigned nsensors = argc - optind;
  if(nsensors > MAX_SENSORS)
  {
    printf("Too many sensors (max %u)\n", MAX_SENSORS);
    return 1;
  }

  printf("GDS Emulator\n");
  printf("  Partition: %s\n", partition);
  if(automatic)
    printf("  Rate:      auto (input stamp intervals)\n");
  else
    printf("  Rate:      %u ms%s\n", rate_ms, rate_ms == 0 ? " (max speed)" : "");
  if(skip)       printf("  Skip:      %u stamps\n", skip);
  if(count)      printf("  Count:     %u stamps per sensor\n", count);
  if(loops == 0) printf("  Loops:     infinite\n");
  if(loops > 1)  printf("  Loops:     %u\n", loops);
  if(comment)    printf("  Comment:   %s\n", comment);
  printf("  Sequence:  %u\n", sequence);
  printf("  Sensors:   %u\n", nsensors);
  if(nsensors > 1) printf("  Mode:      threaded (one thread per sensor)\n");

  GDS::Emulator::Source*    sources[MAX_SENSORS];
  GDS::Emulator::Publisher* publishers[MAX_SENSORS];
  GDS::Emulator::Player*    players[MAX_SENSORS];

  for(unsigned s = 0; s < nsensors; ++s)
  {
    const char* filename = argv[optind + s];

    sources[s] = new GDS::Emulator::Source(filename);
    if(!sources[s]->valid())
    {
      printf("ERROR: cannot open %s\n", filename);
      return 1;
    }

    const GDS::SeriesMetadata& series = sources[s]->series();
    char loc[32];
    sources[s]->startState().sensor().encode(loc);

    // Create per-sensor Publisher (own socket)
    publishers[s] = new GDS::Emulator::Publisher(partition, ack_timeout);

    printf("  [%u] %s  sensor %s  %s  %u x %u\n", s, filename, loc,
           sources[s]->format_name(), series.common().nrows(), series.common().ncols());
  }

  // All files must be from the same acquisition (same first played
  // stamp timestamp and integration time).
  if(!GDS::Emulator::Source::aligned(sources, nsensors, skip))
  {
    printf("Files are not from the same acquisition; refusing to play.\n");
    return 1;
  }

  GDS::Emulator::Rendezvous rendezvous(nsensors);

  GDS::Emulator::Player::Config cfg;
  cfg.rate_ms   = rate_ms;
  cfg.automatic = automatic;
  cfg.skip      = skip;
  cfg.count     = count;
  cfg.loops     = loops;
  cfg.sequence  = (uint16_t)sequence;
  cfg.comment   = comment;
  cfg.verbose   = verbose;

  for(unsigned s = 0; s < nsensors; ++s)
    players[s] = new GDS::Emulator::Player(*sources[s], *publishers[s], rendezvous, cfg);

  // Launch sensor threads (or run inline for single sensor)
  if(nsensors == 1)
  {
    players[0]->run();
  }
  else
  {
    pthread_t threads[MAX_SENSORS];

    for(unsigned s = 0; s < nsensors; ++s)
      pthread_create(&threads[s], 0, _sensor_thread, players[s]);

    for(unsigned s = 0; s < nsensors; ++s)
      pthread_join(threads[s], 0);
  }

  // Summary
  unsigned total_stamps = 0;
  unsigned total_subs   = 0;
  for(unsigned s = 0; s < nsensors; ++s)
  {
    total_stamps += players[s]->stamps_sent();
    if(players[s]->subscribers() > total_subs)
      total_subs = players[s]->subscribers();
  }

  printf("Complete: %u total stamps sent across %u sensors to %u subscribers\n",
         total_stamps, nsensors, total_subs);

  // Cleanup
  for(unsigned s = 0; s < nsensors; ++s)
  {
    delete players[s];
    delete publishers[s];
    delete sources[s];
  }

  return 0;
}
