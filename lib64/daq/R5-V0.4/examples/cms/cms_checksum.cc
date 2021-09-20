
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>

#include "daq/LocationSet.hh"
#include "rms/InstructionList.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"
#include "ims/Stream.hh"
#include "ims/Barrier.hh"

#define PROGRAM    argv[0]
#define PARTITION  argv[1]
#define FOLDER     argv[2]
#define IMAGE_BASE argv[3]
#define SOURCES    argv[4]

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: cms_checksum <partition> <folder> <image name> <sources>\n";

static const DAQ::LocationSet _parse_targets(const DAQ::LocationSet&, int argc, const char** argv);

static uint8_t* _read_source(IMS::Source& source, uint8_t* buffer);

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
  
  if(argc < 3) {printf(USAGE); return EXIT_SUCCESS;}
 
  IMS::Store store(PARTITION);
  CMS::Camera camera(store);

  IMS::Id id = store.catalog.lookup(IMAGE_BASE, FOLDER);
  IMS::Image image(id, store);

  if(!image) {printf(NOT_VALID, FOLDER, IMAGE_BASE); return -1;}

  //image.synopsis();

  DAQ::LocationSet locs = _parse_targets(camera.sources(), argc-4, (const char**)&SOURCES);

  //camera.sources().print();

  if(!locs) locs = image.metadata().elements();

  DAQ::Location loc;
  while(locs.remove(loc))
  {

    IMS::Source source(id, loc, store);
    
    //source.synopsis();

    static const int BUF_SIZE = 0x10000000;
    uint8_t* buf = new uint8_t[BUF_SIZE];

    memset(buf, 0, BUF_SIZE);

    uint8_t* data = _read_source(source, buf);
    if(!data) printf("Error reading source\n");

    /////uint8_t* data = buf;
    /////if(source.read((char*)data, source.size())) 
    /////{
    /////  printf("error reading\n");
    /////  return 0;
    /////}   

    //source.counters().dump();

    if(0)
    {
      char filename[64];
      sprintf(filename, "/tmp/jgt/%s_%02i.%i",image.metadata().name(), loc.bay(), loc.board());
      
      printf("Writing to file %s\n", filename);
      
      FILE* file = fopen(filename,"wb");
      
      fwrite(data, sizeof(uint8_t), source.size(), file);
      
      fclose(file);
    }
    
    if(0)
    {
      printf("%s %02i.%i\n", image.metadata().name(), loc.bay(), loc.board());
      uint32_t* ptr = (uint32_t*)data;
      for(uint64_t i=0; i<30; ++i)
      {
        printf("%lli: %08X\n", (unsigned long long)i, ptr[i]);
      }
    }
    
    //continue;

    uint32_t crc = crc32(0, NULL, 0);
    crc = crc32(crc, data, source.size());

    char locbuf[16];
    printf("%s - %016llX - %s - %llu - %08X\n", image.metadata().timestamp().decode(), (unsigned long long)image.id().value(), loc.encode(locbuf), (unsigned long long)source.size(), crc);

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

//OK
//#define PAGE_SIZE 42135552 // 195072*216
#define PAGE_SIZE 21067776 // 195072*108
//#define PAGE_SIZE 1755648    // LCM of One Page and One Stripe (108B)
//#define PAGE_SIZE 975360   // 195072*5
//#define PAGE_SIZE 780288   // 195072*4
//#define PAGE_SIZE 585216   // 195072*3
//#define PAGE_SIZE 390144   // 195072*2
//#define PAGE_SIZE 195072   // One Page
//#define PAGE_SIZE 65024    // 195072/3 // One Pagelet
//#define PAGE_SIZE 32512    // 195072/6
//#define PAGE_SIZE 16256    // 195072/12
//#define PAGE_SIZE 8128     // 195072/24
//#define PAGE_SIZE 4064     // 195072/48
//#define PAGE_SIZE 2032     // 195072/96
//#define PAGE_SIZE 1016     // 195072/192

// Interestingly?  OK
//#define PAGE_SIZE 512

//NOK
//#define PAGE_SIZE 2097152 // 2 MB
//#define PAGE_SIZE 260096 // 65024*4 // 4 Pagelets
//#define PAGE_SIZE 3048   // 1016*3
//#define PAGE_SIZE 1536   // 512*3
//#define PAGE_SIZE 1024   // 512*2

static uint8_t* _read_source(IMS::Source& source, uint8_t* buffer)
{

  uint64_t remaining = source.size();
  uint64_t offset = 0;
  uint8_t* write_buf = buffer;
  while(remaining)
  {
    //    if(remaining%0x10000 == 0) printf("remaining %lli\n", (unsigned long long)remaining);

    uint64_t size = PAGE_SIZE<remaining ? PAGE_SIZE : remaining;
    int error = source.read((char*)write_buf, size, offset);
    if(error) return 0;
    write_buf += size;
    remaining -= size;
    offset    += size;
  }
  
  return buffer;
}