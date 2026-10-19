#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"
#include "reb/RebPgpStats.hh"

static const char USAGE[] = "usage: reb_pgpstats <partition> [-v] [-c] [DAQ Locations]\n"
  " -v  Verbose: full per-REB field breakdown\n"
  " -c  Clear counters after reading (note: resets all status counters, PGP and image)\n\n";

static bool display(REB::RebPgpStats& stats, const DAQ::Location& loc,
                    bool verbose, bool clear, bool header)
{
  char locstr[16];
  loc.encodeReb(locstr);

  if(!stats.read(loc))
  {
    printf("%8s: ERROR: no response\n", locstr);
    return header;
  }

  if(verbose)
    stats.dump_full(locstr);
  else
  {
    stats.dump(locstr, header);
    header = false;
  }

  if(clear) stats.clear(loc);

  return header;
}

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  char* partition = argv[1];
  bool  verbose   = false;
  bool  clear     = false;

  // Parse options from remaining args
  int locStart = 2;

  for(int i = 2; i < argc; i++)
  {
    if(0 == strcmp(argv[i], "-v"))
    {
      verbose = true;
      locStart = i + 1;
    }
    else if(0 == strcmp(argv[i], "-c"))
    {
      clear = true;
      locStart = i + 1;
    }
    else
    {
      locStart = i;
      break;
    }
  }

  REB::RebPgpStats stats(partition);

  DAQ::LocationSet locs(argc - locStart, (const char**)&argv[locStart]);
  locs &= stats.locations();

  if(!locs) locs = stats.locations();

  DAQ::ScienceSet   science;
  DAQ::WavefrontSet wavefront;
  DAQ::GuidingSet   guiding;

  DAQ::Location loc;
  bool header = true;
  int  total  = 0;

  while(science.remove(loc))
    if(locs.had(loc)) { header = display(stats, loc, verbose, clear, header); total++; }

  while(wavefront.remove(loc))
    if(locs.had(loc)) { header = display(stats, loc, verbose, clear, header); total++; }

  while(guiding.remove(loc))
    if(locs.had(loc)) { header = display(stats, loc, verbose, clear, header); total++; }

  printf("Total Sources = %d\n", total);
  if(clear) printf("Note: all status counters cleared (PGP and image)\n");

  return EXIT_SUCCESS;
}
