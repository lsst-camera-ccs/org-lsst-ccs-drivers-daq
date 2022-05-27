
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

#define PROGRAM    argv[0]
#define PARTITION  argv[1]
#define FOLDER     argv[2]
#define IMAGE_BASE argv[3]
#define START      argv[4]
#define REPEATS    argv[5]
#define SOURCES    argv[6]

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: cms_checksum <partition> <folder> <image name> <sources>\n";

static const DAQ::LocationSet _parse_targets(const DAQ::LocationSet&, int argc, const char** argv);
static bool _crc_error(const IMS::Image& image, const IMS::Editor::Crc32& decoder);
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
  
  if(argc < 6) {printf(USAGE); return EXIT_SUCCESS;}

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
 
  IMS::Store store(PARTITION);
  CMS::Camera camera(store);

  DAQ::LocationSet locs = _parse_targets(camera.sources(), argc-6, (const char**)&SOURCES);

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
    sprintf(image_name, "%s%i", IMAGE_BASE, image_number);

    IMS::Id id = store.catalog.lookup(image_name, FOLDER);
    IMS::Image image(id, store);
    
    if(!image) {printf(NOT_VALID, FOLDER, IMAGE_BASE); return -1;}

    printf("%s - %016llX - %s ", image.metadata().timestamp().decode(), (long long unsigned)image.id().value(), image.metadata().name());
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
      printf("\n");
      decoder.dump(2);
    }

    ++image_number;
    if(++iterations==repeats) break;
  }

  if(CHECK_DATA)
    printf("Checked %i images, %i PASSED, %i FAILED\n", iterations, passed, failed);

  return(EXIT_SUCCESS);
}


#include <string.h>

static const DAQ::LocationSet _parse_targets(const DAQ::LocationSet& all, int argc, const char** argv)
{
  
  DAQ::LocationSet targets;
  
  if(0 == argc) { targets |= all; return targets; }
  
  bool exclude = false;
  
  for(int i=0; i<argc; ++i)
  {
    DAQ::LocationSet set;
    if(0==strncmp(argv[i], "-a", 2))
    {
      set |= all;
    }
    else if(0==strcmp(argv[i], "-x")) exclude = true;
    else if(0==strcmp(argv[i], "-s")) set  = DAQ::ScienceSet();
    else if(0==strcmp(argv[i], "-g")) set  = DAQ::GuidingSet();
    else if(0==strcmp(argv[i], "-w")) set  = DAQ::WavefrontSet();
    else                              set |= DAQ::LocationSet(1, &argv[i]);
    
    if(exclude) targets &= ~set;
    else        targets |=  set;
  }   
  
  targets &= all;
  
  return targets;
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
