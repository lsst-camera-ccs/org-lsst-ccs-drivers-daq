#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gds/Client.hh"

static const char USAGE[] =
  "usage:  gds <partition> <command> [file] [options]\n"
  "        gds <partition> play <file> [file...] [-q]\n"
  "\n"
  "  commands:  wake, sleep, start, stop, pause, resume, series, config, clear\n"
  "\n"
  "  play:      GDS emulator only (gds_emulator -S): load one .fits, .stamp\n"
  "             or .rstamp file per sensor, named relative to the emulator's\n"
  "             data directory, for the next START. A real GDS rejects\n"
  "             play (\"Invalid command specified\").\n"
  "\n"
  "  options:\n"
  "    -c, --comment <text>   comment string (default: none)\n"
  "    -n, --loops <count>    number of stamps (default: infinite)\n"
  "    -q, --quiet            suppress response output\n";

/*
** ++
**
** play: emulator-only (GDS::Client::emu_play()). Takes a list of files
** rather than one, so it is handled before the common argument parsing.
**
** --
*/

static int _play(const char* partition, int argc, char** argv)
{
  const char* files[GDS::Client::EMU_MAX_FILES];
  unsigned    nfiles = 0;
  bool        quiet  = false;

  for(int i = 3; i < argc; ++i)
  {
    if(0==strcmp(argv[i], "-q") || 0==strcmp(argv[i], "--quiet")) quiet = true;
    else if(argv[i][0] == '-')
    {
      printf("unknown option: %s\n", argv[i]);
      printf(USAGE);
      return EXIT_SUCCESS;
    }
    else if(nfiles < GDS::Client::EMU_MAX_FILES) files[nfiles++] = argv[i];
    else
    {
      printf("play: at most %u files\n", (unsigned)GDS::Client::EMU_MAX_FILES);
      return EXIT_SUCCESS;
    }
  }

  if(!nfiles) {printf("play command requires at least one file\n"); return EXIT_SUCCESS;}

  GDS::Client client(partition);

  GDS::Status      response;
  GDS::LocationSet loaded;
  char             reason[GDS::Client::EMU_REASON];

  int error = client.emu_play(files, nfiles, response, loaded, reason);

  if(error < 0 || error == GDS::Exception::Id::INVALID_COMMAND)
  {
    char buffer[256];
    printf("%s\n", GDS::Exception::decode(error, buffer));
  }
  else if(!quiet)
  {
    response.dump();
    if(error) printf("  PLAY rejected: %s\n", reason);
    else      printf("  Loaded %u sensor(s): %s\n", loaded.numof(), loaded.encode());
  }

  return EXIT_SUCCESS;
}

#include "gds/LocationSet.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/RoiFile.hh"
#include "gds/Series.hh"
#include "gds/ClearParameters.hh"
#include "gds/ClearFile.hh"

int main(int argc, char** argv)
{
  if(argc<3) {printf(USAGE); return EXIT_SUCCESS;}

  const char* partition = argv[1];
  const char* command   = argv[2];

  if(0==strcmp(command, "play")) return _play(partition, argc, argv);

  // Find the file argument (if any) — first non-option arg after command
  const char* file = 0;
  if(argc > 3 && argv[3][0] != '-') file = argv[3];

  // Parse options from remaining arguments
  const char* comment = "";
  uint32_t    loops   = GDS::Client::INFINITE_LOOP;
  bool        quiet   = false;

  for(int i = file ? 4 : 3; i < argc; ++i)
  {
    if((0==strcmp(argv[i], "-c") || 0==strcmp(argv[i], "--comment")) && i+1 < argc)
      comment = argv[++i];
    else if((0==strcmp(argv[i], "-n") || 0==strcmp(argv[i], "--loops")) && i+1 < argc)
      loops = atoi(argv[++i]);
    else if(0==strcmp(argv[i], "-q") || 0==strcmp(argv[i], "--quiet"))
      quiet = true;
    else
    {
      printf("unknown option: %s\n", argv[i]);
      printf(USAGE);
      return EXIT_SUCCESS;
    }
  }

  GDS::Client client(partition);

  int error;
  GDS::Status response;
  GDS::Series series;
  GDS::Series idle;
  GDS::RoiCommon common;
  char locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  GDS::RoiLocation* locs = (GDS::RoiLocation*)locbuf;
  unsigned nlocs = 0;
  GDS::ClearParameters idle_params;
  GDS::LocationSet clr_locs;

  if(0==strcmp(command, "start")) {
    if(!file) {printf("start command requires ROI JSON file\n"); return EXIT_SUCCESS;}
    GDS::RoiFile rois(file);
    if(!rois.next(common, locs, nlocs)) return EXIT_SUCCESS;
    if(!client.validate(common, locs, nlocs)) return EXIT_SUCCESS;

    error = client.start(common, locs, nlocs, comment, loops, response);
  }
  else if(0==strcmp(command, "clear"))
  {
    if(!file) {printf("clear command requires Clear JSON file\n"); return EXIT_SUCCESS;}
    GDS::ClearFile json(file);
    GDS::ClearParameters params;
    json.fillParams(params);
    error = client.clear(params, loops, response);
  }
  else if(0==strcmp(command, "stop"))
    error = client.stop(comment, response);
  else if(0==strcmp(command, "pause"))
    error = client.pause(comment, response);
  else if(0==strcmp(command, "resume"))
    error = client.resume(comment, response);
  else if(0==strcmp(command, "wake"))
  {
    if(!file) {printf("wake command requires Clear JSON file\n"); return EXIT_SUCCESS;}
    GDS::ClearFile json(file);
    GDS::ClearParameters params;
    json.fillParams(params);
    error = client.wake(params, response);
  }
  else if(0==strcmp(command, "sleep"))
    error = client.sleep(response);
  else if(0==strcmp(command, "series"))
    error = client.series(response, series, idle);
  else if(0==strcmp(command, "config"))
    error = client.config(response, idle_params, common, locs, nlocs);
  else
  {
    printf("command must be one of: wake, sleep, start, stop, pause, resume, series, config, clear, play\n");
    return EXIT_SUCCESS;
  }
  
  if(error<0)
  {
    char buffer[256];
    printf("%s\n", GDS::Exception::decode(error, buffer));
  }
  else if(!quiet)
  {
    response.dump();
    if(series || idle)
      printf("  Last transition: %s", response.state_out().dump());
    if(series)
      series.dump(2);
    if(idle)
      idle.dump(2);
    if(idle_params)
    {
      printf("  Idle:\n");
      idle_params.dump(4);
    }
    if(nlocs)
    {
      common.dump(2);
      unsigned remaining = nlocs;
      const GDS::RoiLocation* loc = locs;
      printf("  Locations (%i):\n", nlocs);
      while(remaining--) loc++->dump(4);
    }
  }

  return EXIT_SUCCESS;
}
