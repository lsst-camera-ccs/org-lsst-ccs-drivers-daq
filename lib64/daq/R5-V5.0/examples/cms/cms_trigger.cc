
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>
#include <string.h>
#include <zlib.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/GuidingSet.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"
#include "ims/Stream.hh"
#include "ims/Barrier.hh"

#include "ims/../src/editor/Decoder.hh"
#include "ims/../src/editor/Crc32.hh"
#include "ims/../src/editor/Reader.hh"
#include "ims/../src/editor/File.hh"

#define PROGRAM    argv[0]
#define PARTITION  argv[1]
#define IMAGE_BASE argv[2]
#define START      argv[3]
#define REPEATS    argv[4]
#define SOURCES    argv[5]

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: cms_trigger <partition> <image base name> <starting index> <number of images> [sources]\n";

static const char folder[]="";
static bool doExit = false;

#define SOURCE_TIMEOUT (10 * 100) // 10 seconds expressed in 10ms tics... 


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

static const DAQ::LocationSet _parse_targets(const DAQ::LocationSet&, int argc, const char** argv);
static bool _crc_error(const IMS::Image& image, const IMS::Editor::Crc32& decoder);
static uint32_t calc_crc(const DAQ::Location&, unsigned size);

/*
** ++
**
**
** --
*/

static const char SUCCESS[]        = "Triggered using opcode %d the image named %s at %s\n";
static const char FAILED[]         = "Failed to trigger using opcode %d the image to be named %s (%s)\n";
static const char INVALID_OPCODE[] = "The second argument is not a valid opcode\n";
static const char NOT_VALID[]      = "%s is not a valid image specifier (first argument must be of the form: 'folder-name/image-name')\n";
static const char NO_SOURCES[]     = "No valid sources specified\n";

static const bool OVERLAP_CRC = false;
static const bool CHECK_DATA  = false;
static const bool DUMP_CRC    = false;
static const bool ERROR_EXIT  = false;

static const unsigned EXPECTED_SENSOR_SIZE = 42467328; // Expected image size in bytes to calc expected crc
static uint32_t expected_crc[DAQ::LocationSet::SIZE];

//static const uint32_t expected_crc = 0xD559EFBD; // Dummy plugin 32-bit increment
//static const uint32_t expected_crc = 0xC607B90A; // Dummy image all a5's
//static const uint32_t expected_crc = 0xFD89FA1D; // Dummy image of 0xDEADBEEF
//static const uint32_t expected_crc = 0x8CDA7B95; // Incrementing 18-bit packed data
//static const uint32_t expected_crc = 0xEA824C63; // Incrementing 16-bit truncated data
//static const uint32_t expected_crc = 0x684FC5FF; // Constant 0x9695 16-bit data
//static const uint32_t expected_crc = 0xA25B6FC4; // 0x1000 stripes of incrementing 16-bit trunc data
//static const uint32_t expected_crc = 0x6A8D4915; // 0x30 stripes of incrementing 16-bit trunc data
//static const uint32_t expected_crc = 0x7B5AEF53; // CRC for Stefano's pattern when reset
                                     // with every image. (sequencer bit-16 does
                                     // does the reset. Register 0x400006 turns
                                     // on the pattern.

int main(int argc, char* argv[])
{

  if(argc < 5) {printf(USAGE); return EXIT_SUCCESS;}
  
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
  
  IMS::Store store(PARTITION);
  CMS::Camera camera(store);
  DAQ::LocationSet sources = _parse_targets(camera.sources(), argc-5, (const char**)&SOURCES);


  if(CHECK_DATA)
  {
    printf("Calculating expected CRC values for Sources\n");
    DAQ::ScienceSet science;
    DAQ::GuidingSet guiding;
   
    DAQ::LocationSet remaining = sources;
    DAQ::Location loc;
    while(remaining.remove(loc))
    {
      unsigned size = EXPECTED_SENSOR_SIZE;
      if     (science.has(loc)) size *= DAQ::Sensor::SCIENCE;
      else if(guiding.has(loc)) size *= DAQ::Sensor::GUIDE;

      expected_crc[loc.index()] = calc_crc(loc, size);
    }
  }

  //camera.science.insert(RMS::Instruction::GET, 0xA00000);
  //camera.science.insert(RMS::Instruction::GET, 0xA00001);
  //camera.science.insert(RMS::Instruction::GET, 0xA00002);
  //camera.science.insert(RMS::Instruction::GET, 0xA00003);
  //camera.science.insert(RMS::Instruction::GET, 0xA00004);
  //camera.science.insert(RMS::Instruction::GET, 0xA00005);
  //camera.science.insert(RMS::Instruction::GET, 0xA00006);
  //camera.science.insert(RMS::Instruction::GET, 0xA00007);
  //camera.science.insert(RMS::Instruction::GET, 0xA00008);
  //camera.science.insert(RMS::Instruction::GET, 0xA00009);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000a);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000b);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000c);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000d);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000e);
  //camera.science.insert(RMS::Instruction::GET, 0xA0000f);

  if(!sources) {printf(NO_SOURCES); return -1;}

  IMS::Stream* img_stream = new IMS::Stream(store);
  IMS::Stream* src_stream = new IMS::Stream(store);
  
  unsigned repeats = atol(REPEATS);
  unsigned image_number = atol(START);
  char image_name[64];
  char* image = &image_name[0];
  unsigned iterations = 0;

  while(!doExit)
   {
     sprintf(image_name, "%s%i", IMAGE_BASE, image_number);

     IMS::Path path(image, folder);
       
     if(!path) {printf(NOT_VALID, image_name); return -1;}
       
     const char* annotation = (const char*)0;
     int32_t     opcode     = 7;
     
     if(opcode == INVALID) {printf(INVALID_OPCODE); return -1;}

     IMS::ImageMetadata metadata(path.image(), path.folder(), sources, opcode, annotation); 
     
     int32_t error = camera.trigger(metadata);

     if(!error)
     {
       printf(SUCCESS, opcode,  image_name, metadata.timestamp().decode());
       IMS::Image stream_image(store, *img_stream);

       if(OVERLAP_CRC)
       {
	 IMS::Editor::Crc32 decoder(stream_image);
	 decoder.run(*src_stream, SOURCE_TIMEOUT);

	 if(CHECK_DATA)
	 {
	   bool error = _crc_error(stream_image, decoder);
	   doExit |= error & ERROR_EXIT;
	 }
	 if(DUMP_CRC) decoder.dump(2);
       }
       else
       {
	 IMS::Barrier barrier(stream_image);
	 barrier.block(*src_stream, SOURCE_TIMEOUT);

	 if(CHECK_DATA)
	 {
	   IMS::Editor::Crc32 decoder(stream_image);
	   decoder.run();
	   
	   bool error = _crc_error(stream_image, decoder);
	   doExit |= error & ERROR_EXIT;

	   if(DUMP_CRC) decoder.dump(2);
	 }
       }
     }
     else
     {
       printf(FAILED, opcode, image_name, CMS::Exception::decode(error));
       delete img_stream;
       img_stream = new IMS::Stream(store);
       delete src_stream;
       src_stream = new IMS::Stream(store);
     }
     ++image_number;
     if(++iterations==repeats) break;
   }
  
  return(EXIT_SUCCESS);
}


#include <string.h>
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"

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
