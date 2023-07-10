
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"
#include "rms/InstructionList.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"
#include "ims/Stream.hh"
#include "ims/Barrier.hh"
#include "ims/../src/editor/Crc32.hh"

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: cms_checksum <partition> <folder> <image base> <start idx> <repeats> [DAQ Locations]\n";

static bool   _crc_error(const IMS::Image& image, const IMS::Editor::Crc32& decoder);
static uint32_t calc_crc(const DAQ::Location&, unsigned size);

static const unsigned EXPECTED_SENSOR_SIZE = 42467328; // Expected image size in bytes to calc expected crc
static uint32_t expected_crc[DAQ::LocationSet::SIZE];

static const bool CHECK_DATA  = false;

static bool doExit = false;

void quitIt(int s)
{
  printf("\n");
  if(doExit)
  {
    printf("\n");
    exit(1);
  }
  doExit=true;
}

/*
** ++
**
**
** --
*/
static const char NOT_VALID[]      = "Couldn't find metadata for image %s/%s\n"; 
static const char NO_SOURCES[]     = "No valid sources specified\n";

#include <zlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  
  bool verbose = false;

  if(argc < 6) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  int arg = 0;
  char* PARTITION = argv[++arg];
  char* FOLDER    = argv[++arg];
  if(0 == strncmp(FOLDER,"-v", 2))
  {
    verbose = true;
    FOLDER = argv[++arg];
  }
  char*  IMAGE_BASE =  argv[++arg];
  char*  START      =  argv[++arg];
  char*  REPEATS    =  argv[++arg];
  char** SOURCES    = &argv[++arg];

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
 
  IMS::Store store(PARTITION);
  CMS::Camera camera(store);

  if(argc-arg) verbose = true;
  DAQ::LocationSet locs(argc-arg, (const char**)SOURCES);
  locs &= camera.sources();

  if(!locs) {printf("No locations specified\n"); return 0;}

  if(CHECK_DATA)
  {
    printf("Calculating expected CRC values for %i Sources\n", locs.numof());
    DAQ::ScienceSet science;
    DAQ::GuidingSet guiding;

    DAQ::LocationSet remaining = locs;
    DAQ::Location loc;
    while(remaining.remove(loc))
    {
      unsigned size = EXPECTED_SENSOR_SIZE;
      if     (science.has(loc)) size *= DAQ::Sensor::SCIENCE;
      else if(guiding.has(loc)) size *= DAQ::Sensor::GUIDE;

      expected_crc[loc.index()] = calc_crc(loc, size);
    }
  }

  unsigned repeats = atol(REPEATS);
  unsigned image_number = atol(START);
  char image_name[64];
  unsigned iterations = 0;
  unsigned passed = 0;
  unsigned failed = 0;

  while(!doExit)
  {
    sprintf(image_name, "%s%06i", IMAGE_BASE, image_number);

    IMS::Id id = store.catalog.lookup(image_name, FOLDER);
    IMS::Image image(id, store);
    
    if(!image) {printf(NOT_VALID, FOLDER, image_name); return -1;}

    printf("%s - %016llX - %s/%s ", image.metadata().timestamp().decode(), (long long unsigned)image.id().value(), FOLDER, image.metadata().name());
    fflush(stdout);

    IMS::Editor::Crc32 decoder(image, locs);
    decoder.run();

    if(CHECK_DATA)
    {
      if(!_crc_error(image, decoder))
      {
	++passed;
	printf("- OK\n");
      }
      else
      {
	++failed;
      }
    }
    else
    {
      printf("- %llu - 0x%08X\n", (unsigned long long) decoder.size(), (unsigned)decoder.crc());
      //printf("\n");
      if(verbose) decoder.dump(2);
    }

    ++image_number;
    if(++iterations==repeats) break;
  }

  if(CHECK_DATA)
    printf("Checked %i images, %i PASSED, %i FAILED\n", iterations, passed, failed);

  return(EXIT_SUCCESS);
}

static bool _crc_error(const IMS::Image& image, const IMS::Editor::Crc32& decoder)
{
  bool error = false;

  DAQ::LocationSet locs = image.metadata().elements();
  DAQ::Location loc;
  while(locs.remove(loc))
  {
    if(decoder.crc(loc) != expected_crc[loc.index()])
    {
      printf("\n");
      decoder.dump(loc, 2);
      printf("ERROR - expected CRC 0x%08x\n", expected_crc[loc.index()]);
      error = true;
    }
  }
  return error;
}

static uint32_t calc_crc(const DAQ::Location& loc, unsigned size)
{
  uint32_t crc = crc32(0, NULL, 0);
  srand48(loc.index());
  unsigned remaining = size/sizeof(uint32_t);
  while(remaining)
  {
    uint32_t word = lrand48();
    crc = crc32(crc, (uint8_t*)&word, sizeof(uint32_t));
    --remaining;
  }

  char buf[16];
  printf("Location %s, size %i, index %i, crc %08X\n", loc.encode(buf), size, loc.index(), crc);

  return crc;
}
