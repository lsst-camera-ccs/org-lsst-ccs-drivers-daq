#include <zlib.h>
#include <stdio.h>

#include "File.hh"


using namespace IMS;

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

/*
** ++
**
**
** --
*/

Editor::File::File(IMS::Image& image, const char* directory) : 
  Decoder(image),
  _processed(0),
  _locs(),
  _buffer(new uint8_t[PAGE_SIZE])
{
  sprintf(_filebase, "%s/%s_", directory, image.metadata().name());

  printf("filebase %s\n", _filebase);
  unsigned remaining = DAQ::LocationSet::SIZE;
  while(remaining--) _file[remaining] = NULL;
};

/*
** ++
**
**
** --
*/

Editor::File::File(IMS::Image& image, const DAQ::LocationSet& filter, const char* directory) : 
  Decoder(image, filter),
  _processed(0),
  _locs(),
  _buffer(new uint8_t[PAGE_SIZE])
{
  sprintf(_filebase, "%s/%s_", directory, image.metadata().name());

  printf("filebase %s\n", _filebase);

  unsigned remaining = DAQ::LocationSet::SIZE;
  while(remaining--) _file[remaining] = NULL;
};

/*
** ++
**
**
** --
*/

Editor::File::~File() 
{
  unsigned remaining = DAQ::LocationSet::SIZE;
  while(remaining--)
  {    
    if(_file[remaining]) fclose(_file[remaining]); 
  }

  delete[] _buffer;
}

/*
** ++
**
**
** --
*/

void Editor::File::process(IMS::Science::Source&   source, uint64_t length, uint64_t offset) 
{

  _write(source, length, offset);

  _processed += length;

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

/*
** ++
**
**
** --
*/

void Editor::File::process(IMS::Guiding::Source&   source, uint64_t length, uint64_t offset) 
{
  _write(source, length, offset);

  _processed += length;

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

/*
** ++
**
**
** --
*/

void Editor::File::process(IMS::Wavefront::Source& source, uint64_t length, uint64_t offset) 
{
  _write(source, length, offset);

  _processed += length;

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

void Editor::File::_write(IMS::Source& source, uint64_t length, uint64_t base) 
{
  DAQ::Location loc = source.location();

  _locs.insert(loc);

  if(NULL == _file[loc.index()]) 
  {
    char filename[64];
    sprintf(filename, "%s%02i.%i", _filebase, loc.bay(), loc.board());
    printf("opening file %s\n", filename);
    _file[loc.index()] = fopen(filename, "wb");
  }
  
  FILE* file = _file[loc.index()];

  uint64_t remaining = length;
  while(remaining)
  {
    uint64_t size = PAGE_SIZE<remaining ? PAGE_SIZE : remaining;
    source.read((char*)_buffer, size);

    fwrite(_buffer, sizeof(uint8_t), size, file);

    if(source.counters().read.retries || source.counters().read.timouts)
      source.counters().read.dump();

    remaining -= size;
  }
}
