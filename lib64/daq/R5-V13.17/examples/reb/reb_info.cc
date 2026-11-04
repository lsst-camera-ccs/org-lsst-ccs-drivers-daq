#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"
#include "daq/FpgaBuildInfo.hh"
#include "reb/RebInfo.hh"

static const char USAGE[] = "usage: reb_info <partition> [options] [DAQ Locations]\n"
  "  -v          Verbose: decode version register fields\n"
  "  --id        Board ID\n"
  "  --build     Build string\n"
  "  --hash      Git hash\n"
  "  --version   Version register\n"
  "  --clock     Clock period\n"
  "  --all       All fields (default)\n\n";

static void display(REB::RebInfo& info, const DAQ::Location& loc,
                    unsigned flags, bool verbose)
{
  char locstr[16];
  loc.encodeReb(locstr);

  if(!info.read(loc))
  {
    printf("%8s: ERROR: no response\n", locstr);
    return;
  }

  info.dump(locstr, flags, verbose);
}

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  bool     verbose = false;
  int      id      = 0;
  int      build   = 0;
  int      hash    = 0;
  int      version = 0;
  int      clock_  = 0;
  int      all     = 0;

  static struct option long_options[] =
  {
    {"id",      no_argument, 0, 0},
    {"build",   no_argument, 0, 0},
    {"hash",    no_argument, 0, 0},
    {"version", no_argument, 0, 0},
    {"clock",   no_argument, 0, 0},
    {"all",     no_argument, 0, 0},
    {0, 0, 0, 0}
  };

  // Reset getopt
  optind = 2;

  while(1)
  {
    int option_index = 0;
    int c = getopt_long(argc, argv, "v", long_options, &option_index);

    if(c == -1) break;

    if(c == 'v')
    {
      verbose = true;
    }
    else if(c == 0)
    {
      switch(option_index)
      {
        case 0: id      = 1; break;
        case 1: build   = 1; break;
        case 2: hash    = 1; break;
        case 3: version = 1; break;
        case 4: clock_  = 1; break;
        case 5: all     = 1; break;
      }
    }
  }

  if(all) { id = 1; build = 1; hash = 1; version = 1; clock_ = 1; }

  unsigned flags = 0;
  if(id)      flags |= REB::FpgaBuildInfo::DUMP_ID;
  if(build)   flags |= REB::FpgaBuildInfo::DUMP_BUILD;
  if(hash)    flags |= REB::FpgaBuildInfo::DUMP_HASH;
  if(version) flags |= REB::FpgaBuildInfo::DUMP_VERSION;
  if(clock_)  flags |= REB::FpgaBuildInfo::DUMP_CLOCK;

  if(!flags) flags = REB::FpgaBuildInfo::DUMP_ALL;

  char* partition = argv[1];

  REB::RebInfo info(partition);

  DAQ::LocationSet locs(argc - optind, (const char**)&argv[optind]);
  locs &= info.locations();

  if(!locs) locs = info.locations();

  DAQ::ScienceSet   science;
  DAQ::WavefrontSet wavefront;
  DAQ::GuidingSet   guiding;

  DAQ::Location loc;

  while(science.remove(loc))
    if(locs.had(loc)) display(info, loc, flags, verbose);

  while(wavefront.remove(loc))
    if(locs.had(loc)) display(info, loc, flags, verbose);

  while(guiding.remove(loc))
    if(locs.had(loc)) display(info, loc, flags, verbose);

  return EXIT_SUCCESS;
}
